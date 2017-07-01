//
//  GXDisplay.cpp
//  GX
//
//  Created by Manuel Deneu on 22/06/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <GXDisplay.hpp>
#include <GXContext.hpp>

#include <Display.h>

#ifdef USE_DISPMAN
#include "GLES/gl.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#elif defined USE_GLFW
#include <GLFW/glfw3.h>
#endif

#include "NG.h"

GXDisplay::GXDisplay():
onEvent(nullptr),
_impl(nullptr)
{
    
}

GXDisplay::~GXDisplay()
{
    assert(_impl);
    DisplayRelease( _impl);
    
    free(_impl);
    _impl = nullptr;
}

bool GXDisplay::init( const GXSize &size)
{
    _impl =(Display*) malloc(sizeof(Display));
    
    if( DisplayInit(_impl , size.width , size.height) == 0)
    {
        printf("Display init error \n");
        return false;
    }
    DisplayMakeContextCurrent( _impl );
    DisplaySetUserContext(_impl, this);
    DisplaySetEventCallback(_impl,  [](const void* disp , const GXEvent *evt)
    {
        const Display* display = static_cast<const Display*>(disp);
        Display*d = const_cast<Display*>(display);
        
        GXDisplay* self = static_cast<GXDisplay*>( DisplayGetUserContext( d ));
        assert(self);
        
        if( self->onEvent)
        {
            self->onEvent(evt);
        }
    });
    
    return true;
}

GXSize GXDisplay::getSize() const noexcept
{
    assert(_impl);
    
    int w = -1;
    int h = -1;
    DisplayGetWindowSize(_impl, &w, &h);
    
    return GXSizeMake(w, h);
}


void GXDisplay::beginDraw(GXContext* context)
{
    assert(_impl);
    
    nvgluBindFramebuffer( nullptr); // just here to match GXLayer's beginDraw method
    glViewport(0, 0, getSize().width, getSize().height );
    
    //const GXSize fboSize = context->getImageSize( _frameBuffer->image );
    //assert(fboSize  == getSize() );
    context->beginFrame( getSize(), 1.f);
    context->reset();
    
}
void GXDisplay::endDraw(GXContext* ctx)
{
    assert(_impl);
    
    ctx->endFrame();
}

void GXDisplay::clear() const
{
    assert(_impl);
    DisplayClear( _impl);
}

void GXDisplay::swap() const
{
    assert(_impl);
    DisplaySwap(_impl);
}


bool GXDisplay::shouldClose() const
{
    return DisplayShouldClose(_impl);
}

void GXDisplay::pollEvents() const
{
    DisplayPollEvents( _impl);
}
void GXDisplay::waitEvents() const
{
    DisplayWaitEvents(_impl);
}
