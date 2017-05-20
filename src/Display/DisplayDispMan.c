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
#include <string.h>     /* strerror() */
#include <errno.h>      /* errno */
#include <unistd.h>     /* close() */
#include <sys/ioctl.h>  /* ioctl() */
#include <linux/input.h>    /* EVIOCGVERSION ++ */
#include <stdint.h>

#include "Display.h"
#include "piGL.h"

#define MOUSEFILE "/dev/input/mouse0"
#define KEYFILE "/dev/input/by-id/usb-Logitech_USB_Receiver-if02-event-mouse"

/* Mouse stuff */
static void closeMouse(void);
static int initMouse(void);
static int getMouse( const  Display* disp );

static int fdMouse;
static struct input_event ie;

static int xAbs = 0;
static int yAbs = 0;
static int lastButtonState = -1;

/* Keyboard stuff*/

static GXKeyCode KeyConvToGX( int k);
static void closeKey(void);
static int initKey(void);
static int getKey( const  Display* disp );

static int fdKey;

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
    initKey();
        
        return disp->_handle != NULL;
    }
    return 0;
}

int DisplayRelease( Display *disp)
{
    assert(disp);

    closeMouse();
    closeKey();
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

	getKey(disp);

        static int lastState = -1;

        GXEventMouse mouseEv;
        
        if( mouseState != lastState)
        {
            mouseEv.state = mouseState == 1? GXMouseStatePressed : GXMouseStateReleased;//  GXMouseStateMoving;
            lastState = mouseState;
            
        }
        else
        {
            mouseEv.state = GXMouseStateMoving;
        }

        mouseEv.type = GXEventTypeMouse;
        
        mouseEv.x = (float) xAbs;
        mouseEv.y = (float) yAbs;

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
    if((fdMouse = open(MOUSEFILE, O_RDONLY)) == -1)
    {
        printf("Device open ERROR\n");
        return 0;
    }

int flags = fcntl(fdMouse, F_GETFL, 0);
fcntl(fdMouse, F_SETFL, flags | O_NONBLOCK);
    return 1;
}

static void closeMouse()
{
    close( fdMouse );
}

static int getMouse( const  Display* disp )
{
    unsigned char button,bLeft,bMiddle,bRight;
    int8_t x = 0,y = 0;
    
    ssize_t ret = read(fdMouse, &ie, sizeof(struct input_event));
    if( ret == -1 )
    {
//	printf("Only read %zi \n" , ret);
        return 0;
    }
    else
    {
        unsigned char *ptr = (unsigned char*)&ie;

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
            lastButtonState = bLeft;

	    GXEventMouse mouseEv;
        mouseEv.type = GXEventTypeMouse;
        mouseEv.state = bLeft == 1? GXMouseStatePressed : GXMouseStateReleased;//  GXMouseStateMoving;
        mouseEv.x = (float) xAbs;
        mouseEv.y = (float) yAbs;
        disp->eventListener( (void*) disp , (const GXEvent*) &mouseEv);
        }

    }

    return 1;
}

/* **** **** **** **** **** **** **** **** **** **** **** */


