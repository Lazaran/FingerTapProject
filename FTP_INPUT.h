 /*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Corresponding H file for FTP_INPUT.c, holds macros and function
            prototypes
    @since March 4, 2024
    @version Rev 3
**********************************************************************/

/* File FTP_INPUT.h*/
#ifndef FTP_INPUT_H
#define FTP_INPUT_H

#include <stdint.h>

// Conductive Pad Circuit Conditional Mins and Maxes
#define PP_MIN 3650
#define PP_MAX 3750
#define PA_MIN 3050
#define PA_MAX 3150
#define MA_MIN 2250
#define MA_MAX 2550
#define MP_MIN 1850
#define MP_MAX 1950
#define DP_MIN 1350
#define DP_MAX 1450

// FSR Conditional Mins and Maxes (TEMP NEEDS CHANGING)
#define L1_MIN  10
#define L1_MAX  10
#define L2_MIN  10
#define L2_MAX  10
#define L3_MIN  10
#define L3_MAX  10
#define L4_MIN  10
#define L4_MAX  10
#define L5_MIN  10
#define L5_MAX  10

// Input Volatiles Declaration
extern volatile uint16_t IndexCircuit;
extern volatile uint16_t MiddleCircuit;
extern volatile uint16_t RingCircuit;
extern volatile uint16_t PinkyCircuit;
extern volatile uint16_t IndexFSR;
extern volatile uint16_t MiddleFSR;
extern volatile uint16_t RingFSR;
extern volatile uint16_t PinkyFSR;

// Function Prototypes
uint8_t Circuit_Convert(uint16_t input);

uint8_t Circuit_Parse(void);

uint8_t FSR_Convert(uint16_t input);

uint8_t FSR_Parse(void);

void ADC0Seq1_Handler(void);

void ADC0Seq2_Handler(void);

void Input_Init(void);

#endif