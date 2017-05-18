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
#include "GXRenderer.hpp"
#include "GXContext.hpp"
#include "GXLayer.hpp"
#include "GXColor.hpp"




static GXContext* context = nullptr;

class C1 : public GXLayer
{
public:
    C1(const std::string &fileImg) :
    file(fileImg),
    imgH( GXImageInvalid )
    {}
    
    void changeImage()
    {
        static int c = 1;
        file = std::string("images/image")+ std::to_string(c) +std::string(".jpg");
        c++;
        if( c>10)
        {
            c = 1;
        }
        imgH = GXImageInvalid;
        setNeedsDisplay();
        //renderLayer(context, 1.f);
        
    }
    
    void paint( GXContext* context , const GXRect& bounds) override
    {
        context->beginPath();
        //nvgBeginPath(context->_ctx);
        //static int imgH = -1;
        
        if( imgH == GXImageInvalid )
        {
            imgH = context->createImage(file , 0);// nvgCreateImage(context->_ctx, file.c_str() , 0);
        }
        
        GXPaint imgPaint = context->imagePattern(GXPointMakeNull(), bounds.size, 0, imgH, 1.f);
        
        context->addRoundedRect(GXRectMake(GXPointMakeNull(), bounds.size), 5);
        
        context->setFillPainter( imgPaint);
        
        context->fill();
    }
    
    std::string file;
    GXImageHandle imgH;
};

class CWin : public GXLayer
{
public:
    CWin()
    {
        str = "These are the most commonly overridden methods for views but you may not need to override all of them. If you use gesture recognizers to handle events, you do not need to override any of the event-handling methods. Similarly, if your view does not contain subviews or its size does not change, there is no reason to override the layoutSubviews method. Finally, the drawRect: method is needed only when the contents of your view can change at runtime and you are using native technologies such as UIKit or Core Graphics to do your drawing. It is also important to remember that these are the primary integration points but not the only ones. Several methods of the UIView class are designed to be override points for subclasses. You should look at the method descriptions in UIView Class Reference to see which methods might be appropriate for you to override in your custom implementations.";
        background =  GXColors::DarkGray;
    }
    
    void paint( GXContext* context , const GXRect& bounds) override
    {

        context->beginPath();

        const std::string fontName = "Roboto-Regular.ttf";
        
        GXFontHandle fontHandle = context->createFont(fontName);

        assert( fontHandle != GXFontInvalid);
        
        context->setFontId( fontHandle);

        context->setFontSize(20.f);
        
        context->setFillColor( GXColors::Black );

        context->addTextBox(GXPointMake(20, 20), bounds.size.width-20, str);

    }
    std::string str;
};


static CWin* mainWidget = nullptr;
static CWin* imgWidget = nullptr;

static GXRenderer* renderer = nullptr;




static void renderScreen( GXRenderer *render , Display* disp , GXContext *ctx)
{
    
    render->draw( ctx );
    DisplaySwap( disp );
    //DisplayWaitEvents( disp );
    DisplayPollEvents( disp );
    
}


static void eventListener(void* d , const GXEvent *evt)
{
    assert(d);
    assert(evt);
    
    Display* disp =(Display* ) d;
    assert(disp);
    
    
    switch (evt->type)
    {
        case GXEventTypeKey:
        {
            const GXEventKey* key = (const GXEventKey*) evt;
            assert(key);
            
            if( key->action == GXKeyAction_Press)
            {
                static std::string buf;
                
                const char* b = GXKeyGetChar(key);
                if( b)
                {
                    if( key->code == GXKey_ENTER)
                    {
                        
                        buf.clear();
                    }
                    else
                    {
                        buf.push_back(b[0]);
                        
                    }
                }
                else if( key->code == GXKey_BACKSPACE)
                {
                    
                    buf.pop_back();// erase(buf.end());
                }
                else
                {
                    printf("Unkown char %i \n" , key->code);
                }
                printf("'%s'\n" , buf.c_str() );
                assert(mainWidget);
                
                imgWidget->str = buf;
                imgWidget->setNeedsDisplay();
                //imgWidget->renderLayer(context, 1.f);
                //renderer->renderLayer( context, mainWidget, 1.f);
                
                /*
                assert(renderer);
                assert(context);
                
                widget->str = buf;
                renderer->renderLayer( context, widget, 1.f);
                 */
                
            }
        }
            break;
            
        case GXEventTypeMouse:
        {

            const GXEventMouse* mouse = (const GXEventMouse*) evt;
            
            if( mouse->state == GXMouseStatePressed)
            {
                printf("Mouse button %i state %i at (%f,%f) \n" , mouse->button , mouse->state , mouse->x , mouse->y);
            
                assert(imgWidget);
                imgWidget->bounds.origin = GXPointMake( mouse->x , mouse->y);
            }
            
            break;
        }
            
        default:
            assert(false);
            break;
    }
}

