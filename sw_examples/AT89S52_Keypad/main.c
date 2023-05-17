/* 
 * File:   main.c
 * Author: Dragomir Mikhailov
 *
 * Created on March 8, 2021, 12:46 PM
 */

#include <at89x52.h>

#define  FREQ_SYS   11059200ul    //The main frequency of the system is 11.0592MHz
#define  BUAD_RATE  115200ul

#define KP_EN P1_2
//#define DD_KP_EN TRISD7
#define DATA_PIN P1_0
//#define DD_DATA_PIN TRISD3
#define CLOCK_PIN P1_1
//#define DD_CLOCK_PIN TRISD5

__sfr __at(0xFF) nothing; 
void mDelay10uS(unsigned int n);              // Delay in uS
void mDelaymS(unsigned int n);              // Delay in mS


unsigned char KP_buf[8];
unsigned char KP_K1;
unsigned char KP_K2;

const unsigned char KP_font[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b01110111, // A
  0b01111100, // B
  0b00111001, // C
  0b01011110, // D
  0b01111001, // E
  0b01110001  // F
};

/*******************************************************************************
* Function Name  : mDelayus(UNIT16 n)
* Description    : us delay function
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/ 
void mDelay10uS(unsigned int n)  // Delay in units of 10 uS
{
    while ( n )
    {  
        ++ nothing;
        -- n;
    }
}


/*******************************************************************************
* Function Name  : mDelayms(UNIT16 n)
* Description    : ms delay function
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void mDelaymS(unsigned int n)                              // Delay in mS
{
    while ( n ) 
    {
        //mDelayuS( 1000 );
        mDelay10uS(100);
        -- n;
    }
}

void ShiftOut(unsigned char data)
{
    DATA_PIN = 0;
    for(unsigned short i = 0; i < 8; ++i)
    {
        CLOCK_PIN = 0;
        DATA_PIN = data & 0x01;
        //DD_DATA_PIN = data & 0x01;
        //mDelayuS(10);
        mDelay10uS(1);
        CLOCK_PIN = 1;
        //mDelayuS(10);
        mDelay10uS(1);
        data = data >> 1;
    }
    DATA_PIN = 1;
    //DD_DATA_PIN = 1;
}

unsigned char ShiftIn()
{
    unsigned char data;
    //DD_DATA_PIN = 1;
    DATA_PIN = 1;
    for(unsigned char d = 0; d < 8; ++d)
    {
        data = data >> 1;
        CLOCK_PIN = 0;
        //mDelayuS(10);
        mDelay10uS(1);
        CLOCK_PIN = 1;
        data = data | (((unsigned char)DATA_PIN) << 7);
        //mDelayuS(10);
        mDelay10uS(1);
    }
    return data;
}

void KP_Initialize(unsigned char level)
{
    KP_EN = 0;
    ShiftOut(0x44);
    KP_EN = 1;
    mDelaymS(1);
    
    KP_EN = 0;
    ShiftOut(0x88 | level);
    KP_EN = 1;
    mDelaymS(1);
}

void KP_SetAddress(unsigned char address)
{
    KP_EN = 0;
    ShiftOut(0xC0 | (address << 1));
    KP_EN = 1;
    mDelaymS(1);
}

void KP_SetData(unsigned char data)
{
    KP_EN = 0;
    ShiftOut(0x40);
    mDelaymS(1);
    ShiftOut(data);
    KP_EN = 1;
    mDelaymS(1);
}

inline void KP_SetBuf(unsigned char index, unsigned char data)
{
    KP_buf[index] = KP_font[data & 0x0F];
}

void KP_Update()
{
    unsigned char segments;
    unsigned char temp;
    for(unsigned char seg = 0; seg < 8; ++seg)
    {
        for(unsigned char dig = 0; dig < 8; ++ dig)
        {
            segments = segments >> 1;
            temp = KP_buf[dig];
            temp = temp >> seg;
            temp = temp << 7;
            segments = segments | temp;
        }
        KP_SetAddress(seg);
        KP_SetData(segments);
    }
}

inline void KP_ReadButtons()
{
    unsigned char temp;
    KP_EN = 0;
    ShiftOut(0x42);
    mDelaymS(1);
    for(unsigned char d = 0; d < 4; ++d)
    {
        temp = ShiftIn();
        KP_K1 = KP_K1 >> 1;
        KP_K2 = KP_K2 >> 1;
        temp = temp >> 1;
        if(temp & 0x01)
            KP_K2 = KP_K2 | 0x80;
        temp = temp >> 1;
        if(temp & 0x01)
            KP_K1 = KP_K1 | 0x80;
        KP_K1 = KP_K1 >> 1;
        KP_K2 = KP_K2 >> 1;
        temp = temp >> 3;
        if(temp & 0x01)
            KP_K2 = KP_K2 | 0x80;
        temp = temp >> 1;
        if(temp & 0x01)
            KP_K1 = KP_K1 | 0x80;
    }
    KP_EN = 1;
    mDelaymS(1);
}

unsigned char KP_GetKey()
{
    KP_ReadButtons();
    unsigned short all_keys = KP_K2 << 8 | KP_K1;
    unsigned char key_count = 0;
    while(key_count < 0x10)
    {
        if(all_keys & 0x0001)
            return key_count;
        all_keys = all_keys >> 1;
        key_count = key_count + 1;
    }
    return key_count;
}

int main()
{
    unsigned char current_key;
    
    //DD_KP_EN = 0;
    KP_EN = 1;
    //DD_CLOCK_PIN = 0;
    //DD_DATA_PIN = 0;
    CLOCK_PIN = 1;
    DATA_PIN = 0;

    KP_Initialize(0x01);

    KP_SetBuf(7, 0x0D);
    KP_SetBuf(6, 0x0E);
    KP_SetBuf(5, 0x0A);
    KP_SetBuf(4, 0x0D);
    KP_SetBuf(3, 0x0B);
    KP_SetBuf(2, 0x0E);
    KP_SetBuf(1, 0x0E);
    KP_SetBuf(0, 0x0F);
    KP_Update();
  
    mDelaymS(1000);

    while(1)
    {
      mDelaymS(100); // 100 ms Delay
      current_key = KP_GetKey();
      KP_SetBuf(0, current_key & 0x0F);
      KP_SetBuf(1, current_key >> 4);
      KP_SetBuf(4, KP_K1 & 0x0F);
      KP_SetBuf(5, KP_K1 >> 4);
      KP_SetBuf(6, KP_K2 & 0x0F);
      KP_SetBuf(7, KP_K2 >> 4);
      KP_Update();
    }
    //return 0;
}