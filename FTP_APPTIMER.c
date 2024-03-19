/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief This library contains the intialization and configuration for the timer
            to be used for any active application. Functions for setting
            the load value and enabling/disabling/resetting the clock exist
    @note Load should always be a multiple of the FTP_INPUT Timer load
    @since March 17, 2024
    @version Rev 4
**********************************************************************/

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FTP_INPUT.h"

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Intializes and configures GPTM Module 1, Timer A to loop
            5 times slower than the glove is receiving inputs, ensuring
            that there is always an input for the program to use?
    @note Load should always be a multiple of the FTP_INPUT Timer load
    @since March 17, 2024
    @version Rev 4
**********************************************************************/
void AppTimer_Init(void){
    // CLOCK INIT
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;  // Clock Enable: GPTM Module 1

    // TIMER INIT
    TIMER1_CTL_R = 0x0000;      // Control: Disable timer
    TIMER1_CFG_R = 0x00000000;  // Configuration: Set 32-bit Timer
    TIMER1_TAMR_R = 0x0002;     // Timer A Mode: Set Periodic Timer
    TIMER1_TAILR_R = (20*FORTY_LOAD); // Interval Load: Load value
    TIMER1_IMR_R = 0x1;
    // Enabling/Disabling beyond the initialization is done as needed in application libraries
};

