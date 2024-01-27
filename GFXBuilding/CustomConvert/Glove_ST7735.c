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
static int16_t width = ST7735_TFTWIDTH;   // this could probably be a constant, except it is used in Adafruit_GFX and depends on image rotation
static int16_t height = ST7735_TFTHEIGHT;
static uint8_t xOffset, yOffset = 0; // some displays need this changed


/**************************************************************************/
/*!
    @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano  
    @brief Combined list of GreenTab initilization commands for the ST7735R
    @remarks Reformatted for improved readability
    @since January 27, 2024
*/
/**************************************************************************/
initCmdList[] = {                 
    21,
  // 7735R init, part 1 (red or green tab)                        
  //  1: Software reset, 0 args, 150ms delay
  ST77XX_SWRESET, ST_CMD_DELAY, 150,                          
  //  2: Out of sleep mode, 0 args, 500ms delay
  ST77XX_SLPOUT, ST_CMD_DELAY, 255,                          
  //  3: Framerate ctrl - normal mode, 3 args: Rate = fosc/(1x2+40) * (LINE+2C+2D)
  ST7735_FRMCTR1, 3,  0x01, 0x2C, 0x2D,             
  //  4: Framerate ctrl - idle mode, 3 args: Rate = fosc/(1x2+40) * (LINE+2C+2D)
  ST7735_FRMCTR2, 3,  0x01, 0x2C, 0x2D,             
  //  5: Framerate - partial mode, 6 args: 3 for Dot inversion mode, 3 for Line inversion mode
  ST7735_FRMCTR3, 6,  0x01, 0x2C, 0x2D,  0x01, 0x2C, 0x2D,             
  //  6: Display inversion ctrl, 1 arg: No inversion
  ST7735_INVCTR, 1,  0x07,                         
  //  7: Power control, 3 args, no delay: 1st = ?, 2nd = -4.6V, 3rd = AUTO mode
  ST7735_PWCTR1, 3,  0xA2, 0x02, 0x84,                         
  //  8: Power control, 1 arg, no delay: VGH25=2.4C VGSEL=-10 VGH=3 * AVDD
  ST7735_PWCTR2, 1,  0xC5,                         
  //  9: Power control, 2 args, no delay: 1st = Opamp current small, 2nd = Boost frequency
  ST7735_PWCTR3, 2,  0x0A, 0x00,                         
  //  10: Power control, 2 args, no delay: 1st = BCLK/2, 2nd = opamp current small & medium low
  ST7735_PWCTR4, 2,  0x8A, 0x2A,                         
  //  11: Power control, 2 args, no delay: Unknown
  ST7735_PWCTR5, 2,  0x8A, 0xEE,
  //  12: Power control, 1 arg, no delay: Unknown
  ST7735_VMCTR1, 1,  0x0E,
  //  13: Don't invert display, no args
  ST77XX_INVOFF, 0,              
  //  14: Mem access ctl (directions), 1 arg: row/col addr, bottom-top refresh
  ST77XX_MADCTL, 1,  0xC8,                         
  //  15: set color mode, 1 arg, no delay: 16-bit color
  ST77XX_COLMOD, 1,  0x05,                         

  // 7735R init, part 2 (green tab only)
  //  1: Column addr set, 4 args, no delay: 2 for XSTART = 0, 2 for XEND = 127
  ST77XX_CASET, 4,  0x00, 0x02,  0x00, 0x7F+0x02,              
  //  2: Row addr set, 4 args, no delay: 2 for XSTART = 0, 2 for XEND = 159
  ST77XX_RASET, 4,  0x00, 0x01,  0x00, 0x9F+0x01,

  // 7735R init, part 3 (red or green tab)
  //  1: Gamma Adjustments (pos. polarity), 16 args + delay: (Not entirely necessary, but provides accurate colors)
  ST7735_GMCTRP1, 16,  0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10,
  //  2: Gamma Adjustments (neg. polarity), 16 args + delay: (Not entirely necessary, but provides accurate colors)
  ST7735_GMCTRN1, 16,  0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10,
  //  3: Normal display on, no args, 10ms delay
  ST77XX_NORON, ST_CMD_DELAY, 10,                           
  //  4: Main screen turn on, no args, 100 ms delay
  ST77XX_DISPON, ST_CMD_DELAY, 100                           
};                         


