//
//  GXText.hpp
//  GX
//
//  Created by Manuel Deneu on 20/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXTextContainer_hpp
#define GXTextContainer_hpp

#include <string>
#include <vector>
#include <GXGeometry.hpp>
#include <GXFont.hpp>
#include <GXColor.hpp>





class GXTextContainer;
class GXTextContainerDelegate
{
    friend class GXTextContainer;
public:
    
    virtual ~GXTextContainerDelegate(){}
    
protected:
    GXTextContainerDelegate(){}
    
    virtual int widthForLine( GXTextContainer* text , int nLine);
    
};
class GXContext;

class GXTextContainer
{
    //friend class GXTextContainerDelegate;
public:
    
    struct HitTest
    {
        GXPoint point;
        size_t textOffset;
        GXPoint textPos;
    };
    
    typedef std::vector<GXTextContainer::HitTest> HitPointList;
    
    /* **** **** **** **** **** **** **** */
    
    
    GXTextContainer();
    
    void setSize( const GXSize &size) noexcept;
    const GXSize &getSize() const noexcept
    {
        return _size;
    }
    
    void setContent( const std::string &c) noexcept;
    const std::string &getContent() const noexcept
    {
        return _content;
    }
    
    void clearHitTests() noexcept
    {
        _hitList.clear();
    }
    void addHitTest( const GXPoint &p )
    {
        _hitList.push_back( GXTextContainer::HitTest{ p , 0 ,GXPointMakeNull() });
    }
    
    const HitPointList& getHitTestResults() const noexcept
    {
        return _hitList;
    }
    
    void draw(GXContext* context, const GXPoint &pos);

    void setDelegate( GXTextContainerDelegate* del) noexcept
    {
        _delegate = del;
    }
private:

    GXTextContainerDelegate* _delegate;

    GXSize _size;
    std::string _content;
    
    HitPointList _hitList;
    
    static GXTextContainerDelegate _defaultDelegate;
    
};

#endif /* GXTextContainer_hpp */
