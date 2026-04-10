#pragma once

#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 256
#define SCALED_WIDTH (4 * SCREEN_WIDTH)   //3x = 672p   //4x = 896p   // 5x = 1120p 
#define SCALED_HEIGHT (4 * SCREEN_HEIGHT) //3x = 768p   //4x = 1024p  // 5x = 1280p 
#define GRAVITY 0.9f
//Anything the camera looks at / draws ? world space ? SCREEN_WIDTH / HEIGHT
//Anything describing the window itself ? screen space ? SCALED_WIDTH / HEIGHT
