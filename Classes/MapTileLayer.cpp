//
//  MapTileLayer.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/21/16.
//
//

#include "MapTileLayer.h"
#include "MainScene.h"
#include "MapContext.h"
#include "MapTileInfo.h"
#include "MapLoader.h"
#include "MapTile.h"

bool MapTileLayer::init()
{
    return true;
}

void MapTileLayer::tileRegion(const CoordinateRegion& region, int lod)
{
    removeAllChildren();
    
    MapLoader* loader = MapViewLayer::Context.Loader;
    const auto left = region.getMinX();
    const auto right = region.getMaxX();
    const auto top = region.getMinY();
    const auto bottom = region.getMaxY();
    
    Coordinate leftTop(left, top);
    Coordinate rightTop(right, top);
    Coordinate leftBottom(left, bottom);
    // sweep left to right, top to bottom
    
    int tilesWidth = 1;
    const MapTileInfo* info = &loader->getMapTileInfo(leftTop, lod);
    Coordinate offset = leftTop - info->region.origin;
    offset.y *= -1;
    addTile(*info, offset);
    
    // set scale to compensate lod
    float scale = 1 << info->lod;
    setScale(scale);
             
    while(true)
    {
        if(*info == rightTop)
            break;
        ++tilesWidth;
        info = &loader->getMapTileInfo(info->getPointFromNextRegion(1, 0), lod);
        addTile(*info, offset);
    }
    
    int tilesHeight = 1;
    info = &loader->getMapTileInfo(leftTop, lod);
    while(true)
    {
        if(*info == leftBottom)
            break;
        ++tilesHeight;
        info = &loader->getMapTileInfo(info->getPointFromNextRegion(0, 1), lod);
        addTile(*info, offset);
    }
    
    info = &loader->getMapTileInfo(leftTop, lod);
    for(int i = 1; i <= tilesHeight; ++i) // y axis
    {
        for(int j = 1; j <= tilesWidth; ++j) // x axis
        {
            const MapTileInfo& tileInfo = loader->getMapTileInfo(info->getPointFromNextRegion(j, i), lod);
            addTile(tileInfo, offset);
        }
    }
}

void MapTileLayer::addTile(const MapTileInfo& info, const Coordinate& offset)
{
    if(info.blank)return;
    MapTile* tile = MapTile::getOrCreate(info);
    tile->setAnchorPoint({0,1});
    tile->setPosition(info.region.origin - offset);
    tile->setPositionY(-tile->getPositionY());
    float scale = 1 << info.lod;
    tile->setPosition(tile->getPosition() / scale);
    
    addChild(tile);
}
