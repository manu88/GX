//
//  GXContext.c
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>
#include "GXContext.hpp"
#include "NG.h"


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
    nvgDeleteGL3( static_cast<NVGcontext*>( _ctx ) );
#elif defined USE_DISPMAN
    nvgDeleteGLES2( static_cast<NVGcontext*>( _ctx ) );
#endif
}



bool GXContext::beginFrame( const GXSize &size , float pixRatio) noexcept
{
    assert(_ctx);
    if( _ctx)
    {
        nvgBeginFrame( static_cast<NVGcontext*>( _ctx ), size.width, size.height, pixRatio);
        return true;
    }
    return false;
}
bool GXContext::endFrame() noexcept
{
    assert(_ctx);
    if( _ctx)
    {
        nvgEndFrame( static_cast<NVGcontext*>( _ctx ) );
        return true;
    }
    
    return false;
}

void GXContext::beginPath() noexcept
{
    nvgBeginPath(static_cast<NVGcontext*>( _ctx ) );
}

void GXContext::addRoundedRect( const GXRect &rect , float rad) noexcept
{
    nvgRoundedRect( static_cast<NVGcontext*>( _ctx ) , rect.origin.x, rect.origin.y , rect.size.width, rect.size.height, rad);
}

void GXContext::addTextBox( const GXPoint &p, float breakRowWidth, const std::string &str) noexcept
{
    nvgTextBox( static_cast<NVGcontext*>( _ctx ), p.x, p.y, breakRowWidth, str.c_str() , NULL);
}

void GXContext::setFillColor( const GXColor &color) noexcept
{
    nvgFillColor( static_cast<NVGcontext*>( _ctx ) , nvgRGBAf( color.r , color.g , color.b , color.a ) );
}

void GXContext::setFillPainter( const GXPaint&p) noexcept
{
    
    const NVGpaint *pp = reinterpret_cast<const NVGpaint *>(&p);
    
    nvgFillPaint( static_cast<NVGcontext*>( _ctx ) , *pp );
}

void GXContext::fill() noexcept
{
    //nvgFillPaint(<#NVGcontext *ctx#>, <#NVGpaint paint#>)
    nvgFill(static_cast<NVGcontext*>( _ctx ) );
}

void GXContext::stroke() noexcept
{
    nvgStroke(static_cast<NVGcontext*>( _ctx ) );
}

void GXContext::setFontSize( float size ) noexcept
{
    nvgFontSize( static_cast<NVGcontext*>( _ctx ), size);
}

int GXContext::createFont( const std::string &fontName) noexcept
{
    return nvgCreateFont( static_cast<NVGcontext*>( _ctx ), fontName.c_str(), fontName.c_str());
}

void GXContext::setFontId( int id) noexcept
{
    nvgFontFaceId( static_cast<NVGcontext*>( _ctx ), id);
}


int GXContext::createImage(const std::string& file , int flags) noexcept
{
    return nvgCreateImage( static_cast<NVGcontext*>( _ctx ) , file.c_str(), flags);
}

GXPaint GXContext::imagePattern( const GXPoint &c, const GXSize &size, float angle, int image, float alpha) noexcept
{
    /*
     xform[6];
     extent[2];
     radius;
     feather;
     GXColor innerColor;
     GXColor outerColor;
     int image;
     */
    
    NVGpaint p = nvgImagePattern( static_cast<NVGcontext*>( _ctx ), c.x, c.y, size.width, size.height, angle, image, alpha);
    
    return{
        {p.xform[0] ,p.xform[1] , p.xform[2] , p.xform[3] , p.xform[4] , p.xform[5] },
        {p.extent[0] , p.extent[1]},
        p.radius,
        p.feather,
        GXColorMake(p.innerColor.r, p.innerColor.g, p.innerColor.b , p.innerColor.a),
        GXColorMake(p.outerColor.r, p.outerColor.g, p.outerColor.b , p.outerColor.a),
        p.image
    };
}
