#include <wiring_private.h>

#include "HT1621.h"

#define ADDR_MAX 128

#define TAKE_CS()    digitalWrite(cs_pin, LOW)
#define RELEASE_CS() digitalWrite(cs_pin, HIGH)

inline void HT1621::initControlBus()
{
    pinMode(data_pin, OUTPUT);
    pinMode(wr_pin, OUTPUT);
    pinMode(rd_pin, OUTPUT);
    pinMode(cs_pin, OUTPUT);

    digitalWrite(cs_pin, HIGH);
    digitalWrite(wr_pin, HIGH);
    digitalWrite(rd_pin, HIGH);
    digitalWrite(data_pin, HIGH);
}

inline bool HT1621::testMem()
{
    uint8_t test = 10;
    writeMem(0x5a, test);
    if (readMem(0x5a) != test)
        return false;
    return true;
}

bool HT1621::begin()
{
    initControlBus();
    //delay(100);   //uncomment if init fails
    sendCommand(SYS_DIS);
    if (! testMem())
        return false;

    memset(0, 0, ADDR_MAX);
    sendCommand(SYS_EN);
    sendCommand(LCD_ON);
    return true;
}

void HT1621::writeBits(uint8_t data, uint8_t cnt)
{
    uint8_t bitmask;
    while (cnt) {
        digitalWrite(wr_pin, LOW);
        uint8_t bitval = (data & (1 << (cnt - 1))) ? HIGH : LOW;
        digitalWrite(data_pin, bitval);
        digitalWrite(wr_pin, HIGH);
        cnt--;
    }
}

uint8_t HT1621::readBits(uint8_t cnt)
{
    uint8_t data = 0;
    pinMode(data_pin, INPUT);
    while (cnt) {
        digitalWrite(rd_pin, LOW);
        data += digitalRead(data_pin) << (cnt - 1);
        digitalWrite(rd_pin, HIGH);
        cnt--;
    }
    pinMode(data_pin, OUTPUT);
    return data;
}

#define COMMAND_MODE 0b100
void HT1621::sendCommand(uint8_t cmd, bool first /*= true*/, bool last /*= true*/)
{
    if (first) {
        TAKE_CS();
        writeBits(COMMAND_MODE, 3);
    }
    writeBits(cmd, 8);
    writeBits(0, 1); //Last bit - don't care
    if (last)
        RELEASE_CS();
}

#define WRITE_MODE 0b101
void HT1621::writeMem(uint8_t address, uint8_t data)
{
    TAKE_CS();
    writeBits(WRITE_MODE, 3);
    writeBits(address, 6);
    writeBits(data, 4);
    RELEASE_CS();
}

void HT1621::memset(uint8_t address, uint8_t data, uint8_t cnt)
{
  TAKE_CS();
  writeBits(WRITE_MODE, 3);
  writeBits(address, 6);
  for (uint8_t i = 0; i < cnt; i++)
    writeBits(data, 4);
  RELEASE_CS();
}

//Write up to 256 values starting from address
//Note: Data is 8-bit aligned. This is not vary efficient
void HT1621::write(uint8_t address, uint8_t *data, uint8_t cnt)
{
    TAKE_CS();
    writeBits(WRITE_MODE, 3);
    writeBits(address, 6);
    for (uint8_t i = 0; i < cnt; i++)
        writeBits(data[i], 4);
    RELEASE_CS();
}

#define READ_MODE 0b110
uint8_t HT1621::readMem(uint8_t address)
{
    uint8_t data;
    TAKE_CS();
    writeBits(READ_MODE, 3);
    writeBits(address, 6);
    data = readBits(4);
    RELEASE_CS();
    return data;
}

#define READ_MODE 0b110
void HT1621::read(uint8_t address, uint8_t *data, uint8_t cnt)
{
    TAKE_CS();
    writeBits(READ_MODE, 3);
    writeBits(address, 6);
    for (uint8_t i = 0; i < cnt; i++)
        data[i] = readBits(4);
    RELEASE_CS();
}

void HT1621::icon(byte num, bool f)
{
  if (f)
  { 
    MEM[num] = MEM[num] | 0x01;  
  } else 
  {
    MEM[num] = MEM[num] & 0xFE;  
  }
}
void HT1621::unit(byte c)
{
  MEM[8] = c;
}

void HT1621::write_char(byte col, char c)
{
  byte num = c -0x20;
  MEM[col] = (MEM[col] & 0x01) | CHAR_DATA[num]; 
}
void HT1621::write_mem()
{
  for (int i =0; i<9; i++) {
    writeMem(i*2,  (MEM[i]>>4) & 0x0F);
    writeMem(i*2+1, (MEM[i]) & 0x0F);
  }
}

void HT1621::write_str(char *st)
{
  int stl, i, n;
  char c;
  stl = strlen(st);
  n =0;
  icon(ICON_COLON, ICON_OFF);  
  icon(ICON_POINTL, ICON_OFF);
  icon(ICON_POINTR, ICON_OFF);
  for (i=0; i<stl; i++) {
    c = *st++;
    if (c == ':') {
      icon(ICON_COLON, ICON_ON);
    }
    else if (c == '.') {
      if (n == 6) {
        icon(ICON_POINTL, ICON_ON);
      } else if (n == 7) {
        icon(ICON_POINTR, ICON_ON);
      }
    }
    else
    {
      write_char(n, c);
      n++;
    }
  }
}

void HT1621::clear_mem(byte data)
{
  for (int i = 0; i<MEM_MAX;i++){
    MEM[i] = data;
  }
  write_mem();
}

