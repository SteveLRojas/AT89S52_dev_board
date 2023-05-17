//                     /\         /\__
//                   // \       (  0 )_____/\            __
//                  // \ \     (vv          o|          /^v\
//                //    \ \   (vvvv  ___-----^        /^^/\vv\
//              //  /     \ \ |vvvvv/               /^^/    \v\
//             //  /       (\\/vvvv/              /^^/       \v\
//            //  /  /  \ (  /vvvv/              /^^/---(     \v\
//           //  /  /    \( /vvvv/----(O        /^^/           \v\
//          //  /  /  \  (/vvvv/               /^^/             \v|
//        //  /  /    \( vvvv/                /^^/               ||
//       //  /  /    (  vvvv/                 |^^|              //
//      //  / /    (  |vvvv|                  /^^/            //
//     //  / /   (    \vvvvv\          )-----/^^/           //
//    // / / (          \vvvvv\            /^^^/          //
//   /// /(               \vvvvv\        /^^^^/          //
//  ///(              )-----\vvvvv\    /^^^^/-----(      \\
// //(                        \vvvvv\/^^^^/               \\
///(                            \vvvv^^^/                 //
//                                \vv^/         /        //
//                                             /<______//
//                                            <<<------/
//                                             \<
//                                              \
//****************************************************
//* Climate Sensor with Calendar for AT89S52         *
//* Copyright (C) 2023 all rights reserved           *
//*     Esteban Looser and Jakob Grootens.           *
//*                                                  *
//*                                                  *
//****************************************************

#include <at89x52.h>

#define XTALFREQ 12000000UL
#define BAUDRATE 4800UL

__sfr __at(0xFF) nothing;

void mDelay10uS(unsigned int n);              // Delay in uS
void mDelaymS(unsigned int n);              // Delay in mS

// PORTS - Other
#define LED_TEST P0_0


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
    PCON = 0x80;    //double baud rate
    SCON = 0x50;    // Asynchronous mode, 8-bit data and 1-stop bit
    TMOD = 0x20;    //Timer1 in Mode2.
    TH1 = 256UL - XTALFREQ / (6UL * 32UL * BAUDRATE); // Load timer value for baudrate generation
    TR1 = 1;        //Turn ON the timer for Baud rate generation
}

void uart_send(unsigned char ch) // Function to Transmit the Data
{
    SBUF = ch;      // Load the data to be transmitted
    while(TI == 0); // Wait till the data is trasmitted
    TI = 0;         //Clear the Tx flag for next cycle.
}


unsigned char uart_get(void) // Fuction to Recieve Data
{
    while(RI == 0); // Wait till the data is received
    RI = 0;         // Clear Receive Interrupt Flag for next cycle
    return SBUF;    // return the received char
}

void main(void)
{
    const char* msg = "Hello World!";
    unsigned char data;

    uart_init();

    for(unsigned char d = 0; msg[d]; ++d)
    {
        uart_send(msg[d]); // Transmit predefined string
    }

    while(1)
    {
        data = uart_get(); // Receive a char from serial port
        uart_send(data);    // Transmit the received char
    }
}
