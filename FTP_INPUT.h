/* File FTP_INPUT.h*/
#ifndef FTP_INPUT_H
#define FTP_INPUT_H

#include <stdint.h>

extern volatile uint16_t IndexCircuit;
extern volatile uint16_t MiddleCircuit;
extern volatile uint16_t RingCircuit;
extern volatile uint16_t PinkyCircuit;
extern volatile uint16_t IndexFSR;
extern volatile uint16_t MiddleFSR;
extern volatile uint16_t RingFSR;
extern volatile uint16_t PinkyFSR;

void ADC0Seq1_Handler(void);

void ADC0Seq2_Handler(void);

void Input_Init(void);


#endif