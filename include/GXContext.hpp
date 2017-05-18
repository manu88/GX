//
//  GXContext.h
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXContext_h
#define GXContext_h


#include <string>
#include "GXGeometry.hpp"
#include "GXColor.hpp"




typedef struct
{
    float xform[6];
    float extent[2];
    float radius;
    float feather;
    GXColor innerColor;
    GXColor outerColor;
    int image;
} GXPaint;


typedef int GXImageHandle;
static const GXImageHandle GXImageInvalid = 0;

typedef int GXFontHandle;
static const GXFontHandle GXFontInvalid = -1;

class GXContext
{
    friend class GXRenderer;
public:
    GXContext();
    ~GXContext();
    
    bool beginFrame( const GXSize &size , float pixRatio) noexcept;
    bool endFrame() noexcept;
    
    void beginPath() noexcept;
    
    /**/
    
    void setFontSize( float) noexcept;
    GXFontHandle createFont( const std::string &fontName) noexcept;
    void setFontId( GXFontHandle id) noexcept;
    
    /**/
    
    GXImageHandle createImage(const std::string& file , int flags) noexcept;
    
    GXPaint imagePattern( const GXPoint &c, const GXSize &size, float angle, GXImageHandle image, float alpha) noexcept;
    
    
    /**/
    
    void addRoundedRect( const GXRect &rect , float rad) noexcept;
    void addRect( const GXRect &rect) noexcept;
    
    void addTextBox( const GXPoint &p, float breakRowWidth, const std::string &str) noexcept;
    
    void setFillColor( const GXColor &color) noexcept;
    void setFillPainter( const GXPaint&p) noexcept;
    
    void fill() noexcept;
    void stroke() noexcept;
    
    /**/
    
    void translate( const GXPoint &p) noexcept;
    void resetTransform() noexcept;
    
    /**/
    // Sets the current scissor rectangle.
    // The scissor rectangle is transformed by the current transform.
    void scissor( const GXRect &r) noexcept;
    
    // Intersects current scissor rectangle with the specified rectangle.
    // The scissor rectangle is transformed by the current transform.
    // Note: in case the rotation of previous scissor rect differs from
    // the current one, the intersection will be done between the specified
    // rectangle and the previous scissor rectangle transformed in the current
    // transform space. The resulting shape is always rectangle.
    void intersectScissor( const GXRect &r) noexcept;
    
    // Reset and disables scissoring.
    void resetScissor() noexcept;
    
private:
    void* _ctx;
};





#endif /* GXContext_h */
