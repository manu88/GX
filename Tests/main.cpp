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

#include "GXTouchResponder.hpp"
#include "GXTouchHandler.hpp"

class C1 : public GXLayer
{
public:
    C1(const std::string &fileImg) :
    file(fileImg),
    imgH(-1)
    {}
    
    void paint( GXContext* context , const GXRect& bounds) override
    {
        context->beginPath();
        //nvgBeginPath(context->_ctx);
        //static int imgH = -1;
        
        if( imgH == -1)
        {
            imgH = context->createImage(file , 0);// nvgCreateImage(context->_ctx, file.c_str() , 0);
        }
        
        GXPaint imgPaint = context->imagePattern(GXPointMakeNull(), bounds.size, 0.0f/180.0f*M_PI, imgH, 1.f);
        //nvgImagePattern(context->_ctx, 0, 0, bounds.size.width , bounds.size.height, 0.0f/180.0f*NVG_PI, imgH, 1.f);
        
        context->addRoundedRect(GXRectMake(GXPointMakeNull(), bounds.size), 5);
        //nvgRoundedRect(context->_ctx, 0,0, bounds.size.width , bounds.size.height, 5);
        
        context->setFillPainter( imgPaint);
        //nvgFillPaint(context->_ctx, imgPaint);
        
        context->fill();
        //nvgFill(context->_ctx);
    }
    
    const std::string file;
    int imgH;
};

class CWin : public GXLayer , public GXTouchResponder
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
        //nvgBeginPath( context->_ctx );
        
        context->addRoundedRect(bounds, 5);
        //nvgRoundedRect(context->_ctx, bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height , 5);
        //nvgFillColor(context->_ctx, background);
        context->setFillColor(background);
        context->fill();
        //nvgFill( context->_ctx );
        
        
        const std::string fontName = "Roboto-Regular.ttf";
        
        int fontHandle = context->createFont(fontName);
        //nvgCreateFont( context->_ctx, fontName.c_str(), fontName.c_str());
        assert( fontHandle != -1);
        
        context->setFontId( fontHandle);
        //nvgFontFaceId( context->_ctx, fontHandle);
        
        //nvgFontSize(context->_ctx, 20.f);
        context->setFontSize(20.f);
        
        context->setFillColor( GXColors::Black );
        //nvgFillColor(context->_ctx, GXColors::Red);
        
        //const std::string &str = "Hello World";
        //nvgTextBox(context->_ctx, bounds.origin.x, bounds.origin.y, bounds.size.width-20, str.c_str(), NULL);
        
        context->addTextBox(GXPointMake(20, 20), bounds.size.width-20, str);
        //nvgTextBox(context->_ctx, 20 , 20, bounds.size.width-20, str.c_str(), NULL);
        
        //nvgText(context->_ctx , bounds.origin.x, bounds.origin.y , str.c_str() , NULL);
        
        
    }
    std::string str;
};


static CWin* mainWidget = nullptr;
static CWin* imgWidget = nullptr;
static GXContext* context = nullptr;
static GXRenderer* renderer = nullptr;


GXTouchHandler _touchHandler;

static void renderScreen( GXRenderer *render , Display* disp , GXContext *ctx)
{
    
    //glClearColor(0.0, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

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
                
                mainWidget->str = buf;
                mainWidget->setNeedsDisplay();
                renderer->renderLayer( context, mainWidget, 1.f);
                
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
            
            _touchHandler.onEvent(evt);
            /*
            const GXEventMouse* mouse = (const GXEventMouse*) evt;
            
            if( mouse->state == GXMouseStatePressed)
            {
                printf("Mouse button %i state %i at (%f,%f) \n" , mouse->button , mouse->state , mouse->x , mouse->y);
            
                assert(imgWidget);
                imgWidget->bounds.origin = GXPointMake( mouse->x , mouse->y);
            }
             */
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
        
        mainWidget = &mainLayer;
        imgWidget = &t1;
        renderer = &render;
        context = &ctx;
        
        mainLayer.bounds = GXRectMake(0, 0, winWidth, winHeight);
        mainLayer.background = GXColors::LightGray;
        
        _touchHandler.setFirstResponder( &mainLayer);
        
        render.setRoot(&mainLayer);
        mainLayer.addChild(&t1);
        t1.addChild(&t2);
        mainLayer.addChild(&t3);
        //mainLayer.addChild(&t2);
        
        t1.background = GXColorMake(0.5, 0.5, 0 , 0.5);
        t1.bounds.size = GXSizeMake(200, 200);
        t1.bounds.origin = GXPointMake(100, 100);
        
        
        t2.bounds.size = GXSizeMake(200, 200);
        t2.bounds.origin = GXPointMake(100, 100);
        
        t3.bounds.size = GXSizeMake(300, 300);
        t3.bounds.origin = GXPointMake(200, 150);
        
        
        
        render.renderLayer(&ctx, &mainLayer, pxRatio);
        render.renderLayer(&ctx, &t1, pxRatio);
        render.renderLayer(&ctx, &t2, pxRatio);
        render.renderLayer(&ctx, &t3, pxRatio);

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
        /*
        GB::FDSource input(fileno(stdin));
        input.notification = [&](GBRunLoopSourceNotification notif)
        {
            if( notif == GBRunLoopSourceCanRead)
            {
                static char buf[128];
                if(input.read(buf, 128))
                {
                    printf("Read '%s' \n" , buf);
                }
            }
        };
        
        runL.addSource(input);
         */
        /**/

        runL.run();
    }
    
    DisplayRelease(&disp);
	
	return 0;
}
