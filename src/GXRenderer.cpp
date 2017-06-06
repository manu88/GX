//
//  GXRenderer.cpp
//  nanoVGTest3
//
//  Created by Manuel Deneu on 01/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>
#include <chrono>
#include <iostream>
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
    _rootLayer->setNeedsRedraw();
}



void GXRenderer::initView(GXContext* context)
{
    glViewport(0, 0, _rootLayer->getBounds().size.width, _rootLayer->getBounds().size.height);
}


bool GXRenderer::renderOnDemand(GXContext* ctx, GXLayer* layer)
{
    bool doneSomething = false;
    
    
    if( layer->_currentAnim)
    {
        //layer->processAnimations();
    }
    printf("Traverse layer '%s' - " , layer->identifier.c_str());
    if( layer->_needsRedraw )
    {

        
        auto start = std::chrono::steady_clock::now();
        
        layer->renderLayer(ctx, 1.);
        
        auto diff = std::chrono::steady_clock::now() - start;
        
        std::cout << "Layer " << layer->identifier << " " <<  std::chrono::duration <double,std::milli> (diff).count() << " ms" << std::endl;
        
        
        
        doneSomething = true;
        layer->_needsRedraw = false;
        layer->setNeedsDisplay();
        
        
    }
    else
    {
        printf("\n");
    }
    if( layer->_needsDisplay)
    {
        doneSomething  = true;
    }
    
    for(GXLayer* c : layer->getChildren() )
    {
        if( renderOnDemand(ctx,c))
        {
            doneSomething = true;
        }
    }

    
    
    return doneSomething;
}

bool GXRenderer::draw( GXContext* context)
{
    
    if( !_rootLayer)
        return false;
    
    
    std::cout << "---- Render Start --- \n";

    
    bool doneSomething = renderOnDemand(context,_rootLayer);
    
    if( doneSomething)
    {
        glViewport(0, 0, _rootLayer->getBounds().size.width, _rootLayer->getBounds().size.height);
        context->beginFrame(_rootLayer->bounds.size, 1.f);
        drawImage(_rootLayer, context , GXPointMakeNull() );
        context->endFrame();
    }
    
    std::cout << "---- Render End --- \n";
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
    
    if( 1)//layer->_needsDisplay)//layer->_needsRedraw)// layer->_needsDisplay)// || layer->_childNeedsDisplay)
    {
        //printf("Draw layer %p\n" , (void*) layer);
        const GXPaint imgFB = context->imagePattern(layer->bounds.origin, layer->bounds.size, 0, layer->_fb->image, layer->getAlpha());
        
        context->beginPath();

        context->addRect(layer->bounds);
        
        context->setFillPainter(imgFB);
        //context->setStrokeColor(GXColors::Green);
        context->fill();
        //context->stroke();
        
        layer->_needsDisplay = false;
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

