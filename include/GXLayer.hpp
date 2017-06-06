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
#include <string>
#include "GXContext.hpp"
#include "GXGeometry.hpp"
#include "GXColor.hpp"


struct NVGLUframebuffer;

class GXAnimation;



class GXLayer
{
    friend class GXRenderer;
public:
    
    GXLayer();
    virtual ~GXLayer();

    
    void run(  GXAnimation* );
    //void update( GXContext* context , const GXRect& bounds);

    /* Color/apparence*/
    
    GXColor background;

    void setVisible( bool vis) noexcept;
    bool isVisible() const noexcept
    {
        return _visible;
    }
    
    void setAlpha( float v) noexcept;
    
    float getAlpha() const noexcept
    {
        return _opaque? 1.f :  background.a;
    }
    
    void setOpaque( bool opaque) noexcept;

    void setNeedsRedraw();
    bool needsRedraw() const noexcept;
    
    void setNeedsDisplay() noexcept;
    
    /* Geometry */
    
    
    // 1 atop 0
    void setZPos( int ) noexcept;
    int  getZPos() const noexcept;
    
    const GXRect& getBounds() const noexcept
    {
        return bounds;
    }
    
    virtual void setBounds( const GXRect& b) noexcept;

    void setCenter( const GXPoint &p) noexcept;
    void setPos( const GXPoint &p) noexcept;
    void setSize( const GXSize &s) noexcept;
    
    const GXPoint &getPos() const noexcept
    {
        return bounds.origin;
    }
    
    const GXSize& getSize() const noexcept
    {
        return bounds.size;
    }

    GXPoint getCenter() const noexcept
    {
        return getPos() + (getSize()/2);
    }
    
    /* Hierarchy */
    
    virtual bool addChild( GXLayer* layer);
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
    
    /* */
    
    // public temp, should go private
    void renderLayer(GXContext* context , float pxRatio );
    bool createFB( GXContext*ctx );
    void deleteFB();

    std::string identifier;
protected:
    
    
    virtual void paint( GXContext* context , const GXRect& bounds)
    {}
    
    
    

private:
    
    void processAnimations();
    void sizeChanged();
    void sortChildren();
    GXRect bounds;
    int _zOrder;
    /**/
    bool _opaque;
    bool _visible;
    /**/
    
    NVGLUframebuffer* _fb;
    bool _needsRedraw; // will call paint method
    bool _needsDisplay;
    
    
    
    std::vector<GXLayer*> _children;
    
    GXLayer* _parent;
    
    GXAnimation *_currentAnim;
};

#endif /* GXLayer_hpp */
