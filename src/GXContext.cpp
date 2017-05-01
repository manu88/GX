//
//  GXContext.c
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>
#include "GXContext.hpp"


int GXContextInit( GXContext* ctx)
{
#ifdef USE_GLFW
    ctx->_ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#elif defined USE_DISPMAN
    ctx->_ctx = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif
    
    return ctx->_ctx != NULL;
}

int GXContextRelease( GXContext* ctx)
{
#ifdef USE_GLFW
    nvgDeleteGL3( ctx->_ctx );
#elif defined USE_DISPMAN
    nvgDeleteGLES2( ctx->_ctx);
#endif
    return 1;
}

