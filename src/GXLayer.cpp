//
//  GXLayer.cpp
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>
#include <chrono>
#include <iostream>
#include <GXLayer.hpp>
#include "NG.h"




//static bool renderOnDemand(GXContext*, GXLayer*);

GXLayer::GXLayer( GXContext* context ,const GXSize &size):
_size(size),
_frameBuffer(nullptr),
_alpha( 1.f )
{

    if( context)
    {
        createFB(context);
    }
}

GXLayer::~GXLayer()
{
    deleteFB();
    
    
}


void GXLayer::setAlpha( int val) noexcept
{
    _alpha = val;
}

int GXLayer::getAlpha() const noexcept
{
    return _alpha;
}

const GXSize& GXLayer::getSize() const noexcept
{
    return _size;
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void GXLayer::deleteFB()
{
    if( _frameBuffer)
    {
        nvgluDeleteFramebuffer(_frameBuffer);
        _frameBuffer = nullptr;
    }
}

bool GXLayer::createFB( GXContext*ctx)
{
    if( _size.width <= 0 ||  _size.height <=0)
        return false;
    
    const int flag = 0;
    
    if( _frameBuffer)
        return true;

    _frameBuffer = nvgluCreateFramebuffer( static_cast<NVGcontext*>( ctx->_ctx ) , _size.width, _size.height, flag);
    assert( _frameBuffer );
    
    return _frameBuffer != nullptr;
    
}

void GXLayer::beginDraw( GXContext* context )
{
    nvgluBindFramebuffer( _frameBuffer);
    
    glViewport(0, 0, getSize().width, getSize().height);
    
    const GXSize fboSize = context->getImageSize( _frameBuffer->image );
    assert(fboSize  == getSize() );
    context->beginFrame( getSize(), 1.f);
    context->reset();
}

void GXLayer::endDraw( GXContext* context )
{
    context->endFrame();
    nvgluBindFramebuffer(NULL);
}

void GXLayer::test(GXContext* context , const GXColor &color)
{
    beginDraw( context );
    context->beginPath();
    context->addRect(GXRectMake(0, 0, getSize().width, getSize().height));
    context->setFillColor( color);
    context->fill();

    
    endDraw( context );
    
    
}

void GXLayer::draw( GXContext* context , const GXPoint& pt)
{
    if( !_frameBuffer)
        return;

    /*
    context->translate( pt );
    
    
    context->intersectScissor( bounds );
    */
    const GXRect bounds = GXRectMake( pt, getSize());
    const GXPaint imgFB = context->imagePattern( pt, getSize(), 0, _frameBuffer->image, _alpha );
    
    context->beginPath();
        context->addRect( bounds );
        context->setFillPainter(imgFB);
    context->fill();

    // draw children here
    /*
    context->resetScissor();
    context->resetTransform();
     */
}




