#include "tm4c123gh6pm.h"
#include "stdint.h"

/* ADAFRUIT SPI FRAM MB85RS4MT BREAKOUT BOARD PINOUT:
VCC     |  3.3V
GND     |  GND
HOLD    |  PB1: GPIO
SCK     |  PB4: SSI2 CLK
MISO    |  PB6: SSI2 RX
MOSI    |  PB7: SSI2 TX
CS      |  PB5: GPIO / SSI2 Fss
WP      |  PB0: GPIO
*/

#define PB_HOLD 0x02                        // PB1: GPIO
#define PB_SCK  0x10                        // PB4: SSI2 CLK
#define PB_MISO 0x40                        // PB6: SSI2 RX
#define PB_MOSI 0x80                        // PB7: SSI2 TX
#define PB_CS   0x20                        // PB5: GPIO
#define PB_WP   0x01                        // PB0: GPIO
#define PB_SSI2 (PB_SCK|PB_MISO|PB_MOSI)    // SSI2 Pins combined
#define PB_GPIO (PB_HOLD|PB_CS|PB_WP)       // GPIO Pins combined
#define PB_FRAM (PB_SSI2|PB_GPIO)           // Entire FRAM pinset
#define FRAM_CPSDVSR 0x02
#define FRAM_CR0_DSS 0x07
#define FRAM_CR0_SPO 0x40
#define FRAM_CR0_SPH 0x80
#define FRAM_CR0_SCR 0x0000
#define FRAM_CR0_FRF 0x00
#define FRAM_CR0 (FRAM_CR0_DSS|FRAM_CR0_SPO|FRAM_CR0_SPH|FRAM_CR0_SCR|FRAM_CR0_FRF)

#define OP_WREN     0x06    // Set Write Enable Latch
#define OP_WRDI     0x04    // Reset Write Enable Latch
#define OP_RDSR     0x05    // Read Status Register
#define OP_WRSR     0x01    // Write Status Register
#define OP_READ     0x03    // Read Memory Code
#define OP_WRITE    0x02    // Write Memory Code
#define OP_RDID     0x9F    // Read Device ID
#define OP_FSTRD    0x0B    // Fast Read Memory Code
#define OP_SLEEP    0xB9    // Sleep Mode



/*
MOST SIGNIFICANT NOTE SO FAR
Instead of letting the SSI module control the Fss bit
I currently have it set to a GPIO that I set low in the config
This is because the FRAM module has a HOLD bit that can be used
to interrupt (but not end) transmission
I believe this will be necessary for the kind of functions I want 
to develop for the module and currently I don't see any other way
of doing what I want.

If I find a better way, I'll return PB5 to Fss so that the SSI
module operates normally.

ADDITIONALLY

Might switch from APB GPIO to AHB GPIO because of faster response times


*/


