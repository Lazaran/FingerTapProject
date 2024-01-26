#include <stdio.h>
#include <stdint.h>
#include "Glove_ST7735.h"
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "PLL.h"

// Valvano statics, can be changed in the future
static uint8_t ColStart, RowStart; // some displays need this changed
static uint8_t Rotation;           // 0 to 3
static enum initRFlags TabColor;
static int16_t _width = ST7735_TFTWIDTH;   // this could probably be a constant, except it is used in Adafruit_GFX and depends on image rotation
static int16_t _height = ST7735_TFTHEIGHT;

static uint8_t xOffset, yOffset = 0; // some displays need this changed


// Adafruit ST7735R Initilization array, used in conjunction with Valvano
Rcmd1[] = {                       // 7735R init, part 1 (red or green tab)
    15,                             // 15 commands in list:
    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, 0 args, w/delay
      150,                          //     150 ms delay
    ST77XX_SLPOUT,    ST_CMD_DELAY, //  2: Out of sleep mode, 0 args, w/delay
      255,                          //     500 ms delay
    ST7735_FRMCTR1, 3,              //  3: Framerate ctrl - normal mode, 3 arg:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3,              //  4: Framerate ctrl - idle mode, 3 args:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6,              //  5: Framerate - partial mode, 6 args:
      0x01, 0x2C, 0x2D,             //     Dot inversion mode
      0x01, 0x2C, 0x2D,             //     Line inversion mode
    ST7735_INVCTR,  1,              //  6: Display inversion ctrl, 1 arg:
      0x07,                         //     No inversion
    ST7735_PWCTR1,  3,              //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                         //     -4.6V
      0x84,                         //     AUTO mode
    ST7735_PWCTR2,  1,              //  8: Power control, 1 arg, no delay:
      0xC5,                         //     VGH25=2.4C VGSEL=-10 VGH=3 * AVDD
    ST7735_PWCTR3,  2,              //  9: Power control, 2 args, no delay:
      0x0A,                         //     Opamp current small
      0x00,                         //     Boost frequency
    ST7735_PWCTR4,  2,              // 10: Power control, 2 args, no delay:
      0x8A,                         //     BCLK/2,
      0x2A,                         //     opamp current small & medium low
    ST7735_PWCTR5,  2,              // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1,  1,              // 12: Power control, 1 arg, no delay:
      0x0E,
    ST77XX_INVOFF,  0,              // 13: Don't invert display, no args
    ST77XX_MADCTL,  1,              // 14: Mem access ctl (directions), 1 arg:
      0xC8,                         //     row/col addr, bottom-top refresh
    ST77XX_COLMOD,  1,              // 15: set color mode, 1 arg, no delay:
      0x05 };                       //     16-bit color

Rcmd2green[] = {                  // 7735R init, part 2 (green tab only)
    2,                              //  2 commands in list:
    ST77XX_CASET,   4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x02,                   //     XSTART = 0
      0x00, 0x7F+0x02,              //     XEND = 127
    ST77XX_RASET,   4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x01,                   //     XSTART = 0
      0x00, 0x9F+0x01 };            //     XEND = 159

Rcmd3[] = {                         // 7735R init, part 3 (red or green tab)
    4,                              //  4 commands in list:
    ST7735_GMCTRP1, 16      ,       //  1: Gamma Adjustments (pos. polarity), 16 args + delay:
      0x02, 0x1c, 0x07, 0x12,       //     (Not entirely necessary, but provides
      0x37, 0x32, 0x29, 0x2d,       //      accurate colors)
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      ,       //  2: Gamma Adjustments (neg. polarity), 16 args + delay:
      0x03, 0x1d, 0x07, 0x06,       //     (Not entirely necessary, but provides
      0x2E, 0x2C, 0x29, 0x2D,       //      accurate colors)
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST77XX_NORON,     ST_CMD_DELAY, //  3: Normal display on, no args, w/delay
      10,                           //     10 ms delay
    ST77XX_DISPON,    ST_CMD_DELAY, //  4: Main screen turn on, no args w/delay
      100 };                         //     100 ms delay

// Valvano
// Modified name to camel-case

