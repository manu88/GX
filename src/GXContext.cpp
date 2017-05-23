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


GXContext::GXContext() :
_ctx(nullptr),
_fontManager(this)

{
#ifdef USE_GLFW
    _ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#elif defined USE_DISPMAN
    _ctx = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif
    assert(_ctx);
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

void GXContext::addRect( const GXRect &rect) noexcept
{
    nvgRect( static_cast<NVGcontext*>( _ctx ) , rect.origin.x, rect.origin.y , rect.size.width, rect.size.height );
}

void GXContext::addCircle( const GXPoint &center , float rad) noexcept
{
    nvgCircle( static_cast<NVGcontext*>( _ctx ), center.x, center.y, rad);
}

void GXContext::addEllipse( const GXRect &r) noexcept
{
    nvgEllipse( static_cast<NVGcontext*>( _ctx ),
               r.origin.x + (r.size.width/2),
               r.origin.y + (r.size.height/2),
               r.size.width/2,
               r.size.height/2);
}

void GXContext::addArc( float cx, float cy, float r, float a0, float a1, int dir)
{
    nvgArc( static_cast<NVGcontext*>( _ctx ), cx, cy, r, a0, a1, dir);
}

void GXContext::moveTo( const GXPoint &p) noexcept
{
    nvgMoveTo( static_cast<NVGcontext*>( _ctx ) , p.x, p.y);
}

void GXContext::lineTo( const GXPoint &p) noexcept
{
    nvgLineTo( static_cast<NVGcontext*>( _ctx ) , p.x, p.y);
}

void GXContext::closePath() noexcept
{
    
    nvgClosePath( static_cast<NVGcontext*>( _ctx ) );
}

void GXContext::addTextBox( const GXPoint &p, float breakRowWidth, const std::string &str) noexcept
{
    nvgTextBox( static_cast<NVGcontext*>( _ctx ), p.x, p.y, breakRowWidth, str.c_str() , NULL);
}

void GXContext::getTextMetrics(float* ascender, float* descender, float* lineh)
{
    nvgTextMetrics( static_cast<NVGcontext*>( _ctx ), ascender, descender, lineh);
}

void GXContext::addText( const GXPoint &p ,const char* string, const char* end)
{
    nvgText(static_cast<NVGcontext*>( _ctx ), p.x, p.y, string, end);
}
void GXContext::addText( const GXPoint &p , const GXTextContainer& text)
{
    
}

int GXContext::textBreakLines( const char* string, const char*  end, float breakRowWidth, NVGtextRow* rows, int maxRows)
{
    return nvgTextBreakLines(static_cast<NVGcontext*>( _ctx ), string, end, breakRowWidth, rows, maxRows);
}

void GXContext::getTextSize( const GXPoint &p, float breakRowWidth, const std::string &str, GXSize &min , GXSize &max ) noexcept
{
    
    //[xmin,ymin, xmax,ymax]
    float r[4] = { -1.f};
    
    nvgTextBoxBounds(static_cast<NVGcontext*>( _ctx ), p.x, p.y, breakRowWidth, str.c_str() , NULL, r);
    
    min = GXSizeMake(r[0], r[1]);
    max = GXSizeMake(r[2], r[3]);
    
}

void GXContext::setTextSize( float size) noexcept
{
    nvgFontSize( static_cast<NVGcontext*>( _ctx ), size);
}

void GXContext::setTextAlignement( GXTextAlign align) noexcept
{
    nvgTextAlign(static_cast<NVGcontext*>( _ctx ), align);
}

void GXContext::setStrokeColor( const GXColor &color) noexcept
{
    nvgStrokeColor( static_cast<NVGcontext*>( _ctx ) , nvgRGBAf( color.r , color.g , color.b , color.a ) );
}

void GXContext::setStrokeWidth( float w) noexcept
{
    nvgStrokeWidth(static_cast<NVGcontext*>( _ctx ), w);
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

GXFontHandle GXContext::createFont( const std::string &fontName) noexcept
{
    return nvgCreateFont( static_cast<NVGcontext*>( _ctx ), fontName.c_str(), fontName.c_str());
}

void GXContext::setFontId(GXFontHandle id) noexcept
{
    nvgFontFaceId( static_cast<NVGcontext*>( _ctx ), id);
}

GXImageHandle GXContext::createImage(const std::string& file , int flags) noexcept
{
    return nvgCreateImage( static_cast<NVGcontext*>( _ctx ) , file.c_str(), flags);
}

void GXContext::deleteImage( GXImageHandle img) noexcept
{
    nvgDeleteImage( static_cast<NVGcontext*>( _ctx ), img );
}

GXSize GXContext::getImageSize( GXImageHandle image ) noexcept
{
    int w = -1;
    int h = -1;
    nvgImageSize( static_cast<NVGcontext*>( _ctx ) , image, &w, &h);
    
    return GXSizeMake(w, h);
}

GXPaint GXContext::imagePattern( const GXPoint &c, const GXSize &size, float angle, GXImageHandle image, float alpha) noexcept
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
    
    const NVGpaint p = nvgImagePattern( static_cast<NVGcontext*>( _ctx ), c.x, c.y, size.width, size.height, angle, image, alpha);
    
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

GXPaint GXContext::linearGradient(float sx, float sy, float ex, float ey, const GXColor& icol, const GXColor& ocol)
{
    const NVGpaint p = nvgLinearGradient(static_cast<NVGcontext*>( _ctx ), sx, sy, ex, ey,
                                         nvgRGBAf(icol.r, icol.g, icol.b, icol.a),
                                         nvgRGBAf(ocol.r, ocol.g, ocol.b, ocol.a)
                                         );
    
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

void GXContext::translate( const GXPoint &p) noexcept
{
    nvgTranslate( static_cast<NVGcontext*>( _ctx ) , p.x, p.y);
}

void GXContext::resetTransform() noexcept
{
    nvgResetTransform( static_cast<NVGcontext*>( _ctx ) );
}

void GXContext::scissor( const GXRect &r) noexcept
{
    nvgScissor( static_cast<NVGcontext*>( _ctx ), r.origin.x, r.origin.y, r.size.width, r.size.height);
}

void GXContext::intersectScissor( const GXRect &r) noexcept
{
    nvgIntersectScissor( static_cast<NVGcontext*>( _ctx ), r.origin.x, r.origin.y, r.size.width, r.size.height);
}

void GXContext::resetScissor() noexcept
{
    nvgResetScissor( static_cast<NVGcontext*>( _ctx ) );
}

void GXContext::reset() noexcept
{
    nvgReset(static_cast<NVGcontext*>( _ctx ));
}


