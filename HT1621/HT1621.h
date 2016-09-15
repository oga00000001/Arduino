/* HT1621 - Holtek RAM Mapping 32x4 LCD Controller */
#ifndef HT1621_H
#define HT1621_H
#include "7seg_fonts.h"

#include <inttypes.h>
#define MEM_MAX 9

// ICON
#define ICON_ON true
#define ICON_OFF false
#define ICON_1 0
#define ICON_COLON 1
#define ICON_H          2
#define ICON_TIME       3
#define ICON_CAUTION    4
#define ICON_ASTERISK   5
#define ICON_POINTL     6
#define ICON_POINTR     7
#define UNIT_k          0b01000000
#define UNIT_W          0b00001111
#define UNIT_V          0b00001100
#define UNIT_A          0b00010001
#define UNIT_h          0b00100000
#define UNIT_MD         0b10000000

class HT1621 {
public:
    enum {
        SYS_DIS   = 0b00000000,
        SYS_EN    = 0b00000001,
        LCD_OFF   = 0b00000010,
        LCD_ON    = 0b00000011,
        TIMER_DIS = 0b00000100,
        WDT_DIS   = 0b00000101,
        TIMER_EN  = 0b00000110,
        WDT_EN    = 0b00000111,
        TONE_OFF  = 0b00001000,
        TONE_ON   = 0b00001001,
        CLR_TIMER = 0b00001100,
        CLR_WDT   = 0b00001110,
        XTAL_32K  = 0b00010100,
        RC_256K   = 0b00011000,
        EXT_256K  = 0b00011100,
        
        //Set bias to 1/2 or 1/3 cycle
        //Set to 2,3 or 4 connected COM lines
        BIAS_HALF_2_COM  = 0b00100000,
        BIAS_HALF_3_COM  = 0b00100100,
        BIAS_HALF_4_COM  = 0b00101000,
        BIAS_THIRD_2_COM = 0b00100001,
        BIAS_THIRD_3_COM = 0b00100101,
        BIAS_THIRD_4_COM = 0b00101001,
        
        IRQ_DIS   = 0b10000000,
        IRQ_EN    = 0b10001000,
        //Don't use
        TEST_ON   = 0b11100000,
        TEST_OFF  = 0b11100011
    } Commands;

    HT1621(uint8_t data, uint8_t wr, uint8_t rd, uint8_t cs) : 
        data_pin(data), wr_pin(wr), rd_pin(rd), cs_pin(cs) {}

    bool begin();

    void sendCommand(uint8_t cmd, bool first = true, bool last = true);

    void write(uint8_t address, uint8_t *data, uint8_t cnt);
    void read(uint8_t address, uint8_t *data, uint8_t cnt);

    void writeMem(uint8_t address, uint8_t data);
    uint8_t readMem(uint8_t address);

    void memset(uint8_t address, uint8_t data, uint8_t cnt);
    void icon(byte num, bool f);
    void unit(byte c);
    void write_char(byte col, char c);
    void write_mem();
    void write_str(char *st);
    void clear_mem(byte data);

private:
    byte MEM[9] = {};
    void writeBits(uint8_t data, uint8_t cnt);
    uint8_t readBits(uint8_t cnt);

    inline void initControlBus();
    inline bool testMem();
    
    uint8_t data_pin;
    uint8_t wr_pin;
    uint8_t rd_pin;
    uint8_t cs_pin;
};

#endif //HT1621_H
