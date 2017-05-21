//
//  GXText.cpp
//  GX
//
//  Created by Manuel Deneu on 20/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include <assert.h>
#include "GXTextContainer.hpp"
#include "GXContext.hpp"
#include "nanovg/nanovg.h"

GXTextContainer::GXTextContainer(GXContext* context):
_context(context)
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

void GXTextContainer::setFontSize( float size) noexcept
{
    _context->setFontSize( size );
}

void GXTextContainer::setFontId( GXFontHandle id) noexcept
{
    _context->setFontId( id );
}

void GXTextContainer::setTextColor( const GXColor &color) noexcept
{
    _context->setFillColor(color);
}

void GXTextContainer::draw( const GXPoint &textPos)
{
    const char* c = _content.c_str();
    
    int totalRows = 0;
    int nrows =0;
    const char* start = c;
    const char* end = c + strlen(c);
    
    float lineh;
    _context->getTextMetrics(nullptr , nullptr , &lineh);
    
    int x = textPos.x;
    int y = textPos.y;
    
    int lnum = 0;
    
    //float caretx;
    float px;
    int nglyphs = 0;
    
    
    NVGtextRow rows[3];
    int currentRow = 0;
    
    while((nrows = _context->textBreakLines( start, end, _size.width , rows, 1)))
    {
        
        currentRow++;
        for (int i = 0; i < nrows; i++)
        {
            NVGtextRow* row = &rows[i];
            
            _context->addText(GXPointMake(x, y), row->start, row->end);
            
            HitTest hitTest = _hitList.front();
            
            int hit = hitTest.point.x > x && hitTest.point.x < (x+ _size.width) &&
                      hitTest.point.y >= y && hitTest.point.y < (y+lineh);
            
            
            
            if (hit)
            {
                hitTest.textPos.x = (hitTest.point.x < x+row->width/2) ? x : x+row->width;
                px = x;
                
                assert(row->end > row->start);
                const size_t numG = row->end - row->start;
                
                
                NVGglyphPosition glyphs[ numG];
                
                nglyphs = nvgTextGlyphPositions(static_cast<NVGcontext*>( _context->getImpl() ),
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
            
            lnum++;
            y += lineh;
        }
        
        totalRows += nrows;
        start = rows[nrows-1].next;
    }

}
