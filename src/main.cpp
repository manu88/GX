#include <stdio.h>
#include <assert.h>
#include <vector>
#include <functional>


#include "Display.h"


//#include "NG.h"
#include "GXRenderer.hpp"
#include "GXContext.hpp"
#include "GXLayer.hpp"
#include "GXColor.hpp"


class C1 : public GXLayer
{
public:
    C1() : imgH(-1)
    {}
    
    void paint( GXContext* context , const GXRect& bounds) override
    {
        nvgBeginPath(context->_ctx);
        //static int imgH = -1;
        
        if( imgH == -1)
        {
            imgH = nvgCreateImage(context->_ctx, "images/image1.jpg", 0);
        }
        
        NVGpaint imgPaint = nvgImagePattern(context->_ctx, 0, 0, bounds.size.width , bounds.size.height, 0.0f/180.0f*NVG_PI, imgH, 1.f);
        
        nvgRoundedRect(context->_ctx, 0,0, bounds.size.width , bounds.size.height, 5);
        nvgFillPaint(context->_ctx, imgPaint);
        nvgFill(context->_ctx);
    }
    
    int imgH;
};

class CWin : public GXLayer
{
public:
    CWin()
    {
        background =  GXColors::DarkGray;
    }
    
    void paint( GXContext* context , const GXRect& bounds) override
    {

        nvgBeginPath( context->_ctx );
        
        nvgRoundedRect(context->_ctx, bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height , 5);
        nvgFillColor(context->_ctx, background);
        nvgFill( context->_ctx );
    }
};

int main()
{
    GXRenderer render;
    GXContext ctx;
    Display disp;
    {
        
        CWin mainLayer;
        C1 t1;
        CWin t2;
        CWin t3;

        mainLayer.background = GXColors::Red;
        mainLayer.bounds = GXRectMake(0, 0, 800, 400);
        mainLayer.background = GXColors::LightGray;
        render.setRoot(&mainLayer);
        
        t1.bounds.size = GXSizeMake(200, 200);
        t2.bounds.size = GXSizeMake(500, 400);
        
        t3.bounds.size = GXSizeMake(200, 600);
        t3.background = GXColors::Green;
        t3.bounds.origin = GXPointMake(100, 100);

        t2.addChild(&t1);
        t2.addChild(&t3);

        if( DisplayInit(&disp) == 0)
        {
            printf("Display init error \n");
            return -1;
        }
        
        double prevt = 0;
        
        int winWidth, winHeight;
        int fbWidth, fbHeight;
        float pxRatio;


        if (!disp._handle)
        {
            return -1;
        }

        DisplayMakeContextCurrent( &disp );

        
        if( GXContextInit(&ctx) == 0)
        {
            printf("Unable to init GXContext\n");
            return -2;
        }
        DisplayGetWindowSize( &disp, &winWidth, &winHeight);
        DisplayGetFramebufferSize( &disp , &fbWidth, &fbHeight);
        
        // Calculate pixel ration for hi-dpi devices.
        pxRatio = (float)fbWidth / (float)winWidth;

        float t = 0;
        
        
    #ifdef USE_GLFW
        assert(DisplayGetType(&disp) == DisplayGLFW);
    #elif defined USE_DISPMAN
        assert(DisplayGetType(&disp) == DisplayDispman);
    #endif
        
        
        mainLayer.setNeedsDisplay();
        
        
        t1.setNeedsDisplay();
        t3.setNeedsDisplay();
        
        
        render.renderLayer(&ctx, &mainLayer, pxRatio);
        
        while (!DisplayShouldClose( &disp ))
        {
            
            
            double mx, my;

            t+=0.01;

            prevt = t;
            
            
            DisplayGetWindowSize( &disp, &winWidth, &winHeight);
            DisplayGetFramebufferSize(&disp, &fbWidth, &fbHeight);
            // Calculate pixel ration for hi-dpi devices.
            pxRatio = (float)fbWidth / (float)winWidth;

            
            
            
            
            // Update and render
            glViewport(0, 0, fbWidth, fbHeight);
            glClearColor(0.0, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

            nvgBeginFrame(ctx._ctx, winWidth, winHeight, pxRatio);
            
            render.drawImage( &mainLayer , &ctx);
            

            nvgEndFrame(ctx._ctx);

            DisplaySwap(&disp);
            DisplayPollEvents(&disp);
        }

        //nvgluDeleteFramebuffer(fb);
    
    }
    GXContextRelease(&ctx);
    DisplayRelease(&disp);
	
	return 0;
}