int main()
{
    GXRenderer render;
    
    Display disp;
    {
        
        /**/
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

        DisplayGetWindowSize( &disp, &winWidth, &winHeight);
        DisplayGetFramebufferSize( &disp , &fbWidth, &fbHeight);
        
        DisplaySetEventCallback(&disp, eventListener);
        
        // Calculate pixel ration for hi-dpi devices.
        pxRatio = (float)fbWidth / (float)winWidth;
        
        GXContext ctx;
        
#ifdef USE_GLFW
        assert(DisplayGetType(&disp) == DisplayGLFW);
#elif defined USE_DISPMAN
        assert(DisplayGetType(&disp) == DisplayDispman);
#endif
        
        /**/
        
        CWin mainLayer;
        CWin t1;//("images/image1.jpg");
        C1 t2("images/image2.jpg");
        C1 t3("images/image5.jpg");
        
        mainLayer.id = 0;
        t1.id = 1;
        t2.id = 2;
        t3.id = 3;
        
        mainWidget = &mainLayer;
        imgWidget = &t1;
        renderer = &render;
        context = &ctx;
        
        mainLayer.bounds = GXRectMake(0, 0, winWidth, winHeight);
        mainLayer.background = GXColors::LightGray;
                
        render.setRoot(&mainLayer);
        mainLayer.addChild(&t1);
        t1.addChild(&t2);
        mainLayer.addChild(&t3);
        //mainLayer.addChild(&t2);
        
        t1.background = GXColorMake(0.5, 0.5, 0 , 0.5);
        t1.bounds.size = GXSizeMake(200, 200);
        t1.bounds.origin = GXPointMake(500, 200);
        
        
        t2.bounds.size = GXSizeMake(200, 200);
        t2.bounds.origin = GXPointMake(100, 100);
        
        t3.bounds.size = GXSizeMake(300, 300);
        t3.bounds.origin = GXPointMake(200, 150);
        
        /*
        mainLayer.renderLayer(&ctx, pxRatio);
        t1.renderLayer(&ctx, pxRatio);
        t2.renderLayer(&ctx, pxRatio);
        t3.renderLayer(&ctx, pxRatio);
        */
         
        DisplayGetWindowSize( &disp, &winWidth, &winHeight);
        DisplayGetFramebufferSize(&disp, &fbWidth, &fbHeight);
        pxRatio = (float)fbWidth / (float)winWidth;
        
        glViewport(0, 0, fbWidth, fbHeight);
        
        
        GB::RunLoop runL;
        
        /**/
        
        GB::Timer t;
        t.setInterval(40);
        t.setCallback([&](GB::Timer &timer)
        {
            /*
            GLint defaultFBO = -1;
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
            assert(defaultFBO == 0);
            */
            renderScreen(&render , &disp , &ctx);
            
            if( DisplayShouldClose( &disp ))
            {
                runL.stop();
            }
        });
        
        runL.addSource(t);
        /**/
        
        GB::Timer tTest;
        tTest.setInterval(1000);
        tTest.setCallback([&](GB::Timer &timer)
        {
            t3.changeImage();
        });
        runL.addSource(tTest);

        /**/
        /*
        GB::Timer animTime;
        animTime.setInterval(50);
        animTime.setCallback([&t2](GB::Timer &timer)
        {
            t2.bounds.origin += GXPointMake(10, 10);
            if( t2.bounds.origin.y > 600)
            {
                t2.bounds.origin = GXPointMake(10, 10);
            }
        });
        
        runL.addSource(animTime);
        */
        /**/
        
        GB::FDSource input(fileno(stdin));
        input.notification = [&](GBRunLoopSourceNotification notif)
        {
            if( notif == GBRunLoopSourceCanRead)
            {
                static char buf[128];
                const GBSize rLen = input.read(buf, 128);
                if(rLen)
                {
                    buf[rLen-1] = 0;
                    printf("Read '%s' \n" , buf);
                    
                    imgWidget->str = buf;
                    imgWidget->setNeedsDisplay();
                }
            }
        };
        
        runL.addSource(input);
        
        /**/

        runL.run();
    }
    
    DisplayRelease(&disp);
	
	return 0;
}
