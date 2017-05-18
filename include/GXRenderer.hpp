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
#include "GXGeometry.hpp"

class GXLayer;


class GXRenderer
{
public:
    GXRenderer();
    ~GXRenderer();
    
    void setRoot( GXLayer* layer);
    
    
    
    
    
    
    void draw( GXContext* context );
    void renderPass( GXContext* context);
private:
    void drawImage(GXLayer* layer , GXContext* context , const GXPoint &accumPos);
    
    
    
    
    GXLayer *_rootLayer;
};

#endif /* GXRenderer_hpp */
