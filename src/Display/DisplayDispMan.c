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

static int get_mouse( PiGLState *state, int *outx, int *outy);


int DisplayInit( Display *disp)
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

	init_ogl(  (PiGLState*)disp->_handle );
	disp->type = DisplayDispman;

        return disp->_handle != NULL;
    }
    return 0;
}

int DisplayRelease( Display *disp)
{
    assert(disp);

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
        
        const int mouseState = get_mouse( state , &x  ,&y);
        
    }
}

void DisplayWaitEvents( const Display *disp)
{
 //   glfwWaitEvents();
}


static int get_mouse( PiGLState *state, int *outx, int *outy)
{
    static int fd = -1;
    const int width=state->screen_width, height=state->screen_height;
    static int x=800, y=400;
    const int XSIGN = 1<<4, YSIGN = 1<<5;
    
    if (fd<0)
    {
       fd = open("/dev/input/mouse0",O_RDONLY|O_NONBLOCK);
    }
    if (fd>=0) {
        struct {char buttons, dx, dy; } m;
        while (1) {
           int bytes = read(fd, &m, sizeof m);
           if (bytes < (int)sizeof m) goto _exit;
           if (m.buttons&8) {
              break; // This bit should always be set
           }
           read(fd, &m, 1); // Try to sync up again
        }
        if (m.buttons&3)
           return m.buttons&3;
        x+=m.dx;
        y+=m.dy;
        if (m.buttons&XSIGN)
           x-=256;
        if (m.buttons&YSIGN)
           y-=256;
        if (x<0) x=0;
        if (y<0) y=0;
        if (x>width) x=width;
        if (y>height) y=height;
   }
_exit:
   if (outx) *outx = x;
   if (outy) *outy = y;
   return 0;
}  

int DisplayGetCursorPos( const Display* disp, double* x, double* y)
{
    assert(disp);
    assert(disp->_handle);
    PiGLState* state = (PiGLState*) disp->_handle;
    int xx = 0;
    int yy = 0;
    get_mouse( state , &xx , &yy);
    *x = xx;
    *y = state->screen_height - yy;
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
