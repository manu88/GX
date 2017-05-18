//
//  GXTouchResponder.hpp
//  GX
//
//  Created by Manuel Deneu on 18/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXTouchResponder_hpp
#define GXTouchResponder_hpp


#include "GXGeometry.hpp"

struct GXTouch
{
    GXPoint center;
};

class GXTouchResponder
{
    friend class GXTouchHandler;
public:
    virtual ~GXTouchResponder()
    {}
    
protected:
    GXTouchResponder(){}
    
    virtual bool touchBegan( const GXTouch &t);
    virtual bool touchMoved( const GXTouch &t);
    virtual bool touchEnded( const GXTouch &t);
    
private:
    
};

#endif /* GXTouchResponder_hpp */
