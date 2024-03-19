/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Handles inputs from the FingerTap glove, both from the conductive
            pads and the FSRs on each finger. This library does a few things:
              - Holds the volatile variables for the rest of the applications
                to read.
              - Initializes the necessary modules for the inputs to function.
              - Defines the interrupts for the ADC to read to the volatiles.
    @since March 4, 2024
    @version Rev 2
**********************************************************************/

// Library Includes
#include "tm4c123gh6pm.h"
#include "FTP_INPUT.h"



// Input Volatiles
volatile uint16_t IndexCircuit = 0;
volatile uint16_t MiddleCircuit = 0;
volatile uint16_t RingCircuit = 0;
volatile uint16_t PinkyCircuit = 0;
volatile uint16_t IndexFSR = 0;
volatile uint16_t MiddleFSR = 0;
volatile uint16_t RingFSR = 0;
volatile uint16_t PinkyFSR = 0;

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Reads the ADC FIFO to get inputs from the conductive pads.
          Called when AD0 Module 0, Sample Sequencer 1 generates an interrupt
  @since March 3, 2024
**********************************************************************/
void ADC0Seq1_Handler(void){
    IndexCircuit = ADC0_SSFIFO1_R;
    MiddleCircuit = ADC0_SSFIFO1_R;
    RingCircuit = ADC0_SSFIFO1_R;
    PinkyCircuit = ADC0_SSFIFO1_R;
    ADC0_ISC_R |= 0x2;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Reads the ADC FIFO to get inputs from the FSRs.
          Called when AD0 Module 0, Sample Sequencer 2 generates an interrupt
  @since March 3, 2024
**********************************************************************/
void ADC0Seq2_Handler(void){
    IndexFSR = ADC0_SSFIFO2_R;
    MiddleFSR = ADC0_SSFIFO2_R;
    RingFSR = ADC0_SSFIFO2_R;
    PinkyFSR = ADC0_SSFIFO2_R;
    ADC0_ISC_R |= 0x4;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Converts a conductive pad input value into a code based on which pad
          is active
  @since March 3, 2024
**********************************************************************/
uint8_t Circuit_Convert(uint16_t input){
  // Proximal Posterior Pad
  if (input > PP_MIN && input < PP_MAX){
    return 1;
  }
  // Proximal Anterior Pad
  else if (input > PA_MIN && input < PA_MAX){
    return 2;
  }
  // Middle Anterior Pad
  else if (input > MA_MIN && input < MA_MAX){
    return 3;
  }
  // Middle Posterior Pad
  else if (input > MP_MIN && input < MP_MAX){
    return 4;
  }
  // Distal Posterior Pad
  else if (input > DP_MIN && input < DP_MAX){
    return 5;
  }
	return 0;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Parses the pad inputs of each finger, returning a code on the
           first one with a value above 300 (an arbitrary limit)
  @since March 3, 2024
**********************************************************************/
uint8_t Circuit_Parse(void){
  // Index Finger Conductive Pad Circuit
  if (IndexCircuit > 300){
    return (10 + Circuit_Convert(IndexCircuit));
  }
  // Middle Finger Conductive Pad Circuit
  else if (MiddleCircuit > 300){
    return (20 + Circuit_Convert(MiddleCircuit));
  }
  // Ring Finger Conductive Pad Circuit
  else if (RingCircuit > 300){
    return (30 + Circuit_Convert(RingCircuit));
  }
  // Pinky Finger Conductive Pad Circuit
  else if (PinkyCircuit > 300){
    return (40 + Circuit_Convert(PinkyCircuit));
  }
	return 0;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Converts an FSR input value into a code based on the pressure 
          being exerted on the FSR
  @since March 3, 2024
**********************************************************************/
uint8_t FSR_Convert(uint16_t input){
  if (input > L1_MIN && input < L1_MAX){
    return 1;
  }
  else if (input > L2_MIN && input < L2_MAX){
    return 2;
  }
  else if (input > L3_MIN && input < L3_MAX){
    return 3;
  }
  else if (input > L4_MIN && input < L4_MAX){
    return 4;
  }
  else if (input > L5_MIN && input < L5_MAX){
    return 5;
  }
	return 0;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Parses the FSR inputs of each finger, returning a code on the
           first FSR with a value above 300 (an arbitrary limit)
  @since March 3, 2024
**********************************************************************/
uint8_t FSR_Parse(void){
  // Index Finger FSR
  if (IndexFSR > 300){
    return (10 + FSR_Convert(IndexFSR));
  }
  // Middle Finger FSR
  else if (MiddleFSR > 300){
    return (20 + FSR_Convert(MiddleFSR));
  }
  // Ring Finger FSR
  else if (RingFSR > 300){
    return (30 + FSR_Convert(RingFSR));
  }
  // Pinky Finger FSR
  else if (PinkyFSR > 300){
    return (40 + FSR_Convert(PinkyFSR));
  }
	return 0;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Initializing ADC Module 0 Sequencers 1 and 2, GPIO Ports E and D, and GPTM Module 0
  @since March 3, 2024
**********************************************************************/
void Input_Init(void){
    // CLOCK INIT
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4 | SYSCTL_RCGCGPIO_R3; // Clock Enable: GPIO Port E + Port D
    SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0;      // Clock Enable: ADC Module 0
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;  // Clock Enable: GPTM Module 0
    
    // GPIO INIT
    GPIO_PORTE_AFSEL_R = 0x0F;  // Alt Func Select: Enable PortE[0:3]
    GPIO_PORTD_AFSEL_R = 0x0F;  // Alt Func Select: Enable PortD[0:3]
    GPIO_PORTE_DEN_R = 0x00;    // Digital Select: Disable PortE[0:3]
    GPIO_PORTD_DEN_R = 0x00;    // Digital Select: Disable PortD[0:3]
    GPIO_PORTE_AMSEL_R = 0x0F;  // Analog Select: Enable PortE[0:3]
    GPIO_PORTD_AMSEL_R = 0x0F;  // Analog Select: Enable PortD[0:3] 

    // ADC INIT
    ADC0_SSPRI_R = 0x2103;      // Seq1 = Pri 0 | Seq2 = Pri 1 | Seq3 = Pri 2 | Seq0 = Pri 3
    ADC0_ACTSS_R = 0x0;         // Active Sequencers: Disable All
    ADC0_EMUX_R = 0x0550;       // Event Mux: Seq[1:2] GPTM Timeout
    ADC0_SSMUX1_R = 0x3210;     // Sequence Input Mux: AIN[0:3]
    ADC0_SSMUX2_R = 0x7654;     // Sequence Input Mux: AIN[4:7]
    ADC0_SSCTL1_R = 0x6000;     // Seq Control: AIN[3] Interrupt + End | AIN[0:2] Empty
    ADC0_SSCTL2_R = 0x6000;     // Seq Control: AIN[7] Interrupt + End | AIN[4:6] Empty
    ADC0_IM_R = 0x6;            // Interrupt Mask: Enable Seq[1:2]
    ADC0_ISC_R = 0xF;           // Interrupt Status/Clear: Clear All Interrupts
    ADC0_ACTSS_R = 0x6;         // Active Sequencers: Enable Seq[1:2]

    // ADC | Seq1 = Interrupt 15 | Seq2 = Interrupt 16
    NVIC_EN0_R = 0x00018000;    // Interrupt Enable: Enable Int[15,16]
    NVIC_PRI3_R = 0x60000000;   // Interrupt Priority: Priority 3 Int[15]
    NVIC_PRI4_R = 0x000000A0;   // Interrupt Priority: Priority 5 Int[16]

    // TIMER INIT
    TIMER0_CTL_R = 0x0000;      // Control: Disable timer
    TIMER0_CFG_R = 0x00000000;  // Configuration: Set 32-bit Timer
    TIMER0_TAMR_R = 0x0002;     // Timer A Mode: Set Periodic Timer
    TIMER0_TAILR_R = FORTY_LOAD; // Interval Load: Load value: 40,000,000
    TIMER0_CTL_R = 0x0021;      // Control: Enable Timer A
};