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
#include "GXAnimation.hpp"



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

bool GXRenderer::draw( GXContext* context)
{
    if( !_rootLayer)
        return false;
    
    
    bool doneSomething = false;
    
    
    
    std::function<void (GXContext*, GXLayer*) > renderOnDemand = [&doneSomething, &renderOnDemand](GXContext* ctx ,GXLayer* layer)
    {
        if( layer->_currentAnim)
        {
            layer->processAnimations();
        }
        if( layer->needsDisplay())
        {
            layer->renderLayer(ctx, 1.);
            //layer->_needsDisplay = false;
            doneSomething = true;
        }
        for(GXLayer* c : layer->getChildren() )
        {
            renderOnDemand(ctx,c);
        }
        
    };
    
    renderOnDemand(context,_rootLayer);
    
    //if( doneSomething)
    {
        glViewport(0, 0, _rootLayer->bounds.size.width, _rootLayer->bounds.size.height);
        context->beginFrame(_rootLayer->bounds.size, 1.f);
        drawImage(_rootLayer, context , GXPointMakeNull() );
        context->endFrame();
    }
    
    return doneSomething;
}

void GXRenderer::drawImage(GXLayer* layer , GXContext* context , const GXPoint &accumPos)
{
    if( !layer->isVisible())
        return;
    
    if( layer->bounds.size == GXSizeNull)
    {
        assert(layer->_fb == nullptr);
        return;
        
    }
    assert(layer->_fb);
    
    context->translate(accumPos);
    
    context->intersectScissor(layer->bounds);
    
    if( layer->_needsDisplay)
    {
        const GXPaint imgFB = context->imagePattern(layer->bounds.origin, layer->bounds.size, 0, layer->_fb->image, layer->getAlpha());
        
        context->beginPath();
        
        
        
        context->addRect(layer->bounds);
        
        context->setFillPainter(imgFB);
        //context->setStrokeColor(GXColors::Green);
        context->fill();
        //context->stroke();
        //layer->_needsDisplay = false;
        printf("Draw Layer %p \n" , (void*) layer);
    }
    
    if( layer->hasChildren())
    {
        for (auto rit = layer->_children.rbegin(); rit!= layer->_children.rend(); ++rit)
        {
            GXLayer* c  =  (*rit);
            context->resetTransform();
            drawImage(c , context , accumPos +  layer->bounds.origin);
        }
    }

    context->resetScissor();
    context->resetTransform();
    
}

