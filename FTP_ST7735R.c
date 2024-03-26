/*!*******************************************************************
    @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
    @brief Initializes, configures and communicates with the ST7735R
    @note The purpose of this file is to separate SSI command functions from graphical
    manipulation functions. Thus, the FTP_GFX file calls neither writeData or
    writeCommand. Similarly, this file may only call writeData, writeCommand
    or another function defined in this program.
    @note ST7735R Breakout Pin Connections:
        Backlight (pin 10) connected to +3.3 V
        MISO (pin 9) unconnected
        SCK (pin 8) connected to PA2 (SSI0Clk)
        MOSI (pin 7) connected to PA5 (SSI0Tx)
        TFT_CS (pin 6) connected to PA3 (SSI0Fss)
        CARD_CS (pin 5) unconnected
        Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
        RESET (pin 3) connected to PA7 (GPIO)
        VCC (pin 2) connected to +3.3 V
        Gnd (pin 1) connected to ground
    @since March 14, 2024
    @version Rev 3
**********************************************************************/

// Includes
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "FTP_ST7735R.h"

// Volatile Definitions
volatile uint8_t ScreenOrientation = 0;
volatile uint8_t ScreenX = 0;
volatile uint8_t ScreenY = 0;
volatile uint8_t ScreenW = ST7735_TFTWIDTH;
volatile uint8_t ScreenH = ST7735_TFTHEIGHT;

/**************************************************************************/
/*!
    @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano  
    @brief Combined list of GreenTab initilization commands for the ST7735R
    @remarks Reformatted for improved readability
    @since January 27, 2024
*/
/**************************************************************************/
uint8_t initCmdList[] = {                 
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
  ST77XX_MADCTL, 1,  0x08,                         
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

/*!*******************************************************************
  @author Jonathan W. Valvano
  @brief  waits until all data has been sent, configures the 
          Data/Command pin for commands, sends the command, 
          and then waits for the transmission to finish.
  @note Function will crash or stall indefinitely if
        the SSI0 module is not initialized and enabled.
  @param  c  Data to be sent through SSI
  @remarks Changed function name to camel-case
  @since January 27, 2024
**********************************************************************/
void writeCommand(uint8_t c) {
  // Wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
  DC = DC_COMMAND;
  // Data out
  SSI0_DR_R = c;                        
  // Wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
};

/*!*******************************************************************
  @author Jonathan W. Valvano
  @brief  The write data operation waits until there is room in the
          transmit FIFO, configures the Data/Command pin for data,
          and then adds the data to the transmit FIFO.
  @note Function will crash or stall indefinitely if
        the SSI0 module is not initialized and enabled.
  @param  c  Data to be sent through SSI
  @remarks Changed function name to camel-case
  @since January 27, 2024
**********************************************************************/
void writeData(uint8_t c) {
  // Wait until transmit FIFO not full
  while((SSI0_SR_R&SSI_SR_TNF)==0){};   
  DC = DC_DATA;
  // Data out
  SSI0_DR_R = c;                        
};

/*!*******************************************************************
  @authors Qwyntyn Scurr, Limor Fried/Ladyada
  @brief Set software origin offset of LCD Screen
  @param col The offset from 0 for the column address
  @param row The offset from 0 for the row address
  @since January 27, 2024
**********************************************************************/
void setColRowStart(int8_t col, int8_t row) {
  ScreenX = col;
  ScreenY = row;
};

/*!*******************************************************************
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
  @remarks Changed writecommand and writedata to camel-case equivalents
  @since January 25, 2024
**********************************************************************/
void setAddrWindow(uint8_t x,uint8_t y,uint8_t w,uint8_t h){

  writeCommand(ST77XX_CASET); // Column addr set
  writeData(0x00);            
  writeData(x + ScreenX);     // XSTART
  writeData(0x00);            
  writeData(w + ScreenX);     // XEND

  writeCommand(ST77XX_RASET); // Row addr set
  writeData(0x00);            
  writeData(y + ScreenY);     // YSTART
  writeData(0x00);
  writeData(h + ScreenY);     // YEND

  writeCommand(ST77XX_RAMWR); // write to RAM
};

/*!*******************************************************************
  @authors Qwyntyn Scurr, Jonathan W. Valvano 
  @brief Writes the supplied 16-bit color to the ST7735R in two transmissions
  @param c 16-bit color in 5-6-5 format
  @remarks Modified writeData call to fit with Glove_ST7735
  @since February 8, 2024
**********************************************************************/
void pushColor(uint16_t c) {
  writeData((uint8_t)(c >> 8));
  writeData((uint8_t)c);
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Sets the display direction of the LCD screen, as per Sitronix Datsheet
  @details 0: Normal Mode
            1: Y-Mirror
            2: X-Mirror
            3: X-Mirror | Y-Mirror
            4: X-Y Exchange
            5: X-Y Exchange | Y-Mirror
            6: X-Y Exchange | X-Mirror
            7: X-Y Exchange | X-Mirror | Y-Mirror
  @note Derived from the setRotation function devloped by Adafruit/Valvano
        However, this function provides full display direction functionality 
  @param m 0 to 7, determines display direction of screen
  @since Feb 24, 2024
**********************************************************************/
void setDirection(uint8_t m){
  uint8_t direction = m % 7; // Limits input to 0-7
  switch (direction) {
    case 0:
    case 2:
    case 4:
    case 6:
      ScreenW = ST7735_TFTWIDTH;
      ScreenH = ST7735_TFTHEIGHT;
      break;
    case 1:
    case 3:
    case 5:
    case 7:
      ScreenW = ST7735_TFTHEIGHT;
      ScreenH = ST7735_TFTWIDTH;
      break;
  }
  // Set last 3 bits of register with direction select
  direction <<= 5;
  direction |= ST7735_MADCTL_BGR;
  writeCommand(ST77XX_MADCTL);
  writeData(direction);
};

/*!*******************************************************************
  @authors Qwyntyn Scurr, Jonathan W. Valvano 
  @brief Initilization code for the ST7735R
  @remarks Modifications include:
  @remarks shuffling Valvano's settings around to make more sense
  @since January 25, 2024
**********************************************************************/
void commonInit(void) {
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
  GPIO_PORTA_AMSEL_R &= ~0xC8;

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
  SSI0_CR0_R &= ~(SSI_CR0_SCR_M | SSI_CR0_SPH | SSI_CR0_SPO);

  // FULL CR0 = 0x0000FFC0
  // FRF = Freescale format
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;

  // DSS = 8-bit data
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_8;

  // enable SSI
  SSI0_CR1_R |= SSI_CR1_SSE;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
  @brief  Companion code to the initiliazation tables. Reads and issues
          a series of LCD commands stored in PROGMEM byte array.
  @param  addr  Flash memory array with commands and data to send
  @remarks Used Valvanos version as a base since it follows TM4C SSI
  @remarks Changed delay function to Systick_Wait10ms() for cross-compatibility
  @since January 27, 2024
**********************************************************************/
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

/*!*******************************************************************
  @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
  @brief  LCD screen Initilization function; calls commonInit to enable
          communication between TM4C and ST7735R via SSI
          Then calls readCmdList to pass initCmdList configuration
          settings to ST7735R
  @remarks Modified greatly to only require commonInit and readCmdList
  @remarks Since the cmdlist has been merged, only one call of readCmdList is necessary
  @since January 27, 2024
**********************************************************************/
void ST7735_initR(void) {
  commonInit();
  readCmdList(initCmdList);
};