/* SSI info from Valvano
  The Data/Command pin must be valid when the eighth bit is
  sent.  The SSI module has hardware input and output FIFOs
  that are 8 locations deep.  Based on the observation that
  the LCD interface tends to send a few commands and then a
  lot of data, the FIFOs are not used when writing
  commands, and they are used when writing data.  This
  ensures that the Data/Command pin status matches the byte
  that is actually being transmitted.
*/

/**************************************************************************/
/*!
  @author Jonathan W. Valvano
  @brief  waits until all data has been sent, configures the 
          Data/Command pin for commands, sends the command, 
          and then waits for the transmission to finish.
  @note Function will crash or stall indefinitely if
        the SSI0 module is not initialized and enabled.
  @param  c  Data to be sent through SSI
  @remarks Changed function name to camel-case
  @since January 27, 2024
*/
/**************************************************************************/
void writeCommand(uint8_t c) {
  // Wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
  DC = DC_COMMAND;
  // Data out
  SSI0_DR_R = c;                        
  // Wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
};


/**************************************************************************/
/*!
  @author Jonathan W. Valvano
  @brief  The write data operation waits until there is room in the
          transmit FIFO, configures the Data/Command pin for data,
          and then adds the data to the transmit FIFO.
  @note Function will crash or stall indefinitely if
        the SSI0 module is not initialized and enabled.
  @param  c  Data to be sent through SSI
  @remarks Changed function name to camel-case
  @since January 27, 2024
*/
/**************************************************************************/
void writeData(uint8_t c) {
  // Wait until transmit FIFO not full
  while((SSI0_SR_R&SSI_SR_TNF)==0){};   
  DC = DC_DATA;
  // Data out
  SSI0_DR_R = c;                        
};


/**************************************************************************/
/*!
  @authors Qwyntyn Scurr, Limor Fried/Ladyada
  @brief  Set origin of (0,0) of display with offsets
  @param  col  The offset from 0 for the column address
  @param  row  The offset from 0 for the row address
  @remarks I believe this sets the offsets of the screen, leaving as col- and row-start
            for now but likely should be xOffset and yOffset
  @since January 27, 2024
*/
/**************************************************************************/
void setColRowStart(int8_t col, int8_t row) {
  ColStart = col;
  RowStart = row;
};


/**************************************************************************/
/*!
  @authors Qwyntyn Scurr, Limor Fried/Ladyada
  @brief  Change whether display is on or off
  @param  enable 1 if you want the display ON, 0 = OFF
  @remarks Modified to use int instead of bool
  @remarks changed sendcommand to writeCommand
  @since January 27, 2024
 */
/**************************************************************************/
void enableDisplay(int enable) {
  writeCommand(enable ? ST77XX_DISPON : ST77XX_DISPOFF);
};


/**************************************************************************/
/*!
Adafruit
  @authors Qwyntyn Scurr, Limor Fried/Ladyada
  @brief  Change whether TE pin output is on or off
  @param  enable 1 if you want the TE pin ON, 0 = OFF
  @remarks Modified to use int instead of bool
  @remarks changed sendcommand to writeCommand
  @since January 27, 2024
 */
/**************************************************************************/
void enableTearing(int enable) {
  writeCommand(enable ? ST77XX_TEON : ST77XX_TEOFF);
};


/**************************************************************************/
/*!
  @authors Qwyntyn Scurr, Limor Fried/Ladyada
  @brief  Change whether sleep mode is on or off
  @param  enable 1 if you want sleep mode ON, 0 = OFF
  @remarks Modified to use int instead of bool
  @remarks changed sendcommand to writeCommand
  @since January 27, 2024
 */
/**************************************************************************/
void enableSleep(int enable) {
  writeCommand(enable ? ST77XX_SLPIN : ST77XX_SLPOUT);
};


