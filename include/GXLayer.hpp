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
    virtual ~GXLayer();

    void update( GXContext* context , const GXRect& bounds);

    
    GXColor background;
    
    
    void setVisible( bool vis) noexcept;
    bool isVisible() const noexcept
    {
        return _visible;
    }
    
    float getAlpha() const noexcept
    {
        return _opaque? 1.f :  background.a;
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
    
    const GXRect& getBounds() const noexcept
    {
        return bounds;
    }
    
    void setBounds( const GXRect& b) noexcept
    {
        bounds = b;
    }
    
    void setCenter( const GXPoint &p) noexcept;
    void setPos( const GXPoint &p) noexcept
    {
        bounds.origin = p;
    }
    
    const GXPoint &getPos() const noexcept
    {
        return bounds.origin;
    }
    
    const GXSize& getSize() const noexcept
    {
        return bounds.size;
    }
    
    void setSize( const GXSize &s) noexcept
    {
        bounds.size = s;
    }
    
    GXPoint getCenter() const noexcept
    {
        return getPos() + (getSize()/2);
    }
    
    bool addChild( GXLayer* layer);
    bool removeChild( GXLayer* layer);
    
    
    bool removeFromParent();
    bool hasParent() const noexcept
    {
        return _parent;
    }
    
    const GXLayer* getParent() const noexcept
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
    
    
    virtual void paint( GXContext* context , const GXRect& bounds)
    {}
    
    
private:
    GXRect bounds;
    /**/
    bool _opaque;
    bool _visible;
    /**/
    
    NVGLUframebuffer* _fb;
    bool _needsDisplay;
    
    std::vector<GXLayer*> _children;
    
    GXLayer* _parent;
};

#endif /* GXLayer_hpp */
