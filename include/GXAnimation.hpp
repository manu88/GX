//
//  GXAnimation.hpp
//  GX
//
//  Created by Manuel Deneu on 23/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXAnimation_hpp
#define GXAnimation_hpp

#include "GXGeometry.hpp"

class GXLayer;
class GXAnimation
{
public:
    
    static GXAnimation* MoveTo( const GXPoint &p  );
    GXAnimation()
    {
        currentAlpha = 1.f;
        layer = nullptr;
    }
    float currentAlpha;
    
    bool update();
    
    GXLayer *layer;
};

#endif /* GXAnimation_hpp */
