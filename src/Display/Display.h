//
//  Display.h
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef Display_h
#define Display_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__
#	define GLFW_INCLUDE_GLCOREARB
#endif

#ifdef USE_GLFW
#include <GLFW/glfw3.h>
#elif defined USE_DISPMAN
#include "piGL.h"
#endif

typedef enum
{
    DisplayUnknown = 0,
    DisplayGLFW    = 1,
    DisplayDispman = 2
    
} DisplayType;

typedef struct
{
    DisplayType type;
    void* _handle;
    
}Display;

int DisplayInit( Display *disp);
int DisplayRelease( Display *disp);

void DisplayMakeContextCurrent( Display *disp);

static inline DisplayType DisplayGetType( const Display *disp)
{
    return disp->type;
}
void DisplaySwap( const Display* disp);

void DisplayGetWindowSize( const Display* disp, int* width, int* height);
void DisplayGetFramebufferSize( const Display* disp, int* width, int* height);

int DisplayGetCursorPos( const Display* disp, double* x, double* y);

int  DisplayShouldClose( const Display* disp);
void DisplaySetShouldClose( Display* disp , int value);

void DisplayPollEvents( const Display *disp);
void DisplayWaitEvents( const Display *disp);
    
#ifdef __cplusplus
}
#endif

#endif /* Display_h */