// The Data/Command pin must be valid when the eighth bit is
// sent.  The SSI module has hardware input and output FIFOs
// that are 8 locations deep.  Based on the observation that
// the LCD interface tends to send a few commands and then a
// lot of data, the FIFOs are not used when writing
// commands, and they are used when writing data.  This
// ensures that the Data/Command pin status matches the byte
// that is actually being transmitted.
// The write command operation waits until all data has been
// sent, configures the Data/Command pin for commands, sends
// the command, and then waits for the transmission to
// finish.
// The write data operation waits until there is room in the
// transmit FIFO, configures the Data/Command pin for data,
// and then adds the data to the transmit FIFO.
// NOTE: These functions will crash or stall indefinitely if
// the SSI0 module is not initialized and enabled.
void static writeCommand(uint8_t c) {
                                        // wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
  DC = DC_COMMAND;
  SSI0_DR_R = c;                        // data out
                                        // wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
}

// Valvano
// Modified name to camel-case
void static writeData(uint8_t c) {
  while((SSI0_SR_R&SSI_SR_TNF)==0){};   // wait until transmit FIFO not full
  DC = DC_DATA;
  SSI0_DR_R = c;                        // data out
}

// Use Adafruit ST7735R init array and set screen rotation
void ST7735_initR(uint8_t options) {
  commonInit(Rcmd1);
  displayInit(Rcmd2green);
  displayInit(Rcmd3);
  setRotation(0);
}





/**************************************************************************/
/*!
Adafruit
    @brief  Set origin of (0,0) of display with offsets
    @param  col  The offset from 0 for the column address
    @param  row  The offset from 0 for the row address

    I believe this sets the offsets of the screen, leaving as col- and row-start
    for now but likely should be xOffset and yOffset
*/
/**************************************************************************/
void setColRowStart(int8_t col, int8_t row) {
  _colstart = col;
  _rowstart = row;
}

/**************************************************************************/
/*!
Adafruit
 @brief  Change whether display is on or off
 @param  enable 1 if you want the display ON, 0 = OFF

  Modified to use int instead of bool
 */
/**************************************************************************/
void enableDisplay(int enable) {
  sendcommand(enable ? ST77XX_DISPON : ST77XX_DISPOFF);
}

/**************************************************************************/
/*!
Adafruit
 @brief  Change whether TE pin output is on or off
 @param  enable 1 if you want the TE pin ON, 0 = OFF
   Modified to use int instead of bool

 */
/**************************************************************************/
void enableTearing(int enable) {
  sendCommand(enable ? ST77XX_TEON : ST77XX_TEOFF);
}

/**************************************************************************/
/*!
Adafruit
 @brief  Change whether sleep mode is on or off
 @param  enable 1 if you want sleep mode ON, 0 = OFF
   Modified to use int instead of bool
 */
/**************************************************************************/
void enableSleep(int enable) {
  sendCommand(enable ? ST77XX_SLPIN : ST77XX_SLPOUT);
}


/**************************************************************************/
/*!
  @brief Sets an address window rectangle for pixel blitting
  @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
  @param x  Window X position
  @param y  Window Y position
  @param w  Window width
  @param h  Window height
  @since January 25, 2024
  @remarks Modifications include:
  @remarks Using w and h convention for x2 and y2 of rect
  @remarks Using x/yOffset convention rather than col/rowstart or x/ystart
  @remarks Using Valavano 1-byte transfers rather than Ada 32-bit

*/
/**************************************************************************/
void static setAddrWindow(uint8_t x,uint8_t y,uint8_t w,uint8_t h){

  // Setting the columns to blit?
  writecommand(ST77XX_CASET); // Column addr set
  writedata(0x00);            // Dummy? or just null, to space out data transfers, check Datasheet
  writedata(x + xOffset);     // XSTART
  writedata(0x00);            // Dummy? or just null, to space out data transfers, check Datasheet
  writedata(w + xOffset);     // XEND

  // Setting the rows to blit?
  writecommand(ST77XX_RASET); // Row addr set
  writedata(0x00);            // Dummy? or just null, to space out data transfers, check Datasheet
  writedata(y + yOffset);     // YSTART
  writedata(0x00);            // Dummy? or just null, to space out data transfers, check Datasheet
  writedata(h + yOffset);     // YEND

  // Writing something to RAM, still not sure what or why
  writecommand(ST77XX_RAMWR); // write to RAM
};

