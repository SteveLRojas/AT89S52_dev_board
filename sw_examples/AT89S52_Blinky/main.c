#include <at89x52.h>

__sfr __at(0xFF) nothing;

void mDelay10uS(unsigned int n);              // Delay in uS
void mDelaymS(unsigned int n);              // Delay in mS

// PORTS - Other
#define LED_TEST P1_0


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

void main(void)
{
    while(1)
    {
        LED_TEST = 1;
        mDelaymS(100);
        LED_TEST = 0;
        mDelaymS(100);
    }
}