/**************************************************************************/
/*!
  @brief Sets an address window rectangle for pixel blitting
  @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
  @param x  Window X position
  @param y  Window Y position
  @param w  Window width
  @param h  Window height
  @remarks Modifications include:
  @remarks Using w and h convention for x2 and y2 of rect
  @remarks Using x/yOffset convention rather than col/rowstart or x/ystart
  @remarks Using Valavano 1-byte transfers rather than Ada 32-bit
  @remarks Changed writecommand and writeData to camel-case equivalents
  @since January 25, 2024
*/
/**************************************************************************/
void setAddrWindow(uint8_t x,uint8_t y,uint8_t w,uint8_t h){

  // Setting the columns to blit?
  writeCommand(ST77XX_CASET); // Column addr set
  writeData(0x00);            // Dummy? or just null, to space out data transfers, check Datasheet
  writeData(x + xOffset);     // XSTART
  writeData(0x00);            // Dummy? or just null, to space out data transfers, check Datasheet
  writeData(w + xOffset);     // XEND

  // Setting the rows to blit?
  writeCommand(ST77XX_RASET); // Row addr set
  writeData(0x00);            // Dummy? or just null, to space out data transfers, check Datasheet
  writeData(y + yOffset);     // YSTART
  writeData(0x00);            // Dummy? or just null, to space out data transfers, check Datasheet
  writeData(h + yOffset);     // YEND

  // Writing something to RAM, still not sure what or why
  writeCommand(ST77XX_RAMWR); // write to RAM
};


/**************************************************************************/
/*!
  @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
  @brief Sets the rotation of screen
  @param m 0 to 3, determines rotation of screen
  @since January 27, 2024
*/
/**************************************************************************/
void setRotation(uint8_t m){
  uint8_t madctl = 0;
  int rotation = m % 3; // can't be higher than 3

  switch (rotation) {
    case 0:
      madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7735_MADCTL_BGR;
      height = ST7735_TFTHEIGHT;
      width = ST7735_TFTWIDTH;
      xOffset = ColStart;
      yOffset = RowStart;
      break;
    case 1:
      madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
      width = ST7735_TFTHEIGHT;
      height = ST7735_TFTWIDTH;
      yOffset = ColStart;
      xOffset = RowStart;
      break;
    case 2:
      madctl = ST7735_MADCTL_BGR;
      height = ST7735_TFTHEIGHT;
      width = ST7735_TFTWIDTH;
      xOffset = ColStart;
      yOffset = RowStart;
      break;
    case 3:
      madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
      width = ST7735_TFTHEIGHT;
      height = ST7735_TFTWIDTH;
      yOffset = ColStart;
      xOffset = RowStart;
      break;
  };
  writeCommand(ST77XX_MADCTL);
  writeData(madctl);
};

/**************************************************************************/
/*!
  @authors Qwyntyn Scurr, Jonathan W. Valvano 
  @brief Initilization code for the ST7735R
  @param cmdlist  list of commands to set the display with
  @remarks Modifications include:
  @remarks shuffling Valvano's settings around to make more sense
  Must put in ifndefs for systick wait to use it?
  @since January 25, 2024
*/
/**************************************************************************/
void commonInit() {
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
}


/**************************************************************************/
/*!
  @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
  @brief  Companion code to the initiliazation tables. Reads and issues
          a series of LCD commands stored in PROGMEM byte array.
  @param  addr  Flash memory array with commands and data to send
  @remarks Used Valvanos version as a base since it follows TM4C SSI
  @remarks Changed delay function to Systick_Wait10ms() for cross-compatibility
  @since January 27, 2024
*/
/**************************************************************************/
void readCmdList(const uint8_t *addr) {
  uint8_t numCommands, numArgs;
  uint16_t ms;

  // Number of commands to follow
  numCommands = *(addr++);               
  // For each command...
  while(numCommands--) {                 
    // Read, issue command
    writeCommand(*(addr++));             
    // Number of args to follow
    numArgs = *(addr++);                
    // If hibit set, delay follows args
    ms = numArgs & ST_CMD_DELAY;          
    // Mask out delay bit
    numArgs &= ~ST_CMD_DELAY;                   
    // For each argument...
    while(numArgs--) {                   
      // Read, issue argument
      writeData(*(addr++));              
    };

    if(ms) {
      // Read post-command delay time (ms)
      ms = *(addr++);             
      // If 255, delay for 500 ms
      if(ms == 255) ms = 500;     
      SysTick_Wait10ms(ms/10);
    };
  };
};


/**************************************************************************/
/*!
  @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
  @brief  LCD screen Initilization function; calls commonInit to enable
          communication between TM4C and ST7735R via SSI
          Then calls readCmdList to pass initCmdList configuration
          settings to ST7735R
  @remarks Modified greatly to only require commonInit and readCmdList
  @remarks Since the cmdlist has been merged, only one call of readCmdList is necessary
  @since January 27, 2024
*/
/**************************************************************************/
void ST7735_initR() {
  commonInit();
  setRotation(0);
  readCmdList(initCmdList);
};
