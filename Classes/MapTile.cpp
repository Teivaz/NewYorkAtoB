//
//  MapTile.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/17/16.
//
//

#include "MapTile.h"

USING_NS_CC;

MapTile::MapTile()
{
}

bool MapTile::init()
{
    return Node::init();
}

//TODO: add caching
MapTile* MapTile::getOrCreate(const MapTileInfo& tileInfo, const Coordinate& size)
{
    MapTile* tile = new MapTile();
    tile->autorelease();
    if(!tile->init())
    {
        return nullptr;
    }
    
    tile->retain();
    auto onReady = [=](Texture2D* texture){
        Sprite* sprite = Sprite::createWithTexture(texture);
        float spriteScale = sprite->getSpriteFrame()->getOriginalSizeInPixels().width / size.x;
        sprite->setScale(CC_CONTENT_SCALE_FACTOR() / spriteScale);
        
        tile->addChild(sprite);
        sprite->setAnchorPoint({0, 0});
        sprite->setPosition({0, 0});
        tile->release();
    };
    Director::getInstance()->getTextureCache()->addImageAsync(tileInfo.name, onReady);
    
    return tile;
}
