//
//  GXAnimation.cpp
//  GX
//
//  Created by Manuel Deneu on 23/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include "GXAnimation.hpp"
#include "GXLayer.hpp"

bool GXAnimation::update()
{
    currentAlpha -= 0.05;
    if( currentAlpha <0)
    {
        currentAlpha = 1.f;
    }
    
    layer->setAlpha(currentAlpha);
    
    
    return true;
}
