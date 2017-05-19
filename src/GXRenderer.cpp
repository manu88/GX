//
//  GXRenderer.cpp
//  nanoVGTest3
//
//  Created by Manuel Deneu on 01/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>
#include <functional>
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
    
    
    //bool doneSomething = false;
    
    std::function<void (GXContext*, GXLayer*) > renderOnDemand = [&renderOnDemand](GXContext* ctx ,GXLayer* layer)
    {
        if( layer->_needsDisplay)
        {
            layer->renderLayer(ctx, 1.);
            layer->_needsDisplay = false;
        }
        for(GXLayer* c : layer->getChildren() )
        {
            renderOnDemand(ctx,c);
        }
        
    };
    
    renderOnDemand(context,_rootLayer);
    
    glViewport(0, 0, _rootLayer->bounds.size.width, _rootLayer->bounds.size.height);
    context->beginFrame(_rootLayer->bounds.size, 1.f);
        drawImage(_rootLayer, context , GXPointMakeNull() );
    context->endFrame();
    
}

void GXRenderer::drawImage(GXLayer* layer , GXContext* context , const GXPoint &accumPos)
{
    
    assert(layer->_fb);

    //printf("Draw Layer %i at %i %i \n" , layer->id , layer->bounds.origin.x , layer->bounds.origin.y);
    
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
            drawImage(c , context , accumPos +  layer->bounds.origin);
        }
    }
    
    
    
    
    context->resetScissor();
    context->resetTransform();
    
}

