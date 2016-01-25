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

void MapTileLayer::tileRegion(const Coordinate& a, const Coordinate& b, int lod)
{
    removeAllChildren();
    
    MapLoader* loader = MapViewLayer::Context.Loader;
    float height = fabsf(b.y - a.y);
    const auto left = a.x;
    const auto right = b.x;
    const auto bottom = height - b.y;
    const auto top = height - a.y;
    
    Coordinate leftBot(left, bottom);
    Coordinate rightTop(right, top);
    Coordinate size = loader->getTileSize(lod);
    
    auto ai = loader->getTileIndex(leftBot, lod);
    auto bi = loader->getTileIndex(rightTop, lod);
    Coordinate offset;// = loader->getOffsetForTile(leftTop, lod);
    
    int tilesWidth = bi.first - ai.first;
    int tilesHeight = bi.second - ai.second;
    
    const MapTileInfo& info = loader->getMapTileInfo(ai, lod);
    offset = info.region.origin;
    
    for(int i = ai.second; i <= bi.second; ++i) // y axis
    {
        for(int j = ai.first; j <= bi.first; ++j) // x axis
        {
            Coordinate position = Coordinate(j*size.x, height - i*size.y);
            const MapTileInfo& tileInfo = loader->getMapTileInfo({j, i}, lod);
            addTile(tileInfo, position, size);
        }
    }
}

void MapTileLayer::addTile(const MapTileInfo& info, const Coordinate& position, const Coordinate& size)
{
    if(!info.blank)
    {
        ;
    }
    MapTile* tile = MapTile::getOrCreate(info, size);
    tile->setPosition(position);
    float scale = 1 << info.lod;
    
    addChild(tile);
}
