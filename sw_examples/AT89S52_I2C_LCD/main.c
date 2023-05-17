/* 
 * File:   main.c
 * Author: Dragomir Mikhailov
 *
 * Created on March 8, 2021, 12:46 PM
 */

#include <at89x52.h>

#define  FREQ_SYS   11059200ul    //The main frequency of the system is 11.0592MHz
#define  BUAD_RATE  115200ul

#define KP_PORT P0
#define KP_R0 P0_0
#define KP_R1 P0_1
#define KP_R2 P0_2
#define KP_R3 P0_3
#define KP_C0 P0_4
#define KP_C1 P0_5
#define KP_C2 P0_6
#define KP_C3 P0_7

#define SDA P1_0
#define SCL P1_1

#define LCD_CMD_NOP 0x03
#define LCD_CMD_WRITE_COMMAND 0x04
#define LCD_CMD_WRITE_DATA 0x05
#define LCD_CMD_READ_COMMAND 0x06
#define LCD_CMD_READ_DATA 0x07

__sfr __at(0xFF) nothing; 
void mDelay10uS(unsigned int n);              // Delay in uS
void mDelaymS(unsigned int n);              // Delay in mS

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

void mDelay10uS(unsigned int n)  // Delay in units of 10 uS
{
    while ( n )
    {  
        ++ nothing;
        -- n;
    }
}

void mDelaymS(unsigned int n)                              // Delay in mS
{
    while ( n ) 
    {
        //mDelayuS( 1000 );
        mDelay10uS(100);
        -- n;
    }
}

void PCF_Write(unsigned char data)
{
    I2CStart();
    I2CSend(0x4E);
    I2CSend(data);
    I2CStop();
}

unsigned char PCF_Read()
{
    unsigned char data;
    I2CStart();
    I2CSend(0x4F);
    data = I2CRead();
    I2CNak();
    I2CStop();
    return data;
}

void LCD_Init()
{
    //Initial Conditions
    PCF_Write(0xF0 | LCD_CMD_NOP);
    mDelaymS(1);

    //Function Set
    PCF_Write(0x20 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    PCF_Write(0x20 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    PCF_Write(0x80 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    mDelaymS(1);

    //Display Off
    PCF_Write(0x00 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    PCF_Write(0xB0 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    mDelaymS(1);

    //Display Clear
    PCF_Write(0x00 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    PCF_Write(0x10 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    mDelaymS(1);

    //Entry Mode Set
    PCF_Write(0x00 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    PCF_Write(0x60 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    mDelaymS(1);

    //Home Command
    PCF_Write(0x00 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    PCF_Write(0x20 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    mDelaymS(1);

    //Display On
    PCF_Write(0x00 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    PCF_Write(0xF0 | LCD_CMD_WRITE_COMMAND);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    mDelaymS(1);
}

void LCD_Putc(unsigned char input)
{
    PCF_Write((input & 0xF0) | LCD_CMD_WRITE_DATA);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    PCF_Write((input << 4) | LCD_CMD_WRITE_DATA);
    PCF_Write(0xF0 | LCD_CMD_NOP);
    mDelaymS(1);
}

unsigned char KP_GetKey()
{
    unsigned int key_val = 16;
    for(unsigned int d = 0; d < 4; ++d)
    {
        switch(d)
        {
            case 0:
                KP_R0 = 0;
                KP_R1 = 1;
                KP_R2 = 1;
                KP_R3 = 1;
                break;
            case 1:
                KP_R0 = 1;
                KP_R1 = 0;
                KP_R2 = 1;
                KP_R3 = 1;
                break;
            case 2:
                KP_R0 = 1;
                KP_R1 = 1;
                KP_R2 = 0;
                KP_R3 = 1;
                break;
            case 3:
                KP_R0 = 1;
                KP_R1 = 1;
                KP_R2 = 1;
                KP_R3 = 0;
                break;
        }

        if(!KP_C0)
            key_val = (d << 2) + 0;
        if(!KP_C1)
            key_val = (d << 2) + 1;
        if(!KP_C2)
            key_val = (d << 2) + 2;
        if(!KP_C3)
            key_val = (d << 2) + 3;
    }

    KP_R0 = 1;
    KP_R1 = 1;
    KP_R2 = 1;
    KP_R3 = 1;
    return key_val;
}

int main()
{
    unsigned char current_key;
    unsigned char prev_key;

    I2CInit();
    LCD_Init();

    prev_key = KP_GetKey();
  
    mDelaymS(100);

    while(1)
    {
        mDelaymS(100); // 100 ms Delay
        current_key = KP_GetKey();
        if(current_key != prev_key)
        {
            if(current_key == 0x10)
            {
                LCD_Putc(prev_key + 0x30);
            }
            prev_key = current_key;
        }
    }
    //return 0;
}