int DisplayGetCursorPos( const Display* disp, double* x, double* y)
{
    assert(disp);
    assert(disp->_handle);
    PiGLState* state = (PiGLState*) disp->_handle;

    *x = (double) xAbs;
    *y = (double) yAbs;
 
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


/* **** **** **** **** **** **** **** **** **** **** */

static void closeKey()
{
    close(fdKey);
}
static int initKey(void)
{
    if ((fdKey = open( KEYFILE, O_RDONLY)) < 0)
    {
        fprintf(stderr,
                "ERR %d:\n"
                "Unable to open `%s'\n"
                "%s\n",
                errno, KEYFILE, strerror(errno)
                );
        return 0;
    }

int flags = fcntl(fdKey, F_GETFL, 0);
fcntl(fdKey, F_SETFL, flags | O_NONBLOCK);

unsigned version;
    unsigned short id[4];                   /* or use struct input_id */
    char name[256] = "N/A";

    
    /* Error check here as well. */
    ioctl(fdKey, EVIOCGVERSION, &version);
    ioctl(fdKey, EVIOCGID, id); 
    ioctl(fdKey, EVIOCGNAME(sizeof(name)), name);

    fprintf(stderr,
        "Name      : %s\n"
        "Version   : %d.%d.%d\n"
        "ID        : Bus=%04x Vendor=%04x Product=%04x Version=%04x\n"
        "----------\n"
        ,
        name,

        version >> 16,
        (version >> 8) & 0xff,
        version & 0xff,

        id[ID_BUS],
        id[ID_VENDOR],
        id[ID_PRODUCT],
        id[ID_VERSION]
    );


    return 1;
    
}
#define EV_BUF_SIZE 16

static int getKey( const  Display* disp )
{
    struct input_event ev[EV_BUF_SIZE]; /* Read up to N events ata time */
    int  sz;
    unsigned i;

    sz = read(fdKey, ev, sizeof(struct input_event) * EV_BUF_SIZE);

        if (sz < (int) sizeof(struct input_event)) 
        {
	    /*
            fprintf(stderr,
                "ERR %d:\n"
                "Reading of `%s' failed\n"
                "%s\n",
                errno, KEYFILE, strerror(errno)
            );
		*/
	    return 0;
        }

/* Implement code to translate type, code and value */
        int kVal = 0;
        for (i = 0; i < sz / sizeof(struct input_event); ++i)
        {
            if( ev[i].type == EV_SYN)
            {
//              printf("SYNC\n");
            }
            else if( ev[i].type == EV_MSC)
            {
                //printf("Key Val Code %02x val %02x : ", ev[i].code , ev[i].value);
                
                kVal = ev[i].value;

            }
            else if( ev[i].type == EV_KEY)
            {
                /*0 press 1 release 2 repeat */
		/*
                if( ev[i].value == 1)  printf(" Press - ");
                if( ev[i].value == 0)  printf(" Release - ");
                if( ev[i].value == 2)  printf(" Repeat -");
                
                printf("Code : %02x\n" , ev[i].code);
                */

                
                 GXEventKey keyEv;
                 keyEv.type = GXEventTypeKey;
                 
                 keyEv.action   = (GXKeyAction)ev[i].value;
                 keyEv.code     = KeyConvToGX( ev[i].code );
                 keyEv.mod      = (GXKeyMod)0;
                 keyEv.scanCode = 0;
                 
                 disp->eventListener((void*)disp , (const GXEvent*) &keyEv);
                 
                
            }
            
        }

    return 1;
}


static GXKeyCode KeyConvToGX( int k)
{
    switch (k)
    {
        case KEY_ESC		:
            return GXKey_ESCAPE;
        case KEY_1			:
            return GXKey_1;
        case KEY_2			:
            return GXKey_2;
        case KEY_3			:
            return GXKey_3;
        case KEY_4			:
            return GXKey_4;
        case KEY_5			:
            return GXKey_5;
        case KEY_6			:
            return GXKey_6;
        case KEY_7			:
            return GXKey_7;
        case KEY_8			:
            return GXKey_8;
        case KEY_9			:
            return GXKey_9;
        case KEY_0			:
            return GXKey_0;
        case KEY_MINUS		:
            return GXKey_MINUS;
        case KEY_EQUAL		:
            return GXKey_EQUAL;
        case KEY_BACKSPACE	:
            return GXKey_BACKSPACE;
        case KEY_TAB		:
            return GXKey_TAB;
        case KEY_Q			:
            return GXKey_Q;
        case KEY_W			:
            return GXKey_W;
        case KEY_E			:
            return GXKey_E;
        case KEY_R			:
            return GXKey_R;
        case KEY_T			:
            return GXKey_T;
        case KEY_Y			:
            return GXKey_Y;
        case KEY_U			:
            return GXKey_U;
        case KEY_I			:
            return GXKey_I;
        case KEY_O			:
            return GXKey_O;
        case KEY_P			:
            return GXKey_P;
        case KEY_LEFTBRACE	:
            return GXKey_LEFT_BRACKET;
        case KEY_RIGHTBRACE	:
            return GXKey_RIGHT_BRACKET;
        case KEY_ENTER		:
            return GXKey_ENTER;
        case KEY_LEFTCTRL	:
            return GXKey_LEFT_CONTROL;
        case KEY_A			:
            return GXKey_A;
        case KEY_S			:
            return GXKey_S;
        case KEY_D			:
            return GXKey_D;
        case KEY_F			:
            return GXKey_F;
        case KEY_G			:
            return GXKey_G;
        case KEY_H			:
            return GXKey_H;
        case KEY_J			:
            return GXKey_J;
        case KEY_K			:
            return GXKey_K;
        case KEY_L			:
            return GXKey_L;
        case KEY_SEMICOLON	:
            return GXKey_SEMICOLON;
        case KEY_APOSTROPHE	:
            return GXKey_APOSTROPHE;
        case KEY_GRAVE		:
            return GXKey_GRAVE_ACCENT;
        case KEY_LEFTSHIFT	:
            return GXKey_LEFT_SHIFT;
        case KEY_BACKSLASH	:
            return GXKey_BACKSLASH;
        case KEY_Z			:
            return GXKey_Z;
        case KEY_X			:
            return GXKey_X;
        case KEY_C			:
            return GXKey_C;
        case KEY_V			:
            return GXKey_V;
        case KEY_B			:
            return GXKey_B;
        case KEY_N			:
            return GXKey_N;
        case KEY_M			:
            return GXKey_M;
        case KEY_COMMA		:
            return GXKey_COMMA;
        case KEY_DOT		:
            return GXKey_PERIOD;
        case KEY_SLASH		:
            return GXKey_SLASH;
        case KEY_RIGHTSHIFT	:
            return GXKey_RIGHT_SHIFT;
        case KEY_KPASTERISK	:
            return (GXKeyCode)0;
        case KEY_LEFTALT	:
            return GXKey_LEFT_ALT;
        case KEY_SPACE		:
            return GXKey_SPACE;
        case KEY_CAPSLOCK	:
            return GXKey_CAPS_LOCK;
        case KEY_F1			:
            return GXKey_F1;
        case KEY_F2			:
            return GXKey_F2;
        case KEY_F3			:
            return GXKey_F3;
        case KEY_F4			:
            return GXKey_F4;
        case KEY_F5			:
            return GXKey_F5;
        case KEY_F6			:
            return GXKey_F6;
        case KEY_F7			:
            return GXKey_F7;
        case KEY_F8			:
            return GXKey_F8;
        case KEY_F9			:
            return GXKey_F9;
        case KEY_F10		:
            return GXKey_F10;
        case KEY_NUMLOCK	:
            return GXKey_NUM_LOCK;
        case KEY_SCROLLLOCK	:
            return GXKey_SCROLL_LOCK;
        case KEY_KP7		:
            return GXKey_KP_7;
        case KEY_KP8		:
            return GXKey_KP_8;
        case KEY_KP9		:
            return GXKey_KP_9;
        case KEY_KPMINUS	:
            return GXKey_MINUS;
        case KEY_KP4		:
            return GXKey_KP_4;
        case KEY_KP5		:
            return GXKey_KP_5;
        case KEY_KP6		:
            return GXKey_KP_6;
        case KEY_KPPLUS		:
            return GXKey_KP_ADD;
        case KEY_KP1		:
            return GXKey_KP_1;
        case KEY_KP2		:
            return GXKey_KP_2;
        case KEY_KP3		:
            return GXKey_KP_3;
        case KEY_KP0		:
            return GXKey_KP_0;
            
        case KEY_KPDOT :
            printf("Non mapped KEY_KPDOT\n");
            return (GXKeyCode)0;
            
        case KEY_UP:
            return GXKey_UP;
            
        case KEY_DOWN:
            return GXKey_DOWN;
        case KEY_LEFT:
            return GXKey_LEFT;
        case KEY_RIGHT:
            return GXKey_RIGHT;
        
        default:
            break;
    }
    return (GXKeyCode)0;
}



