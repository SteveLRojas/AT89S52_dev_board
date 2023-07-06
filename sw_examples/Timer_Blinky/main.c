#include <at89x52.h>

__sfr __at(0xFF) nothing;

void mDelay10uS(unsigned int n);              // Delay in uS
void mDelaymS(unsigned int n);              // Delay in mS

// PORTS - Other
#define LED_TEST P1_0
#define LED_TEST1 P1_1


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

void timer_init(void)
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

void main(void)
{
    timer_init();

    // Enable global interrupts
    EA = 1;

    while(1)
    {
        LED_TEST1 = 1;
        mDelaymS(100);
        LED_TEST1 = 0;
        mDelaymS(100);
    }
}

// Interrupt service routine for INT5
void timer2_isr(void) __interrupt TF2_VECTOR
{
    TF2 = 0;    //clear the interrupt flag
    // Your code here
    LED_TEST = !LED_TEST;
}
