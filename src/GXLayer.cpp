//
//  GXLayer.cpp
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>

#include "GXLayer.hpp"
#include "NG.h"
#include "GXAnimation.hpp"

GXLayer::GXLayer():

background(GXColors::Black),
bounds(GXRectNull),
_zOrder(0),
_opaque(true),
_visible(true),
_fb(nullptr),
_needsRedraw(false),
_needsDisplay(false),

_parent(nullptr),
_currentAnim(nullptr)
{

}

GXLayer::~GXLayer()
{
    deleteFB();
    
    
}

bool GXLayer::addChild( GXLayer* layer)
{
    if( layer ==nullptr || layer == this)
        return false;
    
    _children.push_back(layer);
    layer->_parent = this;
    sortChildren();
    layer->setNeedsRedraw();
    return true;
}

bool GXLayer::removeFromParent()
{
    if( _parent)
    {
        return _parent->removeChild(this);
    }
    return false;
}

bool GXLayer::removeChild( GXLayer* layer)
{
    if( !layer)
        return false;
    
    _children.erase(std::remove( _children.begin(), _children.end(), layer), _children.end());
    
    layer->_parent = nullptr;
    sortChildren();
    return true;
}

void GXLayer::setZPos( int pos ) noexcept
{
    _zOrder = pos;
}
int GXLayer::getZPos() const noexcept
{
    return _zOrder;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void GXLayer::sizeChanged()
{
    deleteFB();
}

void GXLayer::setBounds( const GXRect& b) noexcept
{
    const GXSize _last = bounds.size;
    
    bounds = b;
    if( _last !=b.size)
    {
        sizeChanged();
    }
}

void GXLayer::setCenter( const GXPoint &p) noexcept
{
    setPos( GXPointMake(p.x-(bounds.size.width/2), p.y-(bounds.size.height/2) ) );
}

void GXLayer::setPos( const GXPoint &p) noexcept
{
    setBounds(GXRectMake(p, bounds.size));

}
void GXLayer::setSize( const GXSize &s) noexcept
{
    setBounds(GXRectMake(bounds.origin, s));
    //bounds.size = s;
    //geometryChanged();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void GXLayer::setVisible( bool vis) noexcept
{
    _visible = vis;
}

void GXLayer::setAlpha( float v) noexcept
{
    background.a = v;
}

void GXLayer::setOpaque( bool opaque) noexcept
{
    _opaque = opaque;
}

void GXLayer::setNeedsRedraw()
{
    _needsRedraw = true;
}

bool GXLayer::needsRedraw() const noexcept
{
    return _needsRedraw;
}

void GXLayer::setNeedsDisplay() noexcept
{
    _needsDisplay = true;
    
    if( _parent)
    {
        _parent->setNeedsDisplay();
    }
}

void GXLayer::deleteFB()
{
    if( _fb)
    {
        nvgluDeleteFramebuffer(_fb);
        _fb = nullptr;
    }
}

bool GXLayer::createFB( GXContext*ctx)
{
    const int flag = 0;
    
    if( _fb)
        return true;

    _fb = nvgluCreateFramebuffer( static_cast<NVGcontext*>( ctx->_ctx ) , bounds.size.width, bounds.size.height, flag);
    assert( _fb );
    
    return _fb != nullptr;
    
};

void GXLayer::renderLayer(GXContext* context ,  float pxRatio )
{
    
    assert(needsRedraw());
    
    if( bounds.size == GXSizeNull )
        return ;
    
    int winWidth, winHeight;
    //int fboWidth, fboHeight;
    
    if ( _fb == nullptr)
    {
        if( !createFB(context))
        {
            assert(false);
        }
    }
    
    //NVGcontext* ctx = static_cast<NVGcontext*>( context->_ctx );
    
    const GXSize fboSize = context->getImageSize( _fb->image );
    assert(fboSize  == bounds.size );
    
    
    winWidth = (int)(fboSize.width / pxRatio);
    winHeight = (int)(fboSize.height / pxRatio);
    
    
    nvgluBindFramebuffer( _fb);
    
    glViewport( 0 ,0 , bounds.size.width, bounds.size.height);
    
    if( _opaque)
    {
        glClearColor(background.r, background.g, background.b, background.a);
        glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    }
    
    context->beginFrame(GXSizeMake(winWidth, winHeight), pxRatio);
    
    const GXRect bounds = GXRectMake(GXPointMakeNull(), GXSizeMake(winWidth, winHeight));
    //update(context, bounds);
    
    
    context->reset();
    context->beginPath();
    paint(context, bounds);
    
    
    //_needsDisplay = false;
    
    context->endFrame();
    
    nvgluBindFramebuffer(NULL);
    
}

struct layer_comparor
{
    bool operator() (const GXLayer* lhs, const GXLayer* rhs) const
    {
        return lhs->getZPos() > rhs->getZPos();
    }
};

void GXLayer::sortChildren()
{
    std::sort(_children.begin() , _children.end(), layer_comparor() );
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void GXLayer::run(GXAnimation*  anim)
{
    if( anim)
    {
        _currentAnim = anim;
        _currentAnim->layer = this;
    }
}

void GXLayer::processAnimations()
{
    assert(_currentAnim);
    
    if(_currentAnim->update() == false)
    {
        printf("Anim Done\n");
        delete _currentAnim;
        _currentAnim = nullptr;
    }
    
}
