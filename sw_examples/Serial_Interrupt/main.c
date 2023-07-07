#include <at89x52.h>

#define XTALFREQ 12000000UL
#define BAUDRATE 4800UL

__sfr __at(0xFF) nothing;

void mDelay10uS(unsigned int n);              // Delay in uS
void mDelaymS(unsigned int n);              // Delay in mS

// PORTS - Other
#define LED_TEST P1_0
#define LED_TEST1 P1_1

unsigned char rx_fifo_read_idx = 0;
unsigned char rx_fifo_write_idx = 0;
unsigned char rx_fifo_num_bytes = 0;
unsigned char rx_fifo_mem[32];

unsigned char tx_fifo_read_idx = 0;
unsigned char tx_fifo_write_idx = 0;
unsigned char tx_fifo_num_bytes = 0;
unsigned char tx_fifo_mem[8];

unsigned char uart_idle;


void rx_fifo_push(unsigned char data)
{
    rx_fifo_mem[rx_fifo_write_idx & 0x1F] = data;
    rx_fifo_write_idx = rx_fifo_write_idx + 1;
    rx_fifo_num_bytes = rx_fifo_num_bytes + 1;
    return;
}

unsigned char rx_fifo_pop(void)
{
    unsigned char data;
    data = rx_fifo_mem[(rx_fifo_read_idx++) & 0x1F];
    --rx_fifo_num_bytes;
    return data;
}

void tx_fifo_push(unsigned char  data)
{
    tx_fifo_mem[tx_fifo_write_idx & 0x07] = data;
    ++tx_fifo_write_idx;
    ++tx_fifo_num_bytes;
    return;
}

unsigned char tx_fifo_pop(void)
{
    unsigned char data;
    data = tx_fifo_mem[(tx_fifo_read_idx++) & 0x07];
    --tx_fifo_num_bytes;
    return data;
}

void mDelay10uS(unsigned int n)  // Delay in units of 10 uS
{
    while ( n )
    {
        ++ nothing;
        -- n;
    }
}

void mDelaymS(unsigned int n)   // Delay in mS
{
    while ( n )
    {
        mDelay10uS(100);
        -- n;
    }
}

void uart_init(void) //UART Initialization
{
    uart_idle = 1;  //uart is initially idle
    //configure the UART
    PCON = 0x80;    //double baud rate
    SCON = 0x50;    // Asynchronous mode, 8-bit data and 1-stop bit
    TMOD = 0x20;    //Timer1 in Mode2.
    TH1 = 256UL - XTALFREQ / (6UL * 32UL * BAUDRATE); // Load timer value for baudrate generation
    TR1 = 1;        //Turn ON the timer for Baud rate generation
    ES = 1;     //Enable UART interrupts
}

void uart_send(unsigned char data) // Function to Transmit the Data
{
    while(tx_fifo_num_bytes >= 8);  //wait until the tx buffer is not full
    EA = 0; //disable interrupts

    if(uart_idle)
    {
        SBUF = data;    // Load the data to be transmitted
        uart_idle = 0;
    }
    else
    {
        tx_fifo_push(data);
    }
    
    EA = 1; //enable interrupts
    return;
}


unsigned char uart_get(void) // Fuction to Recieve Data
{
    unsigned char data;
    while(!rx_fifo_num_bytes);  //wait until bytes are available

    EA = 0; // disable interrupts
    data = rx_fifo_pop();
    EA = 1; //enable interrupts

    return data;
}

void main(void)
{
    unsigned char the_byte;
    uart_init();

    // Enable global interrupts
    EA = 1;

    while(1)
    {
        while(rx_fifo_num_bytes)    //while we have bytes in the buffer
        {
            the_byte = uart_get();  //get byte
            uart_send(the_byte);    //send it back
        }

        //other code goes here
        LED_TEST1 = 1;
        mDelaymS(100);
        LED_TEST1 = 0;
        mDelaymS(100);
    }
}

// Interrupt service routine for UART
void uart_isr(void) __interrupt SI0_VECTOR
{
    if(RI)
    {
        RI = 0;
        rx_fifo_push(SBUF);
    }
    if(TI)
    {
        TI = 0;
        if(tx_fifo_num_bytes)
            SBUF = tx_fifo_pop();
        else
            uart_idle = 1;
    }
    LED_TEST = !LED_TEST;   //for debugging
}
