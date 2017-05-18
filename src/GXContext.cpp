//
//  GXContext.c
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>
#include "GXContext.hpp"



GXContext::GXContext() : _ctx(nullptr)
{
#ifdef USE_GLFW
    _ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#elif defined USE_DISPMAN
    _ctx = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif
    
}

GXContext::~GXContext()
{
#ifdef USE_GLFW
    nvgDeleteGL3( _ctx );
#elif defined USE_DISPMAN
    nvgDeleteGLES2( _ctx);
#endif
}



bool GXContext::beginFrame( const GXSize &size , float pixRatio) noexcept
{
    assert(_ctx);
    if( _ctx)
    {
        nvgBeginFrame( _ctx, size.width, size.height, pixRatio);
        return true;
    }
    return false;
}
bool GXContext::endFrame() noexcept
{
    assert(_ctx);
    if( _ctx)
    {
        nvgEndFrame(_ctx);
        return true;
    }
    
    return false;
}

