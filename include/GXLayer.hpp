//
//  GXLayer.hpp
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXLayer_hpp
#define GXLayer_hpp

#include <vector>
#include "GXContext.hpp"
#include "GXGeometry.hpp"
#include "GXColor.hpp"


struct NVGLUframebuffer;

class GXLayer
{
    friend class GXRenderer;
public:
    GXLayer();
    

    void update( GXContext* context , const GXRect& bounds);

    GXRect bounds;
    GXColor background;
    
    float getAlpha() const noexcept
    {
        return background.a;
    }
    
    void setOpaque( bool opaque) noexcept;
    
    /* */
    
    void setNeedsDisplay()
    {
        _needsDisplay = true;
        if( _parent)
        {
            _parent->setNeedsDisplay();
        }
    }
    
    /* */
    
    
    bool addChild( GXLayer* layer);
    bool removeChild( GXLayer* layer);
    
    bool hasParent() const noexcept
    {
        return _parent;
    }
    bool hasChildren() const noexcept
    {
        return !_children.empty();
    }
    
    const std::vector<GXLayer*>& getChildren() const
    {
        return _children;
    }
    
    // public temp, should go private
    void renderLayer(GXContext* context , float pxRatio );
    bool createFB( GXContext*ctx );
    

    
    int id;
protected:
    virtual ~GXLayer();
    
    virtual void paint( GXContext* context , const GXRect& bounds)
    {}
    
    
private:
    
    /**/
    bool _opaque;
    /**/
    
    NVGLUframebuffer* _fb;
    bool _needsDisplay;
    
    std::vector<GXLayer*> _children;
    
    GXLayer* _parent;
};

#endif /* GXLayer_hpp */
