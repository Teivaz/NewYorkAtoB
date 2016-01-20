//
//  MapTile.hpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/17/16.
//
//

#pragma once
#include "cocos2d.h"
#include "MapTileInfo.h"

class MapTile : public cocos2d::Node{
private:
    MapTile();
    bool init(const CoordinateRegion& region, cocos2d::Sprite* mapFragment);
public:
    static MapTile* create(const CoordinateRegion& region, cocos2d::Sprite* mapFragment);
    static MapTile* getOrCreate(const MapTileInfo& tileInfo);
};
