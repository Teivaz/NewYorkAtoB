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
    bool init();
public:
    static MapTile* createAsync(const MapTileInfo& tileInfo, const Coordinate& size);
    static MapTile* create(const MapTileInfo& tileInfo, const Coordinate& size);
    
    bool isLoaded() const {return m_loaded;}
private:
    bool m_loaded;
};
