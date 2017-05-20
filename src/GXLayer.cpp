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


GXLayer::GXLayer():

background(GXColors::Black),
bounds(GXRectNull),
_opaque(true),
_fb(nullptr),
_needsDisplay(false),
_parent(nullptr)
{

}

GXLayer::~GXLayer()
{
    if( _fb)
        nvgluDeleteFramebuffer(_fb);
}

bool GXLayer::addChild( GXLayer* layer)
{
    if( layer ==nullptr || layer == this)
        return false;
    
    _children.push_back(layer);
    layer->_parent = this;
    layer->setNeedsDisplay();
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
    return true;
}

void GXLayer::update( GXContext* context , const GXRect& bounds)
{

    if( _needsDisplay)
    {
        context->reset();
        context->beginPath();
        paint(context, bounds);
    }
    
    _needsDisplay = false;
}

void GXLayer::setCenter( const GXPoint &p) noexcept
{
    bounds.origin = GXPointMake(p.x-(bounds.size.width/2), p.y-(bounds.size.height/2));
}

void GXLayer::setOpaque( bool opaque) noexcept
{
    _opaque = opaque;
}



bool GXLayer::createFB( GXContext*ctx)
{
    const int flag = 0; // NVG_IMAGE_GENERATE_MIPMAPS //NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY);
    
    if( _fb)
        return true;
    
    
    
    _fb = nvgluCreateFramebuffer( static_cast<NVGcontext*>( ctx->_ctx ) , bounds.size.width, bounds.size.height, flag);
    assert( _fb );
    
    return _fb != nullptr;
    
};

void GXLayer::renderLayer(GXContext* context ,  float pxRatio )
{
    int winWidth, winHeight;
    //int fboWidth, fboHeight;
    
    if ( _fb == nullptr)
    {
        if( !createFB(context))
        {
            assert(false);
        }
    }
    
    NVGcontext* ctx = static_cast<NVGcontext*>( context->_ctx );
    
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
    update(context, bounds);
    
    context->endFrame();
    
    nvgluBindFramebuffer(NULL);
    
}
