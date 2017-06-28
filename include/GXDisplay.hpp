//
//  GXDisplay.hpp
//  GX
//
//  Created by Manuel Deneu on 22/06/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXDisplay_hpp
#define GXDisplay_hpp


#include <functional>
#include <GXGeometry.hpp>
#include <GXEvent.h>
typedef struct _Display Display;


class GXContext;


class GXDisplay
{
public:
    
    typedef std::function<void( const GXEvent*)> EventCallback;
    
    GXDisplay();
    ~GXDisplay();
    
    bool init( const GXSize &);
    
    GXSize getSize() const noexcept;
    
    void beginDraw( GXContext* );
    void endDraw(GXContext* );
    
    
    EventCallback onEvent;
    
    // temp
    void clear() const;
    void swap() const ;
    bool shouldClose() const;
    void waitEvents() const;
    void pollEvents() const;
    
private:
    Display* _impl;
};

#endif /* GXDisplay_hpp */
