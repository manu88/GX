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
    int8_t x,y;
    if(read(fdMouse, &ie, sizeof(struct input_event)))
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
                 keyEv.code     = (GXKeyCode)kVal;
                 keyEv.mod      = (GXKeyMod)0;
                 keyEv.scanCode = 0;
                 
                 disp->eventListener((void*)disp , (const GXEvent*) &keyEv);
                 
                
            }
            
        }

    return 1;
}
