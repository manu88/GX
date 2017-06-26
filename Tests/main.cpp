#include <stdio.h>
#include <assert.h>
#include <string>

#include <GroundBase.hpp>

#ifdef USE_DISPMAN
#include "GLES/gl.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#elif defined USE_GLFW
#include <GLFW/glfw3.h>
#endif

#include "Display.h"
#include "GX.hpp"



int main()
{

    Display disp;

    if( DisplayInit(&disp , 1000 , 800) == 0)
    {
        printf("Display init error \n");
        return -1;
    }

    int winWidth, winHeight;
    int fbWidth, fbHeight;
    float pxRatio;

    if (!disp._handle)
    {
        return -1;
    }
    
    DisplayMakeContextCurrent( &disp );

    GXContext ctx;
        
#ifdef USE_GLFW
    assert(DisplayGetType(&disp) == DisplayGLFW);
#elif defined USE_DISPMAN
    assert(DisplayGetType(&disp) == DisplayDispman);
#endif
        
    DisplayGetWindowSize( &disp, &winWidth, &winHeight);
    DisplayGetFramebufferSize(&disp, &fbWidth, &fbHeight);
    pxRatio = (float)fbWidth / (float)winWidth;
    
    GB::RunLoop runL;
    
    /**/
    
    GB::Timer t;
    t.setInterval(40);
    t.setCallback([&](GB::Timer &timer)
    {
        
        DisplaySwap( &disp );
        DisplayPollEvents( &disp );
        
        if( DisplayShouldClose( &disp ))
        {
            runL.stop();
        }

    });
    
    runL.addSource(t);

    runL.run();

    
   
    DisplayRelease(&disp);
	
	return 0;
}
