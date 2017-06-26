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
#include "GXText.hpp"

typedef struct
{
    enum Winding
    {
        GX_CCW = 1,			// Winding for solid shapes
        GX_CW = 2,				// Winding for holes
    };
    
    float xform[6];
    float extent[2];
    float radius;
    float feather;
    GXColor innerColor;
    GXColor outerColor;
    int image;
} GXPaint;





typedef enum : int
{
    GXimageFlags_None               = 0,
    GXimageFlags_GENERATE_MIPMAPS	= 1<<0,     // Generate mipmaps during creation of the image.
    GXimageFlags_REPEATX			= 1<<1,		// Repeat image in X direction.
    GXimageFlags_REPEATY			= 1<<2,		// Repeat image in Y direction.
    GXimageFlags_FLIPY				= 1<<3,		// Flips (inverses) image in Y direction when rendered.
    GXimageFlags_PREMULTIPLIED		= 1<<4,		// Image data has premultiplied alpha.
    GXimageFlags_NEAREST			= 1<<5,		// Image interpolation is Nearest instead Linear
} GXimageFlags;


typedef int GXImageHandle;
static const GXImageHandle GXImageInvalid = 0;


struct NVGtextRow;

class GXTextContainer;

class GXContext
{
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
    
    GXImageHandle createImage(const std::string& file , GXimageFlags flags) noexcept;
    GXImageHandle createImage( const char* data , size_t dataSize , GXimageFlags flags) noexcept;
    void deleteImage( GXImageHandle img) noexcept;
    
    GXPaint imagePattern( const GXPoint &c, const GXSize &size, float angle, GXImageHandle image, float alpha) noexcept;
    GXPaint linearGradient(float sx, float sy, float ex, float ey, const GXColor& icol, const GXColor& ocol);
    
    
    GXSize getImageSize( GXImageHandle image ) noexcept;
    
    /* Shapes */
    
    void addRoundedRect( const GXRect &rect , float rad) noexcept;
    void addRect( const GXRect &rect) noexcept;
    void addCircle( const GXPoint &center , float rad) noexcept;
    void addEllipse( const GXRect &r) noexcept;
    void addArc( float cx, float cy, float r, float a0, float a1, int dir);
    
    void moveTo( const GXPoint &p) noexcept;
    void lineTo( const GXPoint &p) noexcept;
    void closePath() noexcept;
    
    /* Text */
    
    int textBreakLines( const char* string, const char*  end, float breakRowWidth, NVGtextRow* rows, int maxRows);
    
    
    void addText( const GXPoint &p, const std::string &string);
    void addText( const GXPoint &p, const char* string, const char* end);
    void addText( const GXPoint &p , const GXTextContainer& text);
    void getTextSize( const GXPoint &p, float breakRowWidth, const std::string &str, GXSize &min , GXSize &max ) noexcept;
    void addTextBox( const GXPoint &p, float breakRowWidth, const std::string &str) noexcept;
    void setTextSize( float size) noexcept;
    void setTextAlignement( GXTextAlign align) noexcept;
    
    void getTextMetrics(float* ascender, float* descender, float* lineh);
    
    /* Paint */
    void setStrokeColor( const GXColor &color) noexcept;
    void setStrokeWidth( float w) noexcept;
    
    
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
    
    void reset() noexcept;
    
    GXFontManager& getFontManager()
    {
        return _fontManager;
    }
    
    bool isValid() const noexcept
    {
        return _ctx;
    }
    void* getImpl()
    {
        return _ctx;
    }
    
protected:
    GXFontHandle createFont( const std::string &fontName) noexcept;
private:
    void* _ctx;
    
    GXFontManager _fontManager;

};





#endif /* GXContext_h */
