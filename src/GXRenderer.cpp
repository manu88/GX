//
//  GXRenderer.cpp
//  nanoVGTest3
//
//  Created by Manuel Deneu on 01/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>
#include "GXRenderer.hpp"
#include "GXLayer.hpp"
#include "NG.h"

GXRenderer::GXRenderer():
_rootLayer(nullptr)
{
    
}

GXRenderer::~GXRenderer()
{
    
}

void GXRenderer::setRoot( GXLayer* layer)
{
    if (layer == nullptr)
        return;
    
    _rootLayer = layer;
    _rootLayer->setNeedsDisplay();
}

void GXRenderer::renderPass( GXContext* context)
{
    
}

void GXRenderer::draw( GXContext* context)
{
    if( !_rootLayer)
        return;
    
    
    if( _rootLayer->_needsDisplay)
    {
        _rootLayer->renderLayer(context, 1.);
        _rootLayer->_needsDisplay = false;
    }
    if( _rootLayer->hasChildren())
    {
        for(GXLayer* c : _rootLayer->getChildren() )
        {
            if( c->_needsDisplay)
            {
                c->renderLayer(context, 1.);
                c->_needsDisplay = false;
            }
        }
    }
    
    /*
    if( !_rootLayer->_needsDisplay)
        return;
    */
    glViewport(0, 0, _rootLayer->bounds.size.width, _rootLayer->bounds.size.height);
    context->beginFrame(_rootLayer->bounds.size, 1.f);
        drawImage(_rootLayer, context , GXPointMakeNull() );
    context->endFrame();
    
}

void GXRenderer::drawImage(GXLayer* layer , GXContext* context , const GXPoint &accumPos)
{
    /*
    if( !layer->_fb)
    {
        layer->renderLayer(context, 1.f);
    }
     */
    assert(layer->_fb);
/*
    if( layer->_fb == nullptr)
    {
        createFB(context, layer);
    }
*/
    
    const GXPaint imgFB = context->imagePattern(layer->bounds.origin, layer->bounds.size, 0, layer->_fb->image, layer->getAlpha());
    
    context->beginPath();
    
    context->translate(accumPos);
    
    context->intersectScissor(layer->bounds);
    
    context->addRect(layer->bounds);
    
    context->setFillPainter(imgFB);
    
    context->fill();
    
    if( layer->hasChildren())
    {
        for(GXLayer* c : layer->getChildren() )
        {
            //context->beginFrame(layer->bounds.size, 1.f);
            drawImage(c , context , accumPos +  layer->bounds.origin);
            //context->endFrame();
        }
    }
    
    
    
    
    context->resetScissor();
    context->resetTransform();
    
}

