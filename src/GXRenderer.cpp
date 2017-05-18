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
    
    /*
    if( !_rootLayer->_needsDisplay)
        return;
    */
    context->beginFrame(_rootLayer->bounds.size, 1.f);
        drawImage(_rootLayer, context);
    context->endFrame();
    
}

void GXRenderer::drawImage(GXLayer* layer , GXContext* context)
{

    if( layer->_fb == nullptr)
    {
        createFB(context, layer);
    }
    /*
    if (layer->_needsDisplay)
    {
        printf("Layer %p needs display \n" , (void*) layer);
        renderLayer( context, layer, 1.f);
    }
    */
    /*
    if( layer != _rootLayer )
    {
        assert(layer->_parent);
        
        nvgluBindFramebuffer(layer->_parent->_fb);
    }
    */
    NVGcontext* ctx = static_cast<NVGcontext*>( context->_ctx );
    NVGpaint imgFB = nvgImagePattern(ctx ,
                                     layer->bounds.origin.x,
                                     layer->bounds.origin.y,
                                     layer->bounds.size.width,
                                     layer->bounds.size.height,
                                     0,
                                     layer->_fb->image,
                                     layer->getAlpha() );
    nvgBeginPath( ctx );
    
    
    nvgTranslate(ctx, layer->_parent? layer->_parent->bounds.origin.x : 0 , layer->_parent? layer->_parent->bounds.origin.y : 0);
    
    nvgIntersectScissor(ctx, layer->bounds.origin.x,
     layer->bounds.origin.y ,
     layer->bounds.size.width,
     layer->bounds.size.height);
    
    //printf("Draw Image at %i %i \n" , layer->bounds.origin.x, layer->bounds.origin.y);
    nvgRect( ctx,
            layer->bounds.origin.x,
            layer->bounds.origin.y ,
            layer->bounds.size.width,
            layer->bounds.size.height);
    
    nvgFillPaint( ctx, imgFB);
    nvgFill( ctx);
    
    
    if( layer->hasChildren())
    {
        for(GXLayer* c : layer->getChildren() )
        {
            drawImage(c , context);
        }
        //printf("Layer has children image to draw\n");
    }
    
    
    layer->_needsDisplay = false;
    
    //nvgluBindFramebuffer(NULL);
}

bool GXRenderer::createFB( GXContext*ctx , GXLayer* l )
{
    const int flag = 0; // NVG_IMAGE_GENERATE_MIPMAPS //NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY);
    
    if( l->_fb)
        return true;
    
    
    
    l->_fb = nvgluCreateFramebuffer( static_cast<NVGcontext*>( ctx->_ctx ) , l->bounds.size.width, l->bounds.size.height, flag);
    assert(l->_fb);
    
    return l->_fb != nullptr;
    
};

void GXRenderer::renderLayer(GXContext* vg, GXLayer* layer,  float pxRatio )
{
    int winWidth, winHeight;
    int fboWidth, fboHeight;
    
    if (layer->_fb == NULL)
    {
        if( !createFB(vg, layer))
        {
            assert(false);
        }
    }

    NVGcontext* ctx = static_cast<NVGcontext*>( vg->_ctx );
    
    nvgImageSize( ctx, layer->_fb->image, &fboWidth, &fboHeight);
    assert(fboWidth == layer->bounds.size.width);
    assert(fboHeight == layer->bounds.size.height);
    
    winWidth = (int)(fboWidth / pxRatio);
    winHeight = (int)(fboHeight / pxRatio);

    nvgluBindFramebuffer(layer->_fb);
    
    glViewport(0, 0, fboWidth, fboHeight);
    //glClearColor(0, 0, 0, 0);
    //glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    nvgBeginFrame( ctx, winWidth, winHeight, pxRatio);
    
    const GXRect bounds = GXRectMake(GXPointMakeNull(), GXSizeMake(winWidth, winHeight));
    layer->update(vg, bounds);
    
    
    nvgEndFrame( ctx);
    nvgluBindFramebuffer(NULL);
}
