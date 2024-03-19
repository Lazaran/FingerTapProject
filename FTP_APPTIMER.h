 /*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Corresponding H file for FTP_APPTIMER.c, holds macros and function
            prototypes
    @since March 4, 2024
    @version Rev 3
**********************************************************************/

#ifndef FTP_APPTIMER_H
#define FTP_APPTIMER_H

#include "tm4c123gh6pm.h"

// Macros
#define TIMER_ENABLE 0x0001
#define TIMER_DISABLE 0x0000
#define TIMER_STATUS_INTERRUPT 0x1
#define TIMER_CLEAR_INTERRUPT 0x0

// Prototypes
void AppTimer_Init(void);

#endif