// Set the rotation of the screen Valvano
/**************************************************************************/
/*!
  @brief Sets the rotation of screen
  @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
  @param x  Window X position
  @param y  Window Y position
  @param w  Window width
  @param h  Window height
  @since January 25, 2024
  @remarks Modifications include:
  @remarks Using w and h convention for x2 and y2 of rect
  @remarks Using x/yOffset convention rather than col/rowstart or x/ystart
  @remarks Using Valavano 1-byte transfers rather than Ada 32-bit

*/
/**************************************************************************/
void ST7735_setRotation(uint8_t m) {
  uint8_t madctl = 0;
  int rotation = m % 3; // can't be higher than 3

  switch (rotation) {
    case 0:
      madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7735_MADCTL_BGR;
      _height = ST7735_TFTHEIGHT;
      _width = ST7735_TFTWIDTH;
      // _xstart = _colstart;
      // _ystart = _rowstart;
      break;

    case 1:
      madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
      _width = ST7735_TFTHEIGHT;
      _height = ST7735_TFTWIDTH;
      // _ystart = _colstart;
      // _xstart = _rowstart;
      break;

    case 2:
      madctl = ST7735_MADCTL_BGR;
      _height = ST7735_TFTHEIGHT;
      _width = ST7735_TFTWIDTH;
      // _xstart = _colstart;
      // _ystart = _rowstart;
      break;

    case 3:
      madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
      _width = ST7735_TFTHEIGHT;
      _height = ST7735_TFTWIDTH;
      // _ystart = _colstart;
      // _xstart = _rowstart;
      break;
  };

  writecommand(ST77XX_MADCTL);
  writedata(madctl);
};




/**************************************************************************/
/*!
  @brief Initilization code for the ST7735R
  @authors Qwyntyn Scurr, Jonathan W. Valvano 
  @param cmdlist  list of commands to set the display with
  @since January 25, 2024
  @remarks Modifications include:
  @remarks shuffling Valvano's settings around to make more sense
  Must put in ifndefs for systick wait to use it?
*/
/**************************************************************************/
void static commonInit(const uint8_t *cmdList) {
  ColStart = RowStart = 0; // May be overridden in init func

  SYSCTL_RCGCGPIO_R |= 0x01; // activate port A
  SYSCTL_RCGCSSI_R |= 0x01;  // activate SSI0

  // SSI0Fss is temporarily used as GPIO
  //  configure PA3,6,7 as GPIO
  //  make PA3,6,7 out
  GPIO_PORTA_DIR_R |= 0xC8;
  //  disable alt funct on PA3,6,7
  GPIO_PORTA_AFSEL_R &= ~0xC8;
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0x00FF0FFF)+0x00000000;
  //  enable digital I/O on PA3,6,7
  GPIO_PORTA_DEN_R |= 0xC8;
  //  disable analog functionality on PA3,6,7
  //  GPIO_PORTA_AMSEL_R &= ~0xC8;


  // toggle RST low to reset; CS low so it'll listen to us
  TFT_CS = TFT_CS_LOW;
  RESET = RESET_HIGH;
  SysTick_Wait10ms(50);
  RESET = RESET_LOW;
  SysTick_Wait10ms(50);
  RESET = RESET_HIGH;
  SysTick_Wait10ms(50);


  // Initialize SSI0
  //  Enable alt funct on PA2,3,5
  GPIO_PORTA_AFSEL_R |= 0x2C;

  //  Enable digital I/O on PA2,3,5
  GPIO_PORTA_DEN_R |= 0x2C;

  //  Configure PA2,3,5 as SSI
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF0F00FF)+0x00202200;

  //  Disable analog functionality on PA2,3,5
  GPIO_PORTA_AMSEL_R &= ~0x2C;

  //  Disable SSI
  SSI0_CR1_R &= ~SSI_CR1_SSE;

  //  Master mode
  SSI0_CR1_R &= ~SSI_CR1_MS;

  //  Configure for system clock/PLL baud clock source
  SSI0_CC_R = (SSI0_CC_R&~SSI_CC_CS_M)+SSI_CC_CS_SYSPLL;


  // Clock divider for 8 MHz SSIClk (80 MHz PLL/24)
  // SysClk/(CPSDVSR*(1+SCR))
  // 80/(10*(1+0)) = 8 MHz (slower than 4 MHz)
  SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+10; // must be even number
  // SCR = 0 (8 Mbps data rate), SPH = 0, SPO = 0
  SSI0_CR0_R = (SSI0_CR0_R&~(SSI_CR0_SCR_M | SSI_CR0_SPH | SSI_CR0_SPO | SSI_CR0_FRF_M | SSI_CR0_DSS_M)) + (SSI_CR0_FRF_MOTO |);

  // FULL CR0 = 0x0000FFC0
  // FRF = Freescale format
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;

  // DSS = 8-bit data
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_8;

  // enable SSI
  SSI0_CR1_R |= SSI_CR1_SSE;
  
  if(cmdList) commandList(cmdList);
}
