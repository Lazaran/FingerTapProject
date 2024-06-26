/*!*******************************************************************
    @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
    @brief Corresponding H file for FTP_ST7735R.c, holds macros and function
            prototypes
    @since March 14, 2024
    @version Rev 3
**********************************************************************/

/* File FTP_ST7735R.h*/
#ifndef FTP_ST7735R_H
#define FTP_ST7735R_H

// Includes
#include <stdint.h>
#include <stdlib.h>

// Macros
#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

// Chip enable
#define TFT_CS                  (*((volatile uint32_t *)0x40004020))
#define TFT_CS_LOW              0           // CS normally controlled by hardware
#define TFT_CS_HIGH             0x08
// Data/Command Flag Register
#define DC                      (*((volatile uint32_t *)0x40004100))
// Command Flag Set
#define DC_COMMAND              0
// Data Flag Set
#define DC_DATA                 0x40
#define RESET                   (*((volatile uint32_t *)0x40004200))
#define RESET_LOW               0
#define RESET_HIGH              0x80

// --| Valvano SSI Settings |--

// SSI Serial Clock Rate
#define SSI_CR0_SCR_M           0x0000FF00  
// SSI Serial Clock Phase
#define SSI_CR0_SPH             0x00000080  
// SSI Serial Clock Polarity
#define SSI_CR0_SPO             0x00000040  
// SSI Frame Format Select
#define SSI_CR0_FRF_M           0x00000030  
// Freescale SPI Frame Format
#define SSI_CR0_FRF_MOTO        0x00000000  
// SSI Data Size Select
#define SSI_CR0_DSS_M           0x0000000F  
// 8-bit data
#define SSI_CR0_DSS_8           0x00000007  
// SSI Master/Slave Select
#define SSI_CR1_MS              0x00000004  
// SSI Synchronous Serial Port Enable
#define SSI_CR1_SSE             0x00000002 
// SSI Busy Bit
#define SSI_SR_BSY              0x00000010  
// SSI Transmit FIFO Not Full
#define SSI_SR_TNF              0x00000002  
// SSI Clock Prescale Divisor
#define SSI_CPSR_CPSDVSR_M      0x000000FF  
// SSI Baud Clock Source
#define SSI_CC_CS_M             0x0000000F  
// Either the system clock (if the PLL bypass is in effect) or the PLL output (default)
#define SSI_CC_CS_SYSPLL        0x00000000                        
// SSI0 Clock Gating Control
#define SYSCTL_RCGC1_SSI0       0x00000010  
// port A Clock Gating Control
#define SYSCTL_RCGC2_GPIOA      0x00000001

// --| Adafruit ST7735 Register Settings |--

// Special signifier for command lists
#define ST_CMD_DELAY 0x80 
// Memory Data Access Control | RGB-BGR ORDER | 0 = RGB, 1 = BGR
#define ST7735_MADCTL_BGR 0x08    
// Memory Data Access Control | Horizontal Refresh Order | 0 = Left-Right, 1 = Right-Left
#define ST7735_MADCTL_MH 0x04     
// Frame Rate Control | Normal mode / Full colors
#define ST7735_FRMCTR1 0xB1 
// Frame Rate Control | Idle mode / 8 colors
#define ST7735_FRMCTR2 0xB2 
// Frame Rate Control | Partial mode / Full colors
#define ST7735_FRMCTR3 0xB3 
// Display Inversion Control
#define ST7735_INVCTR 0xB4  
//
#define ST7735_DISSET5 0xB6 
// Power control 1
#define ST7735_PWCTR1 0xC0  
// Power control 2
#define ST7735_PWCTR2 0xC1  
// Power control 3
#define ST7735_PWCTR3 0xC2  
// Power control 4
#define ST7735_PWCTR4 0xC3  
// Power control 5
#define ST7735_PWCTR5 0xC4  
// VCOM control 1
#define ST7735_VMCTR1 0xC5  
// Power control 6
#define ST7735_PWCTR6 0xFC  
// Gamma + Polarity Correction Characteristics control
#define ST7735_GMCTRP1 0xE0 
// Gamma - Polarity Correction Characteristics control
#define ST7735_GMCTRN1 0xE1 

