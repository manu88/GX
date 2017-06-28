#include <stdio.h>
#include <assert.h>
#include <string>

#include <GroundBase.hpp>

#include <GXDisplay.hpp>
#include <GXLayer.hpp>



int main()
{

    GXDisplay disp;

    if( !disp.init(GXSizeMake(1000, 800)))
    {
        printf("Display init error \n");
        return -1;
    }

    GXContext ctx;

        

    
    GB::RunLoop runL;
    
    /**/
    
    GXLayer layer(&ctx , disp.getSize());
    
    layer.test(&ctx, GXColors::Blue);
    
    
    
    /**/
    
    GB::Timer t;
    t.setInterval(40);
    t.setCallback([&](GB::Timer &timer)
    {
        static GXPoint pt = GXPointMakeNull();
        disp.clear();
        disp.beginDraw(&ctx);
        layer.draw(&ctx, pt);
        disp.endDraw(&ctx);
        disp.swap();
        disp.pollEvents();
        
        if( disp.shouldClose())
        {
            runL.stop();
        }
        
        pt.x +=5;
        pt.y +=5;
        
        if(!rectContainsPoint(GXRectMake(GXPointMakeNull(), disp.getSize()), pt))
        {
            pt = GXPointMakeNull();
        }

    });
    
    runL.addSource(t);

    runL.run();

	return 0;
}
