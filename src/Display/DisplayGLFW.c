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

#include <GLFW/glfw3.h>

#include "Display.h"
#include "GXKey.h"


static void GLFWerrorcb(int error, const char* desc);

int DisplayInit( Display *disp ,int width , int height)
{
    assert(disp);
    
    if( disp)
    {
        disp->type = DisplayUnknown;
        if (!glfwInit())
        {
            printf("Failed to init GLFW.");
            return 0;
        }
        glfwSetErrorCallback(GLFWerrorcb);
#ifndef _WIN32 // don't require this on win32, and works with more cards
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
        
#ifdef DEMO_MSAA
        glfwWindowHint(GLFW_SAMPLES, 4);
#endif
        
        disp->_handle = glfwCreateWindow(width, height, "", NULL, NULL);
        
        assert(disp->_handle);
        glfwSetWindowUserPointer(disp->_handle, disp);
        disp->type = DisplayGLFW;
        
        disp->eventListener = NULL;
        
        disp->_usr = NULL;
        return disp->_handle != NULL;
        
        //glfwSetInputMode(disp->_handle, GLFW_CURSOR,  GLFW_CURSOR_HIDDEN );
        /*
        GLFWimage *img = malloc(sizeof(GLFWimage));
        img->height = 1;
        img->width = 1;
        img->pixels = malloc(4);
        memset( img->pixels , 0 , 4);
        
        GLFWcursor* cur = glfwCreateCursor(img, 0, 0);
        
        glfwSetCursor(disp->_handle, cur);
        */
    }
    return 0;
}
int DisplayRelease( Display *disp)
{
    assert(disp);
    if( disp->_handle)
    {
        glfwDestroyWindow(disp->_handle);
    }
    glfwTerminate();
    return 1;
}

static void keyFun(GLFWwindow* win,int key ,int scan,int action ,int mod)
{
    assert(win);
    Display* disp = glfwGetWindowUserPointer(win);
    assert(disp);
    assert(disp->eventListener);
    
    GXEventKey keyEv;
    keyEv.type = GXEventTypeKey;
    
    keyEv.action   = action;
    keyEv.code     = key;
    keyEv.mod      = mod;
    keyEv.scanCode = scan;
    
    disp->eventListener(disp , (const GXEvent*) &keyEv);
    
}

static void mouseButtonFun(GLFWwindow* win,int button,int action ,int mods)
{
    assert(win);
    Display* disp = glfwGetWindowUserPointer(win);
    assert(disp);
    assert(disp->eventListener);
    
    //GLFW_RELEASE                0
    //GLFW_PRESS                  1
    GXEventMouse mouseEv;
    mouseEv.type = GXEventTypeMouse;
    
    if( action == GLFW_PRESS)
    {
        mouseEv.state = GXMouseStatePressed;
    }
    else if( action == GLFW_RELEASE)
    {
        mouseEv.state = GXMouseStateReleased;
    }
    
    mouseEv.button = button;
    
    double x = 0;
    double y = 0;
    glfwGetCursorPos(win, &x, &y);
    mouseEv.x = (float) x;
    mouseEv.y = (float) y;

    disp->eventListener(disp , (const GXEvent*) &mouseEv);
    
}

void DisplaySetEventCallback(Display* disp , GXEventListener callback)
{
    assert(disp);
    disp->eventListener = callback;
    
    glfwSetKeyCallback(disp->_handle, keyFun);
    
    glfwSetMouseButtonCallback(disp->_handle, mouseButtonFun);
}

void DisplayMakeContextCurrent( Display *disp)
{
    assert(disp);
    assert(disp->_handle);
    glfwMakeContextCurrent(disp->_handle);
}

void DisplaySwap( const Display* disp)
{
    assert(disp);
    assert(disp->_handle);
    glfwSwapBuffers(disp->_handle);

}

void DisplayGetWindowSize( const Display* disp, int* width, int* height)
{
    assert(disp);
    assert(disp->_handle);
    glfwGetWindowSize(disp->_handle, width, height);
}

void DisplayGetFramebufferSize( const Display* disp, int* width, int* height)
{
    assert(disp);
    assert(disp->_handle);
    glfwGetFramebufferSize(disp->_handle, width, height);
    
}

int DisplayShouldClose( const Display* disp)
{
    assert(disp);
    assert(disp->_handle);
    return glfwWindowShouldClose(disp->_handle);
}

void DisplaySetShouldClose( Display* disp , int value)
{
    assert(disp);
    assert(disp->_handle);
    glfwSetWindowShouldClose(disp->_handle, value);
}

void DisplayPollEvents( const Display *disp)
{
    glfwPollEvents();
    
    if( disp->eventListener)
    {
        static lastX = 0;
        static lastY = 0;
        double x = 0;
        double y = 0;

        if(1)// glfwGetMouseButton(disp->_handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            glfwGetCursorPos(disp->_handle, &x, &y);

            if( x != lastX && y != lastY)
            {

                lastX = x;
                lastY = y;
                
                GXEventMouse mouseEv;
                mouseEv.type = GXEventTypeMouse;
                mouseEv.state = GXMouseStateMoving;
                mouseEv.x = (float) x;
                mouseEv.y = (float) y;
                disp->eventListener(disp , &mouseEv);
            }
        }
    }
}

void DisplayWaitEvents( const Display *disp)
{
    glfwWaitEvents();
}

static void GLFWerrorcb(int error, const char* desc)
{
    printf("GLFW error %d: %s\n", error, desc);
    assert(0);
}

int DisplayGetCursorPos( const Display* disp, double* x, double* y)
{
    assert(disp);
    assert(disp->_handle);
    glfwGetCursorPos(disp->_handle, x, y);
    return glfwGetMouseButton(disp->_handle, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
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

const char* GXKeyGetChar( const GXEventKey* key)
{
    assert(key);
    
    /*
    if( key->scanCode | GXKeyMod_SHIFT)
        printf("Shift\n");
     */
    switch (key->code)
    {
        case GXKey_SPACE:
            return " ";
        
        case GXKey_ENTER:
            return "\n";
            
        case GXKey_TAB:
            return "\t";
            
        default:
            return glfwGetKeyName(key->code, key->scanCode);
            break;
    }
    
    return NULL;
}
