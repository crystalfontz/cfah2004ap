//===========================================================================
//
//  written for Seeeduino v4.2 runnung at 3.3v
//
//  CRYSTALFONTZ CFAH2004AP 20x4 Character in SPI mode
//
//  ref: https://www.crystalfontz.com/product/CFAH2004AP
//
//  2017-09-26 Max / Crystalfontz
//===========================================================================
//  This is free and unencumbered software released into the public domain.
//
//  AnyLINE_ONE is free to copy, modify, publish, use, compile, sell, or
//  distribute this software, either in source form or as a compiled
//  binary, for any purpose, commercial or non-commercial, and by any
//  means.
//
//  In jurisdictions that recognize copyright laws, the author or authors
//  of this software dedicate any and all copyright interest in the
//  software to the public domain. We make this dedication for the benefit
//  of the public at large and to the detriment of our heirs and
//  successors. We intend this dedication to be an overt act of
//  relinquishment in perpetuity of all present and future rights to this
//  software under copyright law.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
//
//  For more information, please refer to <http://unlicense.org/>
//===========================================================================
#include <SPI.h>
//----------------------------------------------------------------------------
#define Cword 0x14  //20
#define LINE_ONE    0x80  // DD RAM Address The starting position of the first line is 0x00
// set the DD RAM address to 0x80 + 0x00 = 0x80
#define LINE_TWO    0xc0  // DD RAM Address The starting position of the second line is 0x40
// set the DD RAM address to 0x80 + 0x40 = 0xc0
#define LINE_THREE  0x94  // DD RAM Address The starting position of the second line is 0x14
// set the DD RAM address to 0x80 + 0x14 = 0x94
#define LINE_FOUR   0xD4  // DD RAM Address The starting position of the second line is 0x54
// set the DD RAM address to 0x80 + 0x54 = 0xD4

//============================================================================
// LCD SPI & control lines
//   ARD      | Port | LCD
// -----------+------+-------------------------
//  #7/D7     |  PD7 | SD_CS
//  #8/D8     |  PB0 | LCD_RS
//  #9/D9     |  PB1 | LCD_RESET
// #10/D10    |  PB2 | LCD_CS_NOT (or SPI SS)
// #11/D11    |  PB3 | LCD_MOSI   (hardware SPI)
// #12/D12    |  PB4 | not used   (would be MISO)
// #13/D13    |  PB5 | LCD_SCK    (hardware SPI)

#define CLR_RS    (PORTB &= ~(0x01))
#define SET_RS    (PORTB |=  (0x01))
#define CLR_CS    (PORTB &= ~(0x04))
#define SET_CS    (PORTB |=  (0x04))
#define CLR_MOSI  (PORTB &= ~(0x08))
#define SET_MOSI  (PORTB |=  (0x08))
#define CLR_SCK   (PORTB &= ~(0x20))
#define SET_SCK   (PORTB |=  (0x20))

//                //00000000001111111111
// 								//01234567890123456789
uint8_t MSG1[20]  ="**  CRYSTALFONTZ  **";
uint8_t MSG2[20]  ="** CFAH2004AP-TMI **";
uint8_t MSG3[20]  ="**  20 Characters **";
uint8_t MSG4[20]  ="**   by 4 lines   **";
uint8_t CGRAM1[8] ={0x04,0x0E,0x15,0x04,0x04,0x04,0x04,0x04,};
uint8_t CGRAM2[8] ={0x04,0x04,0x04,0x04,0x04,0x15,0x0e,0x04,};
//============================================================================
void writeCommand(uint8_t command);
void writeData(uint8_t data);
void writeString(uint8_t count, uint8_t *MSG);
void Initialize_CFAH2004AP(void);
//============================================================================
void writeCommand(uint8_t command)
{
  // Select the LCD's command register
  CLR_RS;
  // Select the LCD controller
  CLR_CS;
  //Send the command via SPI:
  SPI.transfer(command);
  // Deselect the LCD controller
  SET_CS;
  delay(1);
}
//============================================================================
void writeData(uint8_t data)
{
  // Select the LCD's data register
  SET_RS;
  // Select the LCD controller
  CLR_CS;
  //Send the data via SPI:
  SPI.transfer(data);
  // Deselect the LCD controller
  SET_CS;
  delay(1);
}
//============================================================================
void writeString(uint8_t count,uint8_t *MSG)
{
  for(uint8_t i = 0; i<count;i++)
  {
    writeData(MSG[i]);
  }
}
//============================================================================
void Initialize_CFAH2004AP()
{
  //SPI Init
  delay(50);
  writeCommand(0x38); // Function set
  delay(1);
  writeCommand(0x0C); // Display ON/OFF
  delay(1);
  writeCommand(0x01); // Clear display
  delay(10);
  writeCommand(0x06); // Entry mode set
  delay(1);
}
//============================================================================
void setup( void )
{
  // LCD SPI & control lines
  //   ARD      | Port | LCD
  // -----------+------+-------------------------
  //  #7/D7     |  PD7 | SD_CS
  //  #8/D8     |  PB0 | LCD_RS
  //  #9/D9     |  PB1 | LCD_RESET
  // #10/D10    |  PB2 | LCD_CS_NOT (or SPI SS)
  // #11/D11    |  PB3 | LCD_MOSI   (hardware SPI)
  // #12/D12    |  PB4 | not used   (would be MISO)
  // #13/D13    |  PB5 | LCD_SCK    (hardware SPI)
  // #23/D14/A0 |  PC0 | Touch XL   (only used on TS modules)
  // #24/D15/A1 |  PC1 | Touch XR   (only used on TS modules)
  // #25/D16/A2 |  PC2 | Touch YD   (only used on TS modules)
  // #26/D17/A3 |  PC3 | Touch YU   (only used on TS modules)
  DDRB |= 0x2F;

  //Drive the ports to a reasonable starting state.
  CLR_RS;
  SET_CS;
  CLR_MOSI;
  CLR_SCK;

  //debug console
  Serial.begin(115200);
  Serial.println(F("setup()"));

  // Initialize SPI. By default the clock is 4MHz.
  SPI.begin();
  //Bump the clock to 8MHz. Appears to be the maximum.
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  //Fire up the SPI OLED
  Serial.println(F("Initialize_CFAH2004AP()"));
  Initialize_CFAH2004AP();

  //From the init
  writeCommand(LINE_ONE);
	writeString(Cword,MSG1);
	writeCommand(LINE_TWO);
	writeString(Cword,MSG2);
	writeCommand(LINE_THREE);
	writeString(Cword,MSG3);
	writeCommand(LINE_FOUR);
	writeString(Cword,MSG4);
}
//============================================================================
uint8_t currentLine[20] = "** CFAH2004AP-TMI **";
uint8_t iteration = 0;
void loop(void)
{
  uint8_t temp;
  
  temp = currentLine[0];
  for(uint8_t thisChar = 1; thisChar < 20; thisChar++)
  {
    currentLine[thisChar - 1] = currentLine[thisChar];
  }
  currentLine[19] = temp;
	writeCommand(LINE_TWO);
  writeString(Cword,currentLine);
  iteration++;
  delay(250);

  if(iteration == 20)
  {
    delay(2000);
    iteration = 0;
  }
}
//============================================================================
