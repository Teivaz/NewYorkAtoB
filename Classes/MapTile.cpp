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
    mapFragment->setAnchorPoint({0, 1});
    mapFragment->setPosition({0, getContentSize().height});
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

MapTile* MapTile::getOrCreate(const MapTileInfo& tileInfo, const Coordinate& size)
{
    auto sprite = cocos2d::Sprite::create(tileInfo.name);
    //sprite->setScale(CC_CONTENT_SCALE_FACTOR());
    //if(tileInfo.blank)
    {
        float spriteScale = sprite->getSpriteFrame()->getOriginalSizeInPixels().width / size.x;
        sprite->setScale(CC_CONTENT_SCALE_FACTOR() / spriteScale);
    }
    
    return create(tileInfo.region, sprite);
}