/*!*******************************************************************
    @author Qwyntyn Scurr
    @brief Initialize the MB85RS for SSI communication
    @note PUR is enabled for SSI to prevent floating issues.
    @note Also because using SPI Mode 3 (CPO = 1, CPH = 1)
    @since February 11th, 2024
**********************************************************************/
void FRAM_INIT(){
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R2;

    // Wait for GPIO Clock
    while(!(SYSCTL_PRGPIO_R&SYSCTL_RCGCGPIO_R1)){};

    // PB[0:1,5] GPIO | PB[4,6:7] AFSEL | PB[2:3] Unchanged
    GPIO_PORTB_AFSEL_R = (GPIO_PORTB_AFSEL_R&~PB_FRAM) | PB_SSI2;
    // PB[4,6:7] Set MUX | PB[0:1,5] Clear MUX | PB[2:3] Unchanged
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0x0000FF00) | 0x22020000;
    // PB[4,6:7] PUR Enable | PB[0:3,5] Unchanged
    GPIO_PORTB_PUR_R = (GPIO_PORTB_PUR_R&~PB_SSI2) | PB_SSI2;
    // PB[0:1,5] GPIO Output | PB[2:4,6:7] Unchanged
    GPIO_PORTB_DIR_R = (GPIO_PORTB_DIR_R&~PB_GPIO) | PB_GPIO;
    // PB[0:1,4:7] Digital enable | PB[2:3] Unchanged
    GPIO_PORTB_DEN_R = (GPIO_PORTB_DEN_R&~PB_FRAM) | PB_FRAM;

    // SET CHIP SELECT AND HOLD LOW
    GPIO_PORTB_DATA_R &= ~(PB_CS|PB_HOLD);
    // SET WRITE PROTECT HIGH
    GPIO_PORTB_DATA_R |= PB_WP;

    
    // Wait for SSI2 Clock
    while(!(SYSCTL_PRSSI_R&SYSCTL_PRSSI_R2)){};

    // Disable Serial Port
    SSI2_CR1_R &= ~0x02;
    // Enable Master Mode
    SSI2_CR1_R &= ~0x03;
    // System/PLL Clock Control
    SSI2_CC_R = (SSI2_CC_R&~0xF) | 0x0;
    /* Max operating frequency: 40Mhz
       40Mhz = 80Mhz / (CPSDVSR * (1 + SCR))
       40Mhz = 80Mhz / (2 * (1 + 0))
       CPSDVSR = 2
       SCR = 0
    */ 
    // Set CPSDVSR for SSI2 CLK
    SSI2_CPSR_R = (SSI2_CPSR_R&~0xFF) | FRAM_CPSDVSR;
    // Set CR0 Settings for SSI2
    SSI2_CR0_R = (SSI2_CR0_R&0xFFFF0000) | FRAM_CR0;
    // Enable SSI2
    SSI2_CR1_R |= 0x2;
}

/*!*******************************************************************
    @author Qwyntyn Scurr
    @brief MB85RS4MT Datasheet:
            The WREN command sets WEL (Write Enable Latch). 
            WEL has to be set with the WREN command before
            writing operation (WRSR command and WRITE command). 
    @since February 11th, 2024
**********************************************************************/
void FRAM_OP_WREN(void){
    // WAIT UNTIL FIFO EMPTY
    while(SSI2_SR_R&SSI_SR_BSY){};
    // SET HOLD HIGH
    GPIO_PORTB_DATA_R |= PB_HOLD;
    // SEND OPCODE TO FIFO
    SSI2_DR_R = OP_WREN;
    // WAIT UNTIL FIFO EMPTY
    while(SSI2_SR_R&SSI_SR_BSY){};
    // SET HOLD LOW
    GPIO_PORTB_DATA_R &= ~PB_HOLD;
};

/*!*******************************************************************
    @author Qwyntyn Scurr
    @brief MB85RS4MT Datasheet:
            The WRDI command resets WEL (Write Enable Latch). 
            Writing operation (WRSR command and WRITE
            command) are not performed when WEL is reset. 
    @since February 11th, 2024
**********************************************************************/
void FRAM_OP_WRDI(void){
    // WAIT UNTIL FIFO EMPTY
    while(SSI2_SR_R&SSI_SR_BSY){};
    // SET HOLD HIGH
    GPIO_PORTB_DATA_R |= PB_HOLD;
    // SEND OPCODE TO FIFO
    SSI2_DR_R = OP_WRDI;
    // WAIT UNTIL FIFO EMPTY
    while(SSI2_SR_R&SSI_SR_BSY){};
    // SET HOLD LOW
    GPIO_PORTB_DATA_R &= ~PB_HOLD;
};

void WRITE_OPCODE(uint8_t opcode){
    // WAIT UNTIL FIFO EMPTY
    while(SSI2_SR_R&SSI_SR_BSY){};
    // SET HOLD HIGH
    GPIO_PORTB_DATA_R |= PB_HOLD;
    // SEND OPCODE TO FIFO
    SSI2_DR_R = opcode;
    // WAIT UNTIL FIFO EMPTY
    while(!(SSI2_SR_R&SSI_SR_TFE)){};
    // SET HOLD LOW
    GPIO_PORTB_DATA_R &= ~PB_HOLD;
};


