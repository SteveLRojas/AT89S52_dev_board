#include <SPI.h>

#define CMD_GENUINE_BULLSHIT 0xF0
#define CMD_PING 0xDE
#define CMD_READ 0x01
#define CMD_WRITE 0x02
#define CMD_UNLOCK_ALL 0x04
#define CMD_LOCK_ALL 0x05
#define CMD_GET_STATUS 0x06
#define CMD_ERASE_ALL 0x07
#define CMD_LOCK_BLOCK 0x08
#define CMD_ERASE_BLOCK 0x09
#define CMD_SET_ADDRESS 0x0A
#define CMD_GET_PLATFORM_DESCRIPTOR 0x0B
#define CMD_GET_TARGET_DESCRIPTOR 0x0C
#define CMD_GET_NUM_TARGETS 0x0D
#define CMD_SET_TARGET 0x0E
#define CMD_TEST 0x0F

//Avoid using the SPI SS pin, it can cause the Arduino to hang
#define PIN_AT_RST 9
//The following pins must match the location of the SPI pins on the Arduino
#define PIN_AT_MOSI 11
#define PIN_AT_MISO 12
#define PIN_AT_SCK 13

byte is_unlocked = 0;   //not zero if the correct handshake has been received (0xDEADBEEF)
byte data_buf[256];
const char fs_platform_desc[] PROGMEM = "Arduino UMP";
const char fs_target_desc[] PROGMEM = "AT89S52";

void test_print(const char* str) //sends a string including its null terminator
{
    char temp;

    for(int d = 0; (temp = pgm_read_byte_near(str + d)); ++d)
    {
        Serial.write(temp);
    }

    return;
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(PIN_AT_RST, OUTPUT);
    digitalWrite(PIN_AT_RST, HIGH);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV32);
    SPI.setDataMode(SPI_MODE0);
    
    delay(100);
}

byte at_prog_en()
{
    SPI.transfer(0xAC);
    SPI.transfer(0x53);
    SPI.transfer(0x00);
    return SPI.transfer(0x00);
}

void at_chip_erase()
{
    SPI.transfer(0xAC);
    SPI.transfer(0x80);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    delay(500);
    return;
}

byte at_read_byte(short addr)
{
    SPI.transfer(0x20);
    SPI.transfer((byte)(addr >> 8));
    SPI.transfer((byte)addr);
    return SPI.transfer(0x00);
}

void at_write_byte(short addr, byte data)
{
    SPI.transfer(0x40);
    SPI.transfer((byte)(addr >> 8));
    SPI.transfer((byte)addr);
    SPI.transfer(data);
    return;
}

void at_write_lock_bits(byte lock_bits)
{
    SPI.transfer(0xAC);
    SPI.transfer(0xE0 | (lock_bits & 0x03));
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    return;
}

byte at_read_lock_bits()
{
    SPI.transfer(0x24);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    return SPI.transfer(0x00);
}

byte at_read_signature_byte(short addr)
{
    SPI.transfer(0x28);
    SPI.transfer((byte)(addr >> 8));
    SPI.transfer((byte)addr);
    return SPI.transfer(0x00);
}

void at_read_page(short addr)
{
    SPI.transfer(0x30);
    SPI.transfer((byte)(addr >> 8));

    byte index = 0;
    do
    {
        data_buf[index] = SPI.transfer(0x00);
        ++index;
    } while(index);

    return;
}

void at_write_page(short addr)
{
    SPI.transfer(0x50);
    SPI.transfer((byte)(addr >> 8));

    byte index = 0;
    do
    {
        SPI.transfer(data_buf[index]);
        delayMicroseconds(500);
        ++index;
    } while(index);

    return;
}

void wait_handshake(byte skip_initial)
{
    is_unlocked = 0;

    if(!skip_initial)
    {
        while(!Serial.available());
        if(Serial.read() != 0xDE)
            return;
    }

    while(!Serial.available());
    if(Serial.read() != 0xAD)
        return;

    while(!Serial.available());
    if(Serial.read() != 0xBE)
        return;

    while(!Serial.available());
    if(Serial.read() != 0xEF)
        return;

    is_unlocked = 0xFF;

    //send ACK
    Serial.write(0x55);
    Serial.write(0xAA);
    Serial.write(0xF0);
    Serial.write(0x0F);

    //Send size
    Serial.write(0x00);
    Serial.write(0x00);
    Serial.write(0x20);
    Serial.write(0x00);
    
    return;
}

//This function expects the input string to reside in flash
void serial_send_string(const char* str) //sends a string including its null terminator
{
    char temp;
    byte index = 0;
    
    do
    {
        temp = pgm_read_byte_near(str + index);
        ++index;
        Serial.write(temp);
    } while(temp);

    return;
}

void com_read()
{
    short bytes_remaining;
    short packet_size;
    short address = 0;

    //get number of bytes to read
    while(!Serial.available());
    Serial.read();
    while(!Serial.available());
    Serial.read();
    while(!Serial.available());
    bytes_remaining = Serial.read();
    bytes_remaining = bytes_remaining << 8;
    while(!Serial.available());
    bytes_remaining = bytes_remaining | Serial.read();

    //respond to request
    Serial.write(0x00);
    Serial.write(0x00);
    Serial.write((byte)(bytes_remaining >> 8));
    Serial.write((byte)bytes_remaining);

    while(bytes_remaining)
    {
        if(bytes_remaining > 256)
        {
            packet_size = 256;
        }
        else
        {
            packet_size = bytes_remaining;
        }

        //read the bytes from the target
        at_read_page(address);

        //send the bytes
        for(short d = 0; d < packet_size; ++d)
        {
            Serial.write(data_buf[d]);
        }

        bytes_remaining = bytes_remaining - packet_size;
        address = address + packet_size;
    }
    
    return;
}

