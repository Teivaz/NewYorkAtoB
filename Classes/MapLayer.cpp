//
//  MapLayer.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/19/16.
//
//

#include "MapLayer.h"

#include "MapTileLayer.h"

bool MapLayer::init(){
    m_lod = 0;
    //setAnchorPoint({0.5, 0.5});
    
    m_tileLayer = MapTileLayer::create();
    addChild(m_tileLayer);
    m_tileLayer->setPosition(cocos2d::Vec2::ZERO);
    //setScale(0.1);
    return true;
}

// size of the view in pixels
void MapLayer::setViewSize(const cocos2d::Size& size)
{
    setContentSize(size);
    m_tileLayer->setPosition(cocos2d::Vec2(0, size.height));
    m_tileLayer->setAnchorPoint({0, 1});
    rebuildMap();
}

// scale of the map. Pixels per coordinate unit
float MapLayer::getMapScale() const
{
    return 0;
}
void MapLayer::setMapScale(float s)
{
    int oldLod = m_lod;
    m_mapScale = s;
    m_lod = 1;
    if(oldLod != m_lod){
        onLodChanged();
    }
}

// sets the center point of the map in coordinates
void MapLayer::setMapFocus(const Coordinate& center)
{
    
}

void MapLayer::rebuildMap()
{
    // create a layer for tiles in coordinates that will overlap view in coordinates
    // calculate lod level for
    
    setScale(1/4.0);
    CoordinateRegion region;
    cocos2d::Rect rect(getPosition(), getContentSize());
    rect.origin.y -= rect.size.height;
    
    rect.size = rect.size * 4;
    m_lod = 2;
    m_tileLayer->tileRegion(rect, m_lod);
}

void MapLayer::onLodChanged()
{
    //
}
