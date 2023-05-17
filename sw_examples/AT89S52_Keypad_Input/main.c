/* 
 * File:   main.c
 * Author: Dragomir Mikhailov
 *
 * Created on March 8, 2021, 12:46 PM
 */

#include <at89x52.h>

#define  FREQ_SYS   11059200ul    //The main frequency of the system is 11.0592MHz
#define  BUAD_RATE  115200ul

#define KP_EN_PIN P1_2
#define KP_DATA_PIN P1_0
#define KP_CLOCK_PIN P1_1

#define RTC_CE_PIN  P1_3
#define RTC_DATA_PIN P1_4
#define RTC_SCLK_PIN P1_5

#define SDA P0_0
#define SCL P0_1

__sfr __at(0xFF) nothing; 
void mDelay10uS(unsigned int n);              // Delay in uS
void mDelaymS(unsigned int n);              // Delay in mS


unsigned char KP_buf[8];
unsigned char KP_K1;
unsigned char KP_K2;

__code unsigned char KP_font[] = {
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

void I2CInit()
{
    SDA = 1;
    SCL = 1;
}
 
void I2CStart()
{
    SDA = 0;
    SCL = 0;
}
 
void I2CRestart()
{
    SDA = 1;
    SCL = 1;
    SDA = 0;
    SCL = 0;
}
 
void I2CStop()
{
    SCL = 0;
    SDA = 0;
    SCL = 1;
    SDA = 1;
}
 
void I2CAck()
{
    SDA = 0;
    SCL = 1;
    SCL = 0;
    SDA = 1;
}
 
void I2CNak()
{
    SDA = 1;
    SCL = 1;
    SCL = 0;
    SDA = 1;
}
 
unsigned char I2CSend(unsigned char Data)
{
     unsigned char i, ack_bit;
     for (i = 0; i < 8; i++) {
        if ((Data & 0x80) == 0)
            SDA = 0;
        else
            SDA = 1;
        SCL = 1;
        SCL = 0;
        Data<<=1;
     }
     SDA = 1;
     SCL = 1;
     ack_bit = SDA;
     SCL = 0;
     return ack_bit;
}
 
unsigned char I2CRead()
{
    unsigned char i, Data=0;
    for (i = 0; i < 8; i++) {
        SCL = 1;
        if(SDA)
            Data |=1;
        if(i<7)
            Data<<=1;
        SCL = 0;
    }
    return Data;
}

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

void RTC_Write(unsigned char address, unsigned char data)
{
    RTC_CE_PIN = 1;
    address = address << 1;
    address = address | 0x80;
    for(unsigned char i = 0; i < 8; ++i)
    {
        RTC_DATA_PIN = address & 0x01;
        mDelay10uS(1);
        RTC_SCLK_PIN = 1;
        mDelay10uS(1);
        RTC_SCLK_PIN = 0;
        address = address >> 1;
    }
    for(unsigned char i = 0; i < 8; ++i)
    {
        RTC_DATA_PIN = data & 0x01;
        mDelay10uS(1);
        RTC_SCLK_PIN = 1;
        mDelay10uS(1);
        RTC_SCLK_PIN = 0;
        data = data >> 1;
    }
    RTC_CE_PIN = 0;
}

unsigned char RTC_Read(unsigned char address)
{
    unsigned char data;
    RTC_CE_PIN = 1;
    address = address << 1;
    address = address | 0x81;
    for(unsigned char i = 0; i < 8; ++i)
    {
        RTC_DATA_PIN = address & 0x01;
        mDelay10uS(1);
        RTC_SCLK_PIN = 1;
        mDelay10uS(1);
        RTC_SCLK_PIN = 0;
        address = address >> 1;
    }
    RTC_DATA_PIN = 1;
    for(unsigned char d = 0; d < 8; ++d)
    {
        data = data >> 1;
        mDelay10uS(1);
        RTC_SCLK_PIN = 1;
        data = data | (((unsigned char)RTC_DATA_PIN) << 7);
        mDelay10uS(1);
        RTC_SCLK_PIN = 0;
    }
    RTC_CE_PIN = 0;
    return data;
}

void KP_ShiftOut(unsigned char data)
{
    KP_DATA_PIN = 0;
    for(unsigned char i = 0; i < 8; ++i)
    {
        KP_CLOCK_PIN = 0;
        KP_DATA_PIN = data & 0x01;
        mDelay10uS(1);
        KP_CLOCK_PIN = 1;
        mDelay10uS(1);
        data = data >> 1;
    }
    KP_DATA_PIN = 1;
}

unsigned char KP_ShiftIn()
{
    unsigned char data;
    KP_DATA_PIN = 1;
    for(unsigned char d = 0; d < 8; ++d)
    {
        data = data >> 1;
        KP_CLOCK_PIN = 0;
        mDelay10uS(1);
        KP_CLOCK_PIN = 1;
        data = data | (((unsigned char)KP_DATA_PIN) << 7);
        mDelay10uS(1);
    }
    return data;
}

void KP_Initialize(unsigned char level)
{
    KP_EN_PIN = 0;
    KP_ShiftOut(0x44);
    KP_EN_PIN = 1;
    mDelaymS(1);
    
    KP_EN_PIN = 0;
    KP_ShiftOut(0x88 | level);
    KP_EN_PIN = 1;
    mDelaymS(1);
}

void KP_SetAddress(unsigned char address)
{
    KP_EN_PIN = 0;
    KP_ShiftOut(0xC0 | (address << 1));
    KP_EN_PIN = 1;
    mDelaymS(1);
}

void KP_SetData(unsigned char data)
{
    KP_EN_PIN = 0;
    KP_ShiftOut(0x40);
    mDelaymS(1);
    KP_ShiftOut(data);
    KP_EN_PIN = 1;
    mDelaymS(1);
}

inline void KP_SetBuf(unsigned char index, unsigned char data)
{
    KP_buf[index] = KP_font[data & 0x0F];
}

inline void KP_SetDot(unsigned char index)
{
    KP_buf[index] = KP_buf[index] | 0x80;
}

inline void KP_ResetDot(unsigned char index)
{
    KP_buf[index] = KP_buf[index] & 0x7F;
}

inline void KP_TurnOff(unsigned char index)
{
    KP_buf[index] = 0x00;
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
    KP_EN_PIN = 0;
    KP_ShiftOut(0x42);
    mDelaymS(1);
    for(unsigned char d = 0; d < 4; ++d)
    {
        temp = KP_ShiftIn();
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
    KP_EN_PIN = 1;
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
    unsigned char prev_key;
    unsigned char digit_idx;
    unsigned char bcd_buf[8];
    unsigned char mode;
    
    KP_EN_PIN = 1;
    KP_CLOCK_PIN = 1;
    KP_DATA_PIN = 0;

    RTC_CE_PIN = 0;
    RTC_SCLK_PIN = 0;
    RTC_DATA_PIN = 1;

    I2CInit();
    KP_Initialize(0x01);

    KP_SetBuf(7, 0x0D);
    KP_SetBuf(6, 0x0E);
    KP_SetBuf(5, 0x0A);
    KP_SetBuf(4, 0x0D);
    KP_SetBuf(3, 0x0B);
    KP_SetBuf(2, 0x0E);
    KP_SetBuf(1, 0x0E);
    KP_SetBuf(0, 0x0F);

    mode = 0;
    prev_key = KP_GetKey();
    digit_idx = 7;
    KP_SetDot(digit_idx);
    KP_Update();
  
    mDelaymS(1000);

    while(1)
    {
        mDelaymS(100); // 100 ms Delay
        current_key = KP_GetKey();
        if(current_key != prev_key)
        {
            //KP_SetBuf(0, current_key & 0x0F);
            //KP_SetBuf(1, current_key >> 4);
            //KP_SetBuf(4, KP_K1 & 0x0F);
            //KP_SetBuf(5, KP_K1 >> 4);
            //KP_SetBuf(6, KP_K2 & 0x0F);
            //KP_SetBuf(7, KP_K2 >> 4);

            if(current_key == 0x10)
            {
                switch(prev_key)
                {
                    case 0:
                        bcd_buf[digit_idx] = 7;
                        break;
                    case 1:
                        bcd_buf[digit_idx] = 8;
                        break;
                    case 2:
                        bcd_buf[digit_idx] = 9;
                        break;
                    case 3:
                        mode = 0;   //enter time
                        break;
                    case 4:
                        bcd_buf[digit_idx] = 4;
                        break;
                    case 5:
                        bcd_buf[digit_idx] = 5;
                        break;
                    case 6:
                        bcd_buf[digit_idx] = 6;
                        break;
                    case 7:
                        mode = 1;   //enter date
                        break;
                    case 8:
                        bcd_buf[digit_idx] = 1;
                        break;
                    case 9:
                        bcd_buf[digit_idx] = 2;
                        break;
                    case 10:
                        bcd_buf[digit_idx] = 3;
                        break;
                    case 11:
                        mode = 2;   //display time
                        break;
                    case 12:
                        bcd_buf[digit_idx] = 0;
                        break;
                    case 15:
                        mode = 3;   //display date
                        break;
                }

                if(mode == 0 && prev_key == 14) //set time
                {
                    //set seconds
                    RTC_Write(0x00, (bcd_buf[1] << 4) | bcd_buf[0]);
                    //set minutes
                    RTC_Write(0x01, (bcd_buf[3] << 4) | bcd_buf[2]);
                    //set hours
                    RTC_Write(0x02, (bcd_buf[5] << 4) | bcd_buf[4]);
                }

                if(mode == 1 && prev_key == 14) //set date
                {
                    //set days
                    RTC_Write(0x03, (bcd_buf[1] << 4) | bcd_buf[0]);
                    //set months
                    RTC_Write(0x04, (bcd_buf[3] << 4) | bcd_buf[2]);
                    //set years
                    RTC_Write(0x06, (bcd_buf[5] << 4) | bcd_buf[4]);
                }

                KP_SetBuf(digit_idx, bcd_buf[digit_idx]);
                digit_idx = digit_idx - 1;
                digit_idx = digit_idx & 7;
                KP_SetDot(digit_idx);
            }

            KP_Update();
            prev_key = current_key;
        }

        if(mode == 2)   //display time
        {
            //get seconds
            bcd_buf[0] = RTC_Read(0x00);
            bcd_buf[1] = (bcd_buf[0] >> 4) & 0x07;
            bcd_buf[0] = bcd_buf[0] & 0x0F;

            //get minutes
            bcd_buf[2] = RTC_Read(0x01);
            bcd_buf[3] = (bcd_buf[2] >> 4) & 0x07;
            bcd_buf[2] = bcd_buf[2] & 0x0F;

            //get hours
            bcd_buf[4] = RTC_Read(0x02);
            bcd_buf[5] = (bcd_buf[4] >> 4) & 0x03;
            bcd_buf[4] = bcd_buf[4] & 0x0F;

            KP_TurnOff(7);
            KP_TurnOff(6);
            KP_SetBuf(5, bcd_buf[5]);
            KP_SetBuf(4, bcd_buf[4]);
            KP_SetBuf(3, bcd_buf[3]);
            KP_SetBuf(2, bcd_buf[2]);
            KP_SetBuf(1, bcd_buf[1]);
            KP_SetBuf(0, bcd_buf[0]);
            KP_Update();
        }

        if(mode == 3)   //display date
        {
            //get days
            bcd_buf[0] = RTC_Read(0x03);
            bcd_buf[1] = (bcd_buf[0] >> 4);
            bcd_buf[0] = bcd_buf[0] & 0x0F;

            //get months
            bcd_buf[2] = RTC_Read(0x04);
            bcd_buf[3] = (bcd_buf[2] >> 4);
            bcd_buf[2] = bcd_buf[2] & 0x0F;

            //get years
            bcd_buf[4] = RTC_Read(0x06);
            bcd_buf[5] = (bcd_buf[4] >> 4);
            bcd_buf[4] = bcd_buf[4] & 0x0F;
            bcd_buf[7] = 2;
            bcd_buf[6] = 0;

            KP_SetBuf(7, bcd_buf[7]);
            KP_SetBuf(6, bcd_buf[6]);
            KP_SetBuf(5, bcd_buf[5]);
            KP_SetBuf(4, bcd_buf[4]);
            KP_SetBuf(3, bcd_buf[3]);
            KP_SetBuf(2, bcd_buf[2]);
            KP_SetBuf(1, bcd_buf[1]);
            KP_SetBuf(0, bcd_buf[0]);
            KP_Update();
        }
    }
    //return 0;
}