void com_write()
{
    short bytes_remaining;
    short packet_size;
    short address = 0;

    //get number of bytes to read
    while(!Serial.available());
    Serial.read();
    while(!Serial.available());
    Serial.read();
    while(!Serial.available());
    bytes_remaining = Serial.read();
    bytes_remaining = bytes_remaining << 8;
    while(!Serial.available());
    bytes_remaining = bytes_remaining | Serial.read();

    while(bytes_remaining)
    {
        if(bytes_remaining > 256)
        {
            packet_size = 256;
        }
        else
        {
            packet_size = bytes_remaining;
        }

        //request bytes
        Serial.write(0x00);
        Serial.write(0x00);
        Serial.write((byte)(packet_size >> 8));
        Serial.write((byte)packet_size);

        //receive bytes
        if(packet_size != 256)  //clear buffer to avoid writing garbage in the rest of the page
        {
            byte index = 0;
            do
            {
                data_buf[index] = 0xFF;
                ++index;
            } while(index);
        }
        for(short d = 0; d < packet_size; ++d)
        {
            while(!Serial.available());
            data_buf[d] = Serial.read();
        }

        //write the bytes
        at_write_page(address);

        bytes_remaining = bytes_remaining - packet_size;
        address = address + packet_size;
    }
    
    return;
}

void com_unlock_all()
{
    pinMode(PIN_AT_RST, OUTPUT);
    digitalWrite(PIN_AT_RST, HIGH);
    pinMode(PIN_AT_SCK, OUTPUT);
    pinMode(PIN_AT_MOSI, OUTPUT);
    delay(100);
    
    byte temp = at_prog_en();
    
    if(temp != 0x69)
    {
        Serial.write(0xE1);
        Serial.write(temp);
        return;
    }

    temp = at_read_signature_byte(0x0000);
    if(temp != 0x1E)
    {
        Serial.write(0xE2);
        Serial.write(temp);
        return;
    }

    temp = at_read_signature_byte(0x0100);
    if(temp != 0x52)
    {
        Serial.write(0xE3);
        Serial.write(temp);
        return;
    }

    temp = at_read_signature_byte(0x0200);
    if(temp != 0x06)
    {
        Serial.write(0xE4);
        Serial.write(temp);
        return;
    }

    Serial.write(0x55);
    Serial.write(0xAA);
    
    return;
}

void com_lock_all()
{
    pinMode(PIN_AT_SCK, INPUT);
    pinMode(PIN_AT_MOSI, INPUT);
    pinMode(PIN_AT_RST, INPUT);
    Serial.write(0x55);
    Serial.write(0xAA);
    return;
}

void com_get_platform_descriptor()
{
    serial_send_string(fs_platform_desc);
    return;
}

void com_get_target_descriptor()
{
    serial_send_string(fs_target_desc);
    return;
}

void com_get_num_targets()
{
    Serial.write(0x01);
    return;
}

void loop()
{
    // put your main code here, to run repeatedly:
    if(!is_unlocked)
    {
        wait_handshake(0);
        return;
    }

    while(!Serial.available());
    switch(Serial.read())
    {
        case CMD_GENUINE_BULLSHIT:
            return; //Ignore the bad bytes inserted by the USB to serial chip on genuine arduino boards...
        case CMD_PING:
            wait_handshake(1);
            break;
        case CMD_READ:
            com_read();
            break;
        case CMD_WRITE:
            com_write();
            break;
        case CMD_UNLOCK_ALL:
            com_unlock_all();
            break;
        case CMD_LOCK_ALL:
            com_lock_all();
            break;
        case CMD_GET_STATUS:
            Serial.write(0xE0);
            Serial.write(0x00);
            break;
        case CMD_ERASE_ALL:
            at_chip_erase();
            Serial.write(0x55);
            Serial.write(0xAA);
            break;
        case CMD_LOCK_BLOCK:
            Serial.write(0xE0);
            Serial.write(0x00);
            break;
        case CMD_ERASE_BLOCK:
            Serial.write(0xE0);
            Serial.write(0x00);
            break;
        case CMD_SET_ADDRESS:
            Serial.write(0xE0);
            Serial.write(0x00);
            break;
        
        case CMD_GET_PLATFORM_DESCRIPTOR:
            com_get_platform_descriptor();
            break;
        case CMD_GET_TARGET_DESCRIPTOR:
            com_get_target_descriptor();
            break;
        case CMD_GET_NUM_TARGETS:
            com_get_num_targets();
            break;
        case CMD_SET_TARGET:
            while(!Serial.available());
            Serial.read();
            break;
        default:
            is_unlocked = 0;
            //report error
            Serial.write(0x00);
            Serial.write(0x00);
            Serial.write(0x00);
            Serial.write(0x00);
    }

    return;
}


// End
