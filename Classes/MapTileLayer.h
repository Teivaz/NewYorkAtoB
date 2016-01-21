//
//  MapTileLayer.hpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/21/16.
//
//

#pragma once
#include "cocos2d.h"
#include "Coordinates.h"

struct MapTileInfo;

class MapTileLayer : public cocos2d::Node
{
public:
    CREATE_FUNC(MapTileLayer);
    bool init();
    
    void tileRegion(const CoordinateRegion& region, int lod);
    
private:
    void addTile(const MapTileInfo& info, const Coordinate& offset);
};
