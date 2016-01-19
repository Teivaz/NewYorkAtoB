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

bool MapTile::init(const cocos2d::Rect& coordinateRect, cocos2d::Sprite* mapFragment)
{
    addChild(mapFragment);
    setContentSize(mapFragment->getContentSize());
    return true;
}

MapTile* MapTile::create(const cocos2d::Rect& coordinateRect, cocos2d::Sprite* mapFragment)
{
    MapTile* tile = new MapTile();
    tile->autorelease();
    if(tile->init(coordinateRect, mapFragment))
        return tile;
    return nullptr;
}

MapTile* MapTile::getOrCreate(const cocos2d::Rect& coordinateRect, const std::string& name)
{
    auto sprite = cocos2d::Sprite::create(name);
    return create(coordinateRect, sprite);
}
