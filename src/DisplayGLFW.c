//
//  Display.c
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>
#include <stdio.h>
#include "Display.h"


#include <GLFW/glfw3.h>




static void GLFWerrorcb(int error, const char* desc);

int DisplayInit( Display *disp)
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
        
        disp->_handle = glfwCreateWindow(1000, 600, "NanoVG", NULL, NULL);
        
        disp->type = DisplayGLFW;
        return disp->_handle != NULL;
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
}

void DisplayWaitEvents( const Display *disp)
{
    glfwWaitEvents();
}

static void GLFWerrorcb(int error, const char* desc)
{
    printf("GLFW error %d: %s\n", error, desc);
}

int DisplayGetCursorPos( const Display* disp, double* x, double* y)
{
    assert(disp);
    assert(disp->_handle);
    glfwGetCursorPos(disp->_handle, x, y);
    return glfwGetMouseButton(disp->_handle, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
}
