#include <at89x52.h>

// SDCC
#define uint8_t unsigned char
#define uint16_t unsigned short

// PORTS
#define LED_TEST0 P1_0
#define LED_TEST1 P1_1
#define MAX7219_DATA P0_0
#define MAX7219_CS P0_1
#define MAX7219_CLK P0_2

__sfr __at(0xFF) nothing;

__code uint8_t max7219_lut[16] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47};
    //0111 1110; // '0'     0x7E
    //0011 0000; // '1'     0x30
    //0110 1101; // '2'     0x6D
    //0111 1001; // '3'     0x79
    //0011 0011; // '4'     0x33
    //0101 1011; // '5'     0x5B
    //0101 1111; // '6'     0x5F
    //0111 0000; // '7'     0x70
    //0111 1111; // '8'     0x7F
    //0111 1011; // '9'     0x7B
    //0111 0111; // 'A'     0x77
    //0001 1111; // 'b'     0x1F
    //0100 1110; // 'C'     0x4E
    //0011 1101; // 'd'     0x3D
    //0100 1111; // 'E'     0x4F
    //0100 0111; // 'F'     0x47

volatile uint16_t counter = 0;
uint16_t prev_counter = 0;
uint16_t rnd_reg = 0;

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

void rnd_generate(void)
{
    //rnd_reg <= {~(rnd_reg[7] ^ rnd_reg[0]), rnd_reg[15:1]}
    uint16_t temp;
    temp = rnd_reg >> 7;
    temp = temp ^ rnd_reg;
    temp = temp ^ 1;
    temp = temp << 15;
    rnd_reg = rnd_reg >> 1;
    rnd_reg = rnd_reg | temp;
}

uint8_t build_byte(void)
{
    for(uint8_t d = 0; d < 8; ++d)
    {
        rnd_generate();
    }
    return (uint8_t)rnd_reg;
}

uint16_t build_word(void)
{
    for(uint8_t d = 0; d < 16; ++d)
    {
        rnd_generate();
    }
    return rnd_reg;
}

void timer_init(uint8_t rcap_h, uint8_t rcap_l)
{
    T2CON = 0b00000100;
    T2MOD = 0b00000000;
    TH2 = 0;
    TL2 = 0;
    //Timer will count up Fclk / 12 times per second
    //Interrupt is triggered when {TH2, TL2} overflows.
    //RCAP2H and RCAP2L are loaded into TH2 and TL2 when the timer overflows.
    RCAP2H = rcap_h;
    RCAP2L = rcap_l;
    //Overflow rate is (Fclk / 12) / (65536 - {RCAP2H, RCAP2L})
    ET2 = 1;    //Enable timer 2 interrupts
}

void max7219_shift_out(uint16_t data)
{
    MAX7219_CS = 0;

    for(uint8_t d = 0; d < 16; ++d)
    {
        MAX7219_DATA = (data & 0x8000);
        data = data << 1;
        MAX7219_CLK = 1;
        MAX7219_CLK = 0;
    }

    MAX7219_CS = 1;
    return;
}

void max7219_init(uint8_t intensity)
{
    intensity = intensity & 0x0F;

    max7219_shift_out(0x0900);  //bypass decoding for all digits
    max7219_shift_out(0x0A00 | (uint16_t)intensity);    //set intensity
    max7219_shift_out(0x0B07);  //scan all digits
    max7219_shift_out(0x0C01);  //Not shut down
    max7219_shift_out(0x0F00);  //not in test mode

    max7219_shift_out(0x0180);
    max7219_shift_out(0x0280);
    max7219_shift_out(0x0380);
    max7219_shift_out(0x0480);
    max7219_shift_out(0x0580);
    max7219_shift_out(0x0680);
    max7219_shift_out(0x0780);
    max7219_shift_out(0x0880);  //clear all digits
    return;
}

void max7219_update(uint16_t h_word, uint16_t l_word, uint8_t dp)
{
    uint8_t temp;
    uint16_t command;
    uint8_t d;

    d = 0;
    while(d < 4)
    {
        temp = dp & 0x01;
        temp = temp << 7;
        temp = temp | max7219_lut[(uint8_t)l_word & 0x0F];
        ++d;
        command = (uint16_t)(d);
        command = command << 8;
        command = command | temp;
        max7219_shift_out(command);
        l_word = l_word >> 4;
        dp = dp >> 1;
    }

    while(d < 8)
    {
        temp = dp & 0x01;
        temp = temp << 7;
        temp = temp | max7219_lut[(uint8_t)h_word & 0x0F];
        ++d;
        command = (uint16_t)(d);
        command = command << 8;
        command = command | temp;
        max7219_shift_out(command);
        h_word = h_word >> 4;
        dp = dp >> 1;
    }
    return;
}

void main(void)
{
    uint8_t dp;
    uint16_t rnd_word;

    P1_5 = 1;
    P1_6 = 1;
    P1_7 = 1;
    LED_TEST0 = 0;
    LED_TEST1 = 0;
    MAX7219_CS = 1;
    MAX7219_CLK = 0;

    max7219_init(0x08); //initialize with medium intensity
    mDelaymS(200);

    timer_init(0x00, 0x00); //set timer for T = 65.5 ms.

    // Enable global interrupts
    EA = 1;

    while(1)
    {
        if(counter != prev_counter)
        {
            dp = 0x01 << ((uint8_t)counter & 0x07);
            rnd_word = build_word();
            max7219_update(rnd_word, counter, dp);
            prev_counter = counter;
            LED_TEST1 = !LED_TEST1;
        }
    }
}

// Interrupt service routine for INT5
void timer2_isr(void) __interrupt TF2_VECTOR
{
    TF2 = 0;    //clear the interrupt flag
    // Your code here
    LED_TEST0 = !LED_TEST0;
    ++counter;
}
