//Standard Includes
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

//FTP Includes
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_GAMEOBJ.h"
#include "FTP_PHYSX.h"
#include "FTP_PONG.h"

//External Lib Includes
#include "SysTick.h"
#include "tm4c123gh6pm.h"

// END OF INCLUDES

//Vector Modifiers
#define VEC_ADD 1
#define VEC_SUB -1
#define VEC_NON 0

//Pong Viewport Dimensions
#define P_VP_X ScreenX
#define P_VP_Y ScreenY
#define P_VP_W ScreenW
#define P_VP_H ScreenH

//Pong Paddle Dimensions
#define P_PAD_X 5
#define P_PAD_Y 40
#define P_PAD_W 20
#define P_PAD_H 36

//Pong Ball Dimensions
#define P_BALL_X 40
#define P_BALL_Y 80
#define P_BALL_W 20
#define P_BALL_H 20
#define P_BALL_XVEL 2
#define P_BALL_YVEL 2

//END OF MACRO DEFINITIONS


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Calls necessary functions to do physics for Pong
  @param  ball Pong Ball struct
  @param  paddle Pong Paddle struct
  @param  viewport Pong Viewport struct
  @since February 25, 2024
**********************************************************************/
void orderedUpdate(struct GameObject *ball, struct GameObject *paddle, struct GameObject *viewport){
    p_VelUpdate(ball);
    p_PosUpdate(ball);
    p_ViewCollide(ball, viewport);
    p_AABBCollide(ball, paddle);
    p_Bounce(ball, viewport);
}


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Pong game init and hyper-loop
  @returns Return 0 if game closes 
  @since February 25, 2024
**********************************************************************/
int pong_main(void) {
    uint8_t exitcode = 1;

	// Generate GameObjects
	struct GameObject PongPaddle = {ST7735_WHITE,P_PAD_X,P_PAD_Y,P_PAD_W,P_PAD_H,0,0,0,0};
	struct GameObject PongBall = {ST7735_WHITE,P_BALL_X,P_BALL_Y,P_BALL_W,P_BALL_H,P_BALL_XVEL,P_BALL_YVEL,0,0};
  struct GameObject PongViewport = {ST7735_BLACK,ScreenX,ScreenY,ScreenW,ScreenH,0,0,0,0};

    // Pong Hyperloop
	while(exitcode){
        SysTick_Wait10ms(25);
        r_Rect(PongBall.X, PongBall.Y, PongBall.W, PongBall.H, ST7735_BLACK);
        orderedUpdate(&PongBall, &PongBall, &PongViewport);
        r_Rect(PongBall.X, PongBall.Y, PongBall.W, PongBall.H, PongBall.COLOR);
        // r_Rect(40, 80, 20, 20, ST7735_WHITE);
    };

    return exitcode;
};
