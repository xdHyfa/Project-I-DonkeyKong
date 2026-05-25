#pragma once


#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 256
#define SCALED_WIDTH (3 * SCREEN_WIDTH)   //3x = 672p   //4x = 896p   // 5x = 1120p 
#define SCALED_HEIGHT (3* SCREEN_HEIGHT) //3x = 768p   //4x = 1024p  // 5x = 1280p 
#define GRAVITY 0.9f

extern int AllTimeScore; 
extern bool Option1True;
extern bool Option2True;
extern bool Option3True;
extern bool Option4True;

void SaveScores();
//Anything the camera looks at / draws ? world space ? SCREEN_WIDTH / HEIGHT
//Anything describing the window itself ? screen space ? SCALED_WIDTH / HEIGHT
