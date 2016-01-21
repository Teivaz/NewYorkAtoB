//
//  MapLodLevel.hpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/19/16.
//
//

#pragma once

#include "cocos2d.h"
#include "Coordinates.h"

class MapTileLayer;

// MapLayer is responsible for converting pixels to coordinates
// and representing a tiled map
class MapLayer : public cocos2d::Node
{
public:
    
    CREATE_FUNC(MapLayer);
    
    bool init();
    
    // size of the view in pixels
    void setViewSize(const cocos2d::Size& size);
    
    // scale of the map. Pixels per coordinate unit
    float getMapScale() const;
    void setMapScale(float s);
    
    // sets the center point of the map in coordinates
    void setMapFocus(const Coordinate& center);
    
private:
    void rebuildMap();
    
    void onLodChanged();
    
private:
    int m_lod;
    float m_mapScale;
    MapTileLayer* m_tileLayer;
    
};
