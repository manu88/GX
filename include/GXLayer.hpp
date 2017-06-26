//
//  GXLayer.hpp
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXLayer_hpp
#define GXLayer_hpp


#include "GXContext.hpp"
#include "GXGeometry.hpp"
#include "GXColor.hpp"


struct NVGLUframebuffer;



class GXLayer
{
public:
    
    GXLayer( GXContext* context, const GXSize &size);
    ~GXLayer();

    const GXSize& getSize() const noexcept;
    int getAlpha() const noexcept;
    void setAlpha( int) noexcept;



    bool isValid() const noexcept
    {
        return _frameBuffer;
    }
    
    void test(GXContext* context , const GXColor &color);
    
    void beginDraw( GXContext* );
    void draw( GXContext* context , const GXPoint& pt);
    void endDraw( GXContext* );
    
    
    
protected:

private:
    bool createFB( GXContext*ctx );
    void deleteFB();
    
    GXSize _size;
    NVGLUframebuffer* _frameBuffer;
    float _alpha;
};

#endif /* GXLayer_hpp */
