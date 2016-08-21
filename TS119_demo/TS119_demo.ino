/**
 * TEST
 * TS119-3
 * TS119-5  http://www.aitendo.com/product/5152
 * TS174    http://www.aitendo.com/product/5153
 * http://www.holtek.com.tw/documents/10179/a33bf4b4-f0ef-4b77-94e4-3dd5d0c35f16
 * 
 * HT1621 Library
 * https://storage.googleapis.com/google-code-archive-source/v2/code.google.com/wirejungle/source-archive.zip
 * 
 * reference
 * http://jr4pdp.blog.enjoy.jp/myblog/2016/01/lcd-ts174nblpic-da92.html
 * http://morecatlab.akiba.coocan.jp/lab/index.php/2015/10/segment-lcd/
*/
#include "HT1621.h"
#include "7seg_fonts.h"
#define BIASTEST
// PORT
#define DATA_PIN 5
#define WR_PIN 4
#define RD_PIN 3
#define CS_PIN 2
#define LED_A 7
#define LED_K 6
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

#define BACKLIGHT_ON    digitalWrite(LED_A, HIGH)
#define BACKLIGHT_OFF    digitalWrite(LED_A, LOW)
#define WAIT_TIME 1000
#define MEM_MAX 9
byte MEM[MEM_MAX] = {0,0,0,0,0,0,0,0,0};
char STR_BUF[MEM_MAX];
HT1621 ht1621(DATA_PIN, WR_PIN, RD_PIN, CS_PIN);
void setup()
{
  pinMode(LED_A, OUTPUT); //LED
  pinMode(LED_K, OUTPUT); //LED
  pinMode(8, INPUT_PULLUP); //IRQ?
  digitalWrite(LED_K, LOW);
  Serial.begin(9600);
  if (! ht1621.begin()) {
    Serial.println("Could not init device!\n");
  }
  // TS174 default is uninitialize.
  //ht1621.sendCommand(ht1621.BIAS_HALF_2_COM);
  //ht1621.sendCommand(ht1621.BIAS_HALF_3_COM);
  //ht1621.sendCommand(ht1621.BIAS_HALF_4_COM);
  //ht1621.sendCommand(ht1621.BIAS_THIRD_2_COM);
  //ht1621.sendCommand(ht1621.BIAS_THIRD_3_COM);  // TS119-3 default
  ht1621.sendCommand(ht1621.BIAS_THIRD_4_COM);  // TS119-5 default
}
byte bias;
void loop()
{
  BACKLIGHT_ON;
  write_char(0, '0');
  write_char(1, '1');
  write_char(2, '2');
  write_char(3, '3');
  write_char(4, '4');
  write_char(5, '5');
  write_char(6, '6');
  write_char(7, '7');
  write_mem();
  delay(WAIT_TIME);
  write_char(0, '8');
  write_char(1, '9');
  write_char(2, 'A');
  write_char(3, 'B');
  write_char(4, 'C');
  write_char(5, 'D');
  write_char(6, 'E');
  write_char(7, 'F');
  write_mem();
  delay(WAIT_TIME);
  write_char(0, 'G');
  write_char(1, 'H');
  write_char(2, 'I');
  write_char(3, 'J');
  write_char(4, 'K');
  write_char(5, 'L');
  write_char(6, 'M');
  write_char(7, 'N');
  write_mem();
  delay(WAIT_TIME);
  write_char(0, 'O');
  write_char(1, 'P');
  write_char(2, 'Q');
  write_char(3, 'R');
  write_char(4, 'S');
  write_char(5, 'T');
  write_char(6, 'U');
  write_char(7, 'V');
  write_mem();
  delay(WAIT_TIME);
  write_char(0, 'W');
  write_char(1, 'X');
  write_char(2, 'Y');
  write_char(3, 'Z');
  write_char(4, ' ');
  write_char(5, '-');
  write_char(6, '=');
  write_char(7, ' ');
  write_mem();
  delay(WAIT_TIME);
  write_char(0, '!');
  write_char(1, '"');
  write_char(2, '#');
  write_char(3, '$');
  write_char(4, '%');
  write_char(5, '&');
  write_char(6, '\'');
  write_char(7, '(');
  write_mem();
  delay(WAIT_TIME);
  write_str("0123:45.67");
  write_mem();
  delay(WAIT_TIME);
  write_str(" 999887.1");
  write_mem();
  delay(WAIT_TIME);
   icon(ICON_1, ICON_ON);
  icon(ICON_COLON, ICON_ON);
  icon(ICON_H, ICON_ON);
  icon(ICON_TIME, ICON_ON);
  icon(ICON_CAUTION, ICON_ON);
  icon(ICON_ASTERISK, ICON_ON);
  icon(ICON_POINTL, ICON_ON);
  icon(ICON_POINTR, ICON_ON);
  unit(UNIT_k|UNIT_A|UNIT_h|UNIT_MD);
  write_mem();
  delay(WAIT_TIME);
  BACKLIGHT_OFF;
  clear_mem(0x00);
  delay(WAIT_TIME);
  for (bias = 0 ; bias<0x10; bias++) {
    #ifdef BIASTEST
    ht1621.sendCommand(0b00100000|bias);
    #endif
    sprintf(STR_BUF, "8888%4d", bias);
    write_str(STR_BUF);
    write_mem();
    delay(WAIT_TIME);
  }
}
void icon(byte num, bool f)
{
  if (f)
  { 
    MEM[num] = MEM[num] | 0x01;  
  } else 
  {
    MEM[num] = MEM[num] & 0xFE;  
  }
}
void unit(byte c)
{
  MEM[8] = c;
}

void write_char(byte col, char c)
{
  byte num = c -0x20;
  MEM[col] = (MEM[col] & 0x01) | CHAR_DATA[num]; 
}
void write_mem()
{
  for (int i =0; i<9; i++) {
    ht1621.writeMem(i*2,  (MEM[i]>>4) & 0x0F);
    ht1621.writeMem(i*2+1, (MEM[i]) & 0x0F);
  }
}

void write_str(char *st)
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

void clear_mem(byte data)
{
  for (int i = 0; i<MEM_MAX;i++){
    MEM[i] = data;
  }
  write_mem();
}


