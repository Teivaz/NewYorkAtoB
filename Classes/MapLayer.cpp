//
//  MapLayer.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/19/16.
//
//

#include "MapLayer.h"

bool MapLayer::init(){
    setAnchorPoint({0.5, 0.5});
    return true;
}

// size of the view in pixels
void MapLayer::setViewSize(const cocos2d::Size& size)
{
    setContentSize(size);
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
    m_lod = 0;
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
    
}

void MapLayer::onLodChanged()
{
    //
}
