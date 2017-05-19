//
//  Display.c
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "Display.h"
#include "piGL.h"


#include <fcntl.h>
#include <linux/input.h>
#include <stdint.h>
#define MOUSEFILE "/dev/input/mouse0"


static void closeMouse(void);
static int initMouse(void);
static int getMouse( const  Display* disp );

static int fd;
static struct input_event ie;

static int xAbs = 0;
static int yAbs = 0;
static int lastButtonState = -1;

int DisplayInit( Display *disp ,int width , int height)
{
     bcm_host_init();
    assert(disp);
    
    if( disp)
    {
        disp->type = DisplayUnknown;


	disp->_handle = malloc(sizeof(PiGLState));
	assert( disp->_handle );
	
	if( disp->_handle == NULL)
	    return 0;

    PiGLState* state = (PiGLState*)disp->_handle ;
    state->screen_width = width;
    state->screen_height = height;
        
	init_ogl(  state );
	disp->type = DisplayDispman;
	initMouse();
        return disp->_handle != NULL;
    }
    return 0;
}

int DisplayRelease( Display *disp)
{
    assert(disp);

    closeMouse();
    if( disp->_handle)
    {
	free(disp->_handle);
    }
    return 1;
}

void DisplayMakeContextCurrent( Display *disp)
{
    assert(disp);
    assert(disp->_handle);
    PiGLState* state = (PiGLState*) disp->_handle;
    // connect the context to the surface
    EGLBoolean result = eglMakeCurrent(state->display, state->surface, state->surface, state->context);
    assert(EGL_FALSE != result);

}

void DisplaySwap( const Display* disp)
{
    assert(disp);
    assert(disp->_handle);
    PiGLState* state = (PiGLState*) disp->_handle;
    eglSwapBuffers( state->display, state->surface);
}

void DisplayGetWindowSize( const Display* disp, int* width, int* height)
{
    assert(disp);
    assert(disp->_handle);
    PiGLState* state = (PiGLState*) disp->_handle;
	
    *width = state->screen_width;
    *height = state->screen_height;
}

void DisplayGetFramebufferSize( const Display* disp, int* width, int* height)
{
    assert(disp);
    assert(disp->_handle);
    
    DisplayGetWindowSize( disp , width , height);
}

int DisplayShouldClose( const Display* disp)
{
    assert(disp);
    assert(disp->_handle);
    return 0;
}

void DisplaySetShouldClose( Display* disp , int value)
{
    assert(disp);
    assert(disp->_handle);
}

void DisplayPollEvents( const Display *disp)
{
    if( disp->eventListener)
    {
        PiGLState* state = (PiGLState*) disp->_handle;
        int x = -1;
        int y = -1;
        
        const int mouseState = getMouse(disp);
        printf("Mouse Pos %i %i state \n" , xAbs , yAbs);
	
	static int lastState = -1;

	if( mouseState != lastState)
	{
	   lastState = mouseState;
	}
        GXEventMouse mouseEv;
        mouseEv.type = GXEventTypeMouse;
        mouseEv.state = mouseState == 1? GXMouseStatePressed : GXMouseStateReleased;//  GXMouseStateMoving;
        mouseEv.x = (float) x;
        mouseEv.y = (float) y;

	disp->eventListener( (void*) disp , (const GXEvent*) &mouseEv);
    }
}

void DisplayWaitEvents( const Display *disp)
{
 //   glfwWaitEvents();
}

/* **** **** **** **** **** **** **** **** **** **** **** */



static int initMouse()
{
    if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
        printf("Device open ERROR\n");
        return 0;
    }
    return 1;
}

static void closeMouse()
{
    close( fd );
}

static int getMouse( const  Display* disp )
{
    unsigned char button,bLeft,bMiddle,bRight;
    int8_t x,y;
   if(read(fd, &ie, sizeof(struct input_event)))
    {
        unsigned char *ptr = (unsigned char*)&ie;
        int i;       
        //
        button=ptr[0];
        bLeft = button & 0x1;
        bMiddle = ( button & 0x4 ) > 0;
        bRight = ( button & 0x2 ) > 0;
        x= ptr[1];
        y=ptr[2];

        xAbs += x;
        yAbs -= y;

        if( xAbs <= 0) 
            xAbs = 0;

        if( yAbs <= 0)
            yAbs = 0;

	if( bLeft != lastButtonState)
        {
            printf("Button changed to %i \n" , bLeft);
            lastButtonState = bLeft;

	    GXEventMouse mouseEv;
        mouseEv.type = GXEventTypeMouse;
        mouseEv.state = bLeft == 1? GXMouseStatePressed : GXMouseStateReleased;//  GXMouseStateMoving;
        mouseEv.x = (float) xAbs;
        mouseEv.y = (float) yAbs;
        disp->eventListener( (void*) disp , (const GXEvent*) &mouseEv);
        }

        //printf("bLEFT:%d, bMIDDLE: %d, bRIGHT: %d, rx: %i  ry=%i Abs %i %i \n",bLeft,bMiddle,bRight, x,y , xAbs , yAbs);
    }

    return 1;
}

/* **** **** **** **** **** **** **** **** **** **** **** */


int DisplayGetCursorPos( const Display* disp, double* x, double* y)
{
    assert(disp);
    assert(disp->_handle);
    PiGLState* state = (PiGLState*) disp->_handle;
    int xx = 0;
    int yy = 0;
    
    *x = (double) xAbs;
    *y = (double) yAbs;
    //*x = 200;
    //*y = 200; 
    return 0;
}


const char* GXKeyGetChar( const GXEventKey* key)
{
    return NULL;
}

void DisplaySetEventCallback(Display* disp , GXEventListener callback)
{
    assert(disp);
    disp->eventListener = callback;
}


void  DisplaySetUserContext( Display* disp , void* user)
{
    assert(disp);
    disp->_usr = user;
}
void* DisplayGetUserContext( Display* disp)
{
    assert(disp);
    return disp->_usr;
}
