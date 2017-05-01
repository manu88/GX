//
//  GXRenderer.hpp
//  nanoVGTest3
//
//  Created by Manuel Deneu on 01/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXRenderer_hpp
#define GXRenderer_hpp

#include "GXContext.hpp"

class GXLayer;


class GXRenderer
{
public:
    GXRenderer();
    ~GXRenderer();
    
    void setRoot( GXLayer* layer);
    
    
    void renderLayer(GXContext* vg, GXLayer* layer,  float pxRatio );
    
    
    
    void draw( GXContext* context );
private:
    void drawImage(GXLayer* layer , GXContext* context);
    
    bool createFB( GXContext*ctx , GXLayer* l );
    
    
    GXLayer *_rootLayer;
};

#endif /* GXRenderer_hpp */
