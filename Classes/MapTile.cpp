//
//  MapTile.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/17/16.
//
//

#include "MapTile.h"

MapTile::MapTile()
{
}

bool MapTile::init(const CoordinateRegion& region, cocos2d::Sprite* mapFragment)
{
    addChild(mapFragment);
    setContentSize(mapFragment->getContentSize());
    return true;
}

MapTile* MapTile::create(const CoordinateRegion& region, cocos2d::Sprite* mapFragment)
{
    MapTile* tile = new MapTile();
    tile->autorelease();
    if(tile->init(region, mapFragment))
        return tile;
    return nullptr;
}

MapTile* MapTile::getOrCreate(const MapTileInfo& tileInfo)
{
    auto sprite = cocos2d::Sprite::create(tileInfo.name);
    return create(tileInfo.region, sprite);
}
