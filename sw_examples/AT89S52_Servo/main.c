#include <at89x52.h>
#include "main.h"

#define XTALFREQ 12000000UL
#define BAUDRATE 4800UL

// PORTS
#define SERVO_OUT P1_0

__sfr __at(0xFF) nothing;

__idata uint8_t rx_fifo_read_idx = 0;
__idata uint8_t rx_fifo_write_idx = 0;
__idata uint8_t rx_fifo_num_bytes = 0;
__idata uint8_t rx_fifo_mem[32];

__idata uint8_t tx_fifo_read_idx = 0;
__idata uint8_t tx_fifo_write_idx = 0;
__idata uint8_t tx_fifo_num_bytes = 0;
__idata uint8_t tx_fifo_mem[8];

uint8_t uart_idle;

uint8_t servo_state = 0;    //indicates the current portion of the cycle (high or low)
uint16_t servo_time = 1000;    //calculated pulse length (in microseconds)

__code char str_done[] = "Done!\n";

// Interrupt service routine for INT5
void timer2_isr(void) __interrupt TF2_VECTOR
{
    uint16_t timer_reload;
    uint8_t* timer_bytes = (uint8_t*)(&timer_reload);
    TF2 = 0;    //clear the interrupt flag
    // Toggle servo pin
    servo_state = !servo_state;
    SERVO_OUT = servo_state;
    //calculate delay for the next part of the cycle. The total cycle time is kept at 20ms
    if(servo_state)
    {
        //when the pin is high we are in the short portion of the cycle
        //next time the timer overflows it should be reloaded for the long portion of the cycle
        timer_reload = 45536 + servo_time; //same as 65535 - (20000 - servo_time), which results in a delay of 20ms - servo_time
    }
    else
    {
        //when the pin is low we are in the long portion of the cycle
        //next time the timer overflows it should be reloaded with the short delay
        timer_reload = ~servo_time; //same as 65535 - servo_time, which results in a delay of servo_time
    }
    RCAP2H = timer_bytes[1];    //high byte of timer_reload
    RCAP2L = timer_bytes[0];    //low byte of timer_relod
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
}

void main(void)
{
    uart_init();
    timer2_init();

    // Enable global interrupts
    EA = 1;

    //timer reload for 1 ms: 64536
    //timer reload for 2 ms: 63536
    uint16_t time_calc;
    while(1)
    {
        time_calc = get_dec();
        if(time_calc > 1000)
            time_calc = 1000;
        time_calc = time_calc + 1000;   //the pulse length is kept between 1000 and 2000 microseconds
        EA = 0;
        servo_time = time_calc;
        EA = 1;
        uart_write_string(str_done);
    }
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

void timer2_init(void)
{
    T2CON = 0b00000100;
    T2MOD = 0b00000000;
    TH2 = 0;
    TL2 = 0;
    //Timer will count up Fclk / 12 times per second
    //Interrupt is triggered when {TH2, TL2} overflows.
    //RCAP2H and RCAP2L are loaded into TH2 and TL2 when the timer overflows.
    RCAP2H = 0;
    RCAP2L = 0;
    //Overflow rate is (Fclk / 12) / (65536 - {RCAP2H, RCAP2L})
    ET2 = 1;    //Enable timer 2 interrupts
}

void rx_fifo_push(uint8_t data)
{
    rx_fifo_mem[rx_fifo_write_idx & 0x1F] = data;
    rx_fifo_write_idx = rx_fifo_write_idx + 1;
    rx_fifo_num_bytes = rx_fifo_num_bytes + 1;
    return;
}

uint8_t rx_fifo_pop(void)
{
    unsigned char data;
    data = rx_fifo_mem[(rx_fifo_read_idx++) & 0x1F];
    --rx_fifo_num_bytes;
    return data;
}

void tx_fifo_push(uint8_t  data)
{
    tx_fifo_mem[tx_fifo_write_idx & 0x07] = data;
    ++tx_fifo_write_idx;
    ++tx_fifo_num_bytes;
    return;
}

uint8_t tx_fifo_pop(void)
{
    unsigned char data;
    data = tx_fifo_mem[(tx_fifo_read_idx++) & 0x07];
    --tx_fifo_num_bytes;
    return data;
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

void uart_send(uint8_t data) // Function to Transmit the Data
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

uint8_t uart_get(void) // Fuction to Recieve Data
{
    unsigned char data;
    while(!rx_fifo_num_bytes);  //wait until bytes are available

    EA = 0; // disable interrupts
    data = rx_fifo_pop();
    EA = 1; //enable interrupts

    return data;
}

uint8_t uart_write_string(const char* str)
{
    uint8_t len = 0;

    while(*str)
    {
        uart_send(*str++);
        len++;
    }
    return len;
}

uint8_t uart_get_string(char* buf, uint8_t size)
{
    uint8_t bytes_read = 0;
    char received_data;

    --size; //leave space for the null byte
    while(bytes_read < size)
    {
        while(!rx_fifo_num_bytes) {};
        EA = 0; // disable interrupts
        received_data = rx_fifo_pop();
        EA = 1; //enable interrupts
        buf[bytes_read++] = received_data;
        if(received_data == '\r' || received_data == '\n')
            break;
    }
    buf[bytes_read] = '\0';
    return bytes_read;
}

uint16_t get_hex()
{
    uint8_t hexbuf[7];  //leave space for pesky CR LF and null
    uint8_t temp;
    uint16_t value = 0;

    uint8_t d;
    for(d = 0; d < 7; ++d)
    {
        hexbuf[d] = 0;
    }

    uart_get_string(hexbuf, 7);

    for(d = 0; d < 7; ++d)
    {
        temp = hexbuf[d];
        if(temp >= '0' && temp <= '9')  //convert numbers
            temp = temp - '0';
        else if(temp >= 'A' && temp <= 'F')   //convert uppercase letters
            temp = temp - 'A' + 10;
        else if(temp >= 'a' && temp <= 'f')   //convert the annoying lowercase letters
            temp = temp - 'a' + 10;
        else
            continue;

        value = value << 4;
        value = value | temp;
    }

    return value;
}

uint16_t get_dec()
{
    uint8_t hexbuf[8];  //leave space for pesky CR LF and null
    uint8_t temp;
    uint16_t value = 0;

    uint8_t d;
    for(d = 0; d < 8; ++d)
    {
        hexbuf[d] = 0;
    }

    uart_get_string(hexbuf, 8);

    for(d = 0; d < 8; ++d)
    {
        temp = hexbuf[d];
        if(temp >= '0' && temp <= '9')  //convert numbers
            temp = temp - '0';
        else
            continue;

        value = value * 10;
        value = value + temp;
    }

    return value;
}