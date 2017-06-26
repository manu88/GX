//
//  GXLayerUnitTest.cpp
//  GX
//
//  Created by Manuel Deneu on 22/06/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//





#include <stdio.h>
#include <assert.h>

#include "GXLayerUnitTest.hpp"

#include "GXContext.hpp"
#include "GXLayer.hpp"

#include "GXDisplay.hpp"

bool GXLayerUnitTest::run()
{
    printf("Start GXLayerUnitTest \n");

    GXDisplay disp;
    
    assert(disp.init( GXSizeMake( 1000, 700 ) ));
    
    assert(disp.getSize() == GXSizeMake( 1000, 700 ));
    
    GXContext ctx;
    assert(ctx.isValid());
    
    {
        printf("\tCreate invalid GXLayer instance \n");
    
        const GXSize size1 = GXSizeInvalid;
        GXLayer layer( &ctx , size1);
        assert(!layer.isValid());
    }
    
    printf("\tCreate VALID GXLayer instance \n");
    
    /**/
    
    const GXSize size1 = GXSizeMake(100, 200);
    GXLayer layer1( &ctx , size1);
    assert(layer1.isValid());
    assert(layer1.getSize() == size1);
    
    /**/
    
    const GXSize size2 = GXSizeMake(200, 400);
    GXLayer layer2( &ctx , size2);
    assert(layer2.isValid());
    assert(layer2.getSize() == size2);
    
    /**/
    
    const GXSize size3 = GXSizeMake(50, 50);
    GXLayer layer3( &ctx , size3);
    assert(layer3.isValid());
    assert(layer3.getSize() == size3);

    layer1.test(&ctx , GXColors::Red);
    layer2.test(&ctx , GXColors::Blue);
    layer3.test(&ctx , GXColors::Green);
    
    disp.beginDraw(&ctx);
    
    ctx.beginFrame( disp.getSize() , 1.f);

    layer2.draw( &ctx, GXPointMake(0, 0));
    layer3.draw( &ctx, GXPointMake(75, 175));
    layer1.draw( &ctx, GXPointMake(100, 200));
    
    
    ctx.endFrame();
    disp.endDraw(&ctx);
    disp.swap();
    

    while ( !disp.shouldClose())
    {
        
        disp.waitEvents();
        
        
    }
    
    
    return true;
}