void READ_DATA(){};

void WRITE_DATA(uint8_t data){
    // WAIT UNTIL FIFO NOT FULL
    while(!(SSI2_SR_R&SSI_SR_TNF)){};
    // SET HOLD HIGH
    GPIO_PORTB_DATA_R |= PB_HOLD;
    // SEND OPCODE TO FIFO
    SSI2_DR_R = data;
    // WAIT UNTIL FIFO EMPTY
    while(!(SSI2_SR_R&SSI_SR_TFE)){};
    // SET HOLD LOW
    GPIO_PORTB_DATA_R &= ~PB_HOLD;
}









/*!*******************************************************************
    @author Qwyntyn Scurr
    @brief MB85RS4MT Datasheet:
            The RDSR command reads status register data.
            After op-code of RDSR is input to SI, 
            8-cycle clock is input to SCK. 
            The SI value is invalid for this time.
            SO is output synchronously to a falling edge of SCK. 
            In the RDSR command, repeated reading of status register
            is enabled by sending SCK continuously before rising of CS 
    @since February 11th, 2024
**********************************************************************/
// uint8_t FRAM_OP_RDSR(void){
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_RNE){};
//     // SET HOLD HIGH
//     GPIO_PORTB_DATA_R |= PB_HOLD;
//     // SEND OPCODE TO FIFO
//     SSI2_DR_R = OP_RDSR;
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // // SET HOLD LOW
//     // GPIO_PORTB_DATA_R &= ~PB_HOLD;
// };

// void FRAM_OP_WRSR(void){
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD HIGH
//     GPIO_PORTB_DATA_R |= PB_HOLD;
//     // SEND OPCODE TO FIFO
//     SSI2_DR_R = OP_WRDI;
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD LOW
//     GPIO_PORTB_DATA_R &= ~PB_HOLD;
// };

// void FRAM_OP_READ(void){
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD HIGH
//     GPIO_PORTB_DATA_R |= PB_HOLD;
//     // SEND OPCODE TO FIFO
//     SSI2_DR_R = OP_WRDI;
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD LOW
//     GPIO_PORTB_DATA_R &= ~PB_HOLD;
// };

// void FRAM_OP_WRITE(uint32_t address){
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD HIGH
//     GPIO_PORTB_DATA_R |= PB_HOLD;
//     // SEND OPCODE TO FIFO
//     SSI2_DR_R = OP_WRDI;
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD LOW
//     GPIO_PORTB_DATA_R &= ~PB_HOLD;
// };

// void FRAM_OP_FSTRD(void){
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD HIGH
//     GPIO_PORTB_DATA_R |= PB_HOLD;
//     // SEND OPCODE TO FIFO
//     SSI2_DR_R = OP_WRDI;
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD LOW
//     GPIO_PORTB_DATA_R &= ~PB_HOLD;
// };

// void FRAM_OP_RDID(void){
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD HIGH
//     GPIO_PORTB_DATA_R |= PB_HOLD;
//     // SEND OPCODE TO FIFO
//     SSI2_DR_R = OP_WRDI;
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD LOW
//     GPIO_PORTB_DATA_R &= ~PB_HOLD;
// };

// void FRAM_OP_SLEEP(void){
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD HIGH
//     GPIO_PORTB_DATA_R |= PB_HOLD;
//     // SEND OPCODE TO FIFO
//     SSI2_DR_R = OP_WRDI;
//     // WAIT UNTIL FIFO EMPTY
//     while(SSI2_SR_R&SSI_SR_BSY){};
//     // SET HOLD LOW
//     GPIO_PORTB_DATA_R &= ~PB_HOLD;
// };
