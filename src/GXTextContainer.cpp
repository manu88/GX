//
//  GXText.cpp
//  GX
//
//  Created by Manuel Deneu on 20/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//
#include <string.h> //strlen
#include <assert.h>
#include <GXTextContainer.hpp>
#include <GXContext.hpp>
#include "nanovg/nanovg.h"


int GXTextContainerDelegate::widthForLine( GXTextContainer* text , int nLine)
{
    return text->getSize().width;
}

/*static*/ GXTextContainerDelegate GXTextContainer::_defaultDelegate;

GXTextContainer::GXTextContainer():
_delegate(nullptr)
{
    
}

void GXTextContainer::setSize( const GXSize &size) noexcept
{
    _size = size;
}

void GXTextContainer::setContent( const std::string &c) noexcept
{
    _content = c;
}


void GXTextContainer::draw(GXContext* context, const GXPoint &textPos)
{
    GXTextContainerDelegate* del = _delegate;
    if( del == nullptr)
    {
        //printf("No Layout delegate, use default\n");
        del = &_defaultDelegate;
    }
    
    const char* c = _content.c_str();
    
    int totalRows = 0;
    int nrows =0;
    const char* start = c;
    const char* end = c + strlen(c);
    
    float lineh;
    context->getTextMetrics(nullptr , nullptr , &lineh);
    
    int x = textPos.x;
    int y = textPos.y;
    
    int lnum = 0;
    
    //float caretx;
    float px;
    int nglyphs = 0;
    
    
    NVGtextRow rows[3];
    int currentRow = 0;
    
    int lineWidth = del->widthForLine(this, totalRows);
    assert(lineWidth >= 0);
    while((nrows = context->textBreakLines( start, end, lineWidth , rows, 1)))
    {
        
        currentRow++;
        for (int i = 0; i < nrows; i++)
        {
            NVGtextRow* row = &rows[i];
            
            context->addText(GXPointMake(x, y), row->start, row->end);
            
            
            /* Hit test */
            
            for (HitTest hitTest : _hitList)
            {
                //HitTest hitTest = _hitList.front();
                
                int hit = hitTest.point.x > x && hitTest.point.x < (x+ _size.width) &&
                          hitTest.point.y >= y && hitTest.point.y < (y+lineh);
                
                
                
                if (hit)
                {
                    hitTest.textPos.x = (hitTest.point.x < x+row->width/2) ? x : x+row->width;
                    px = x;
                    
                    if(row->end > row->start)
                    {
                        //assert(row->end > row->start);
                        const size_t numG = row->end - row->start;
                        
                        
                        NVGglyphPosition glyphs[ numG];
                        
                        nglyphs = nvgTextGlyphPositions(static_cast<NVGcontext*>( context->getImpl() ),
                                                        x, y, row->start, row->end, glyphs, (int)numG);
                        
                        for (int j = 0; j < nglyphs; j++)
                        {
                            float x0 = glyphs[j].x;
                            float x1 = (j+1 < nglyphs) ? glyphs[j+1].x : x+row->width;
                            float gx = x0 * 0.3f + x1 * 0.7f;
                            
                            if (hitTest.point.x >= px && hitTest.point.x < gx)
                            {
                                const char*p = glyphs[j].str;

                                hitTest.textOffset  = p-c;
                                hitTest.textPos.x = glyphs[j].x;
                                hitTest.textPos.y = y;
                                
                                _hitList.front() = hitTest;

                            }
                        
                            px = gx;
                        }
                    }
                }
            }
            /* End Hit test */
            lnum++;
            y += lineh;
        }
        
        totalRows += nrows;
        start = rows[nrows-1].next;
        
        int lineWidth = del->widthForLine(this, totalRows);
        assert(lineWidth >= 0);
    }

}
