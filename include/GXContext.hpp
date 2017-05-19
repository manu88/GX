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
#include "GXFont.hpp"

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


typedef enum   : int
{
    // Horizontal align
    GXTextAlign_LEFT 		= 1<<0,	// Default, align text horizontally to left.
    GXTextAlign_CENTER 	= 1<<1,	// Align text horizontally to center.
    GXTextAlign_RIGHT 	= 1<<2,	// Align text horizontally to right.
    // Vertical align
    GXTextAlign_TOP 		= 1<<3,	// Align text vertically to top.
    GXTextAlign_MIDDLE	= 1<<4,	// Align text vertically to middle.
    GXTextAlign_BOTTOM	= 1<<5,	// Align text vertically to bottom.
    GXTextAlign_BASELINE	= 1<<6, // Default, align text vertically to baseline.
} GXTextAlign;

    
static inline GXTextAlign operator|( GXTextAlign a , GXTextAlign b)
{
    return static_cast<GXTextAlign>( static_cast<int>(a) | static_cast<int>(b) );
}

typedef int GXImageHandle;
static const GXImageHandle GXImageInvalid = 0;



class GXContext
{
    friend class GXRenderer;
    friend class GXLayer;
    friend class GXFontManager;
public:
    GXContext();
    ~GXContext();
    
    bool beginFrame( const GXSize &size , float pixRatio) noexcept;
    bool endFrame() noexcept;
    
    void beginPath() noexcept;
    
    /* Fonts */
    
    void setFontSize( float) noexcept;
    
    void setFontId( GXFontHandle id) noexcept;
    
    /* Images */
    
    GXImageHandle createImage(const std::string& file , int flags) noexcept;
    
    GXPaint imagePattern( const GXPoint &c, const GXSize &size, float angle, GXImageHandle image, float alpha) noexcept;
    
    GXSize getImageSize( GXImageHandle image ) noexcept;
    
    /* Shapes */
    
    void addRoundedRect( const GXRect &rect , float rad) noexcept;
    void addRect( const GXRect &rect) noexcept;
    void addCircle( const GXPoint &center , float rad) noexcept;
    void addEllipse( const GXRect &r) noexcept;
    
    /* Text */
    
    void getTextSize( const GXPoint &p, float breakRowWidth, const std::string &str, GXSize &min , GXSize &max ) noexcept;
    void addTextBox( const GXPoint &p, float breakRowWidth, const std::string &str) noexcept;
    void setTextSize( float size) noexcept;
    void setTextAlignement( GXTextAlign align) noexcept;
    
    /* Paint */
    void setStrokeColor( const GXColor &color) noexcept;
    
    void setFillColor( const GXColor &color) noexcept;
    void setFillPainter( const GXPaint&p) noexcept;
    
    void fill() noexcept;
    void stroke() noexcept;
    
    /* Transforms / scissors */
    
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
    
    GXFontManager& getFontManager()
    {
        return _fontManager;
    }
protected:
    GXFontHandle createFont( const std::string &fontName) noexcept;
private:
    void* _ctx;
    
    GXFontManager _fontManager;
};





#endif /* GXContext_h */
