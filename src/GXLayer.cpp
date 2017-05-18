//
//  GXLayer.cpp
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include "GXLayer.hpp"
#include "NG.h"


GXLayer::GXLayer():
bounds(GXRectNull),
background(GXColors::Black),
_fb(nullptr),
_needsDisplay(false),
_needsLayout(false),
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

void GXLayer::update( GXContext* context , const GXRect& bounds)
{
    if( _needsDisplay)
    {
        paint(context, bounds);
    }
    
    _needsDisplay = false;
}