// --| Adafruit ST77XX Register Settings |--

// No Operation
#define ST77XX_NOP 0x00 
// Software Reset | Also maybe Hardware Reset and Power On sequence
#define ST77XX_SWRESET 0x01 
// Read Display ID
#define ST77XX_RDDID 0x04 
// Read Display Status
#define ST77XX_RDDST 0x09 
// Sleep in and Booster Off
#define ST77XX_SLPIN 0x10 
// Sleep out and Booster On
#define ST77XX_SLPOUT 0x11  
// Partial mode on
#define ST77XX_PTLON 0x12 
// Partial off (Normal)
#define ST77XX_NORON 0x13 
// Display Inversion Off
#define ST77XX_INVOFF 0x20  
// Display Inversion On
#define ST77XX_INVON 0x21 
// Display Off
#define ST77XX_DISPOFF 0x28 
// Display On
#define ST77XX_DISPON 0x29  
// Column Address Set
#define ST77XX_CASET 0x2A 
// Row Address Set
#define ST77XX_RASET 0x2B 
// Memory Write
#define ST77XX_RAMWR 0x2C 
// Memory Read
#define ST77XX_RAMRD 0x2E 
// Partial Area
#define ST77XX_PTLAR 0x30 
// Tearing Effect Line Off
#define ST77XX_TEOFF 0x34 
// Tearing Effect Mode Set and On
#define ST77XX_TEON 0x35  
// Memory Data Access Control
#define ST77XX_MADCTL 0x36  
// Interface Pixel Format | Interface Format
#define ST77XX_COLMOD 0x3A  
// Row Address Order
#define ST77XX_MADCTL_MY 0x80 
// Column Address Order
#define ST77XX_MADCTL_MX 0x40 
// Row/Column  Order
#define ST77XX_MADCTL_MV 0x20 
// Scan Direction Parameter 
#define ST77XX_MADCTL_ML 0x10 
// RGB/BGR Order
#define ST77XX_MADCTL_RGB 0x00  
// Read ID1 Value
#define ST77XX_RDID1 0xDA 
// Read ID2 Value
#define ST77XX_RDID2 0xDB 
// Read ID3 Value
#define ST77XX_RDID3 0xDC 
// Read ID4 Value
#define ST77XX_RDID4 0xDD 

// Scurr custom set of SSI etc Settings

// Defined 4-byte 0-string, for posterity mostly
#define HEX_NULL 0x00000000 
// SSI Master/Slave Select
#define SSI_CR1_MS              0x00000004  
// SSI Synchronous Serial Port Enable
#define SSI_CR1_SSE             0x00000002 
// SSI Busy Bit
#define SSI_SR_BSY              0x00000010  
// SSI Transmit FIFO Not Full
#define SSI_SR_TNF              0x00000002  
// SSI Clock Prescale Divisor
#define SSI_CPSR_CPSDVSR_M      0x000000FF  
// SSI Baud Clock Source
#define SSI_CC_CS_M             0x0000000F  
// Either the system clock (if the PLL bypass is in effect) or the PLL output (default)
#define SSI_CC_CS_SYSPLL        0x00000000                        
// SSI0 Clock Gating Control
#define SYSCTL_RCGC1_SSI0       0x00000010  
// port A Clock Gating Control
#define SYSCTL_RCGC2_GPIOA      0x00000001

// Volatile Declarations
volatile extern uint8_t ScreenOrientation;
volatile extern uint8_t ScreenX;
volatile extern uint8_t ScreenY;
volatile extern uint8_t ScreenW;
volatile extern uint8_t ScreenH;

// Prototypes
void pushColor(uint16_t c);

void setDirection(uint8_t m);

void writeCommand(uint8_t c);

void writeData(uint8_t c);

void setColRowStart(int8_t col, int8_t row);

void setAddrWindow(uint8_t x,uint8_t y,uint8_t w,uint8_t h);

void commonInit(void);

void readCmdList(const uint8_t *addr);

void ST7735_initR(void);

#endif /* File FTP_ST7735R.h*/

