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
}



void GXRenderer::draw( GXContext* context)
{
    drawImage(_rootLayer, context);
}

void GXRenderer::drawImage(GXLayer* layer , GXContext* context)
{
    if( layer->_fb == nullptr)
    {
        createFB(context, layer);
    }
    NVGpaint imgFB = nvgImagePattern(context->_ctx,
                                     layer->bounds.origin.x,
                                     layer->bounds.origin.y,
                                     layer->bounds.size.width,
                                     layer->bounds.size.height,
                                     0,
                                     layer->_fb->image,
                                     layer->getAlpha() );
    nvgBeginPath( context->_ctx );
    /*
     nvgScissor(ctx._ctx, l->bounds.origin.x,
     l->bounds.origin.y ,
     l->bounds.size.width-100,
     l->bounds.size.height-100);
     */
    printf("Draw Image at %i %i \n" , layer->bounds.origin.x, layer->bounds.origin.y);
    nvgRect(context->_ctx,
            layer->bounds.origin.x,
            layer->bounds.origin.y ,
            layer->bounds.size.width,
            layer->bounds.size.height);
    
    nvgFillPaint( context->_ctx, imgFB);
    nvgFill( context->_ctx);
    
    if( layer->hasChildren())
    {
        for(GXLayer* c : layer->getChildren() )
        {
            drawImage(c , context);
        }
        printf("Layer has children image to draw\n");
    }
     
}

bool GXRenderer::createFB( GXContext*ctx , GXLayer* l )
{
    const int flag = 0; // NVG_IMAGE_GENERATE_MIPMAPS //NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY);
    
    if( l->_fb)
        return true;
    
    l->_fb = nvgluCreateFramebuffer( ctx->_ctx, l->bounds.size.width, l->bounds.size.height, flag);
    assert(l->_fb);
    
    return l->_fb != nullptr;
    
};

void GXRenderer::renderLayer(GXContext* vg, GXLayer* layer,  float pxRatio )
{
    int winWidth, winHeight;
    int fboWidth, fboHeight;
    
    if (layer->_fb == NULL)
    {
        printf("Create FB for %p \n",(void*) this);
        
        if( !createFB(vg, layer))
        {
            assert(false);
        }
    }
    
    nvgImageSize(vg->_ctx, layer->_fb->image, &fboWidth, &fboHeight);
    winWidth = (int)(fboWidth / pxRatio);
    winHeight = (int)(fboHeight / pxRatio);

    nvgluBindFramebuffer(layer->_fb);
    glViewport(0, 0, fboWidth, fboHeight);
    glClearColor(0, 0, 0, 0);
    //glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    nvgBeginFrame(vg->_ctx, winWidth, winHeight, pxRatio);
    
    const GXRect bounds = GXRectMake(GXPointMakeNull(), GXSizeMake(winWidth, winHeight));
    layer->update(vg, bounds);
    
    /*
    if( layer->hasChildren() )
    {
        
        for (GXLayer* c : layer->getChildren())
        {
            printf("Layer %p has Child %p to draw\n" , (void*)layer , (void*)c);
            
            const GXRect p = GXRectMake(GXPointMakeNull(), c->bounds.size);
            //const GXRect p = c->bounds + bounds.origin;
            printf("F : %i %i %i %i : C %i %i %i %i \n" ,
                   bounds.origin.x , bounds.origin.y , bounds.size.width , bounds.size.height ,
                   p.origin.x , p.origin.y , p.size.width , p.size.height
                   );
            
            //renderLayer(vg, c, pxRatio);
            c->update(vg, c->bounds);
        }
    }
    */
    nvgEndFrame(vg->_ctx);
    nvgluBindFramebuffer(NULL);
}
