#include <stdio.h>
#include <assert.h>
#include <string>

#include <GroundBase.hpp>

#include <GXDisplay.hpp>
#include <GXLayer.hpp>



int main(int argc , char** argv)
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
    
    GXLayer layer1(&ctx , disp.getSize());
    GXLayer layer2(&ctx , GXSizeMake(200, 300));
    
    layer1.test(&ctx, GXColors::Blue);
    layer2.test(&ctx, GXColors::Red);
    
    
    int deltaT = 40;
    if( argc > 1)
        deltaT = std::stoi(argv[1]);
    
    /**/
    
    GB::Timer t;
    t.setInterval(deltaT);
    t.setCallback([&](GB::Timer &timer)
    {
        static GXPoint pt1 = GXPointMakeNull();
        static GXPoint pt2 = GXPointMake(disp.getSize().width, disp.getSize().height);
        disp.clear();
        disp.beginDraw(&ctx);
        layer1.draw(&ctx, pt1);
        layer2.draw(&ctx, pt2);
        disp.endDraw(&ctx);
        disp.swap();
        disp.pollEvents();
        
        if( disp.shouldClose())
        {
            runL.stop();
        }
        
        pt1.x +=5;
        pt1.y +=5;
        
        pt2.x -=5;
        pt2.y -=5;
        
        if(!rectContainsPoint(GXRectMake(GXPointMakeNull(), disp.getSize()), pt1))
        {
            pt1 = GXPointMakeNull();
        }

        if(!rectContainsPoint(GXRectMake(GXPointMakeNull(), disp.getSize()), pt2))
        {
            pt2 = GXPointMake(disp.getSize().width, disp.getSize().height);
        }
    });
    
    runL.addSource(t);

    runL.run();

	return 0;
}
