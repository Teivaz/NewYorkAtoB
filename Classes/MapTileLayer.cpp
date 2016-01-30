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
    m_lod = -1;
    m_indexA = {0, 0};
    m_indexB = {0, 0};
    return true;
}

void MapTileLayer::tileRegion(const Coordinate& a, const Coordinate& b, int lod)
{
    MapLoader* loader = MapViewLayer::Context.Loader;
    
    auto ai = loader->getTileIndex(a, lod);
    auto bi = loader->getTileIndex(b, lod);
    
    if(ai == m_indexA && bi == m_indexB && m_lod == lod){
        // don't rebuild map
        return;
    }
    removeAllChildren();
    m_indexA = ai;
    m_indexB = bi;
    m_lod = lod;
    
    int tilesPending = _tileRegion(a, b, lod, true);
    if(tilesPending > 0)
    {
        _tileRegion(a, b, lod+1, false);
    }
}

int MapTileLayer::_tileRegion(const Coordinate& a, const Coordinate& b, int lod, bool async)
{
    MapLoader* loader = MapViewLayer::Context.Loader;
    auto ai = loader->getTileIndex(a, lod);
    auto bi = loader->getTileIndex(b, lod);
    Coordinate size = loader->getTileSize(lod);
    
    cocos2d::log("Map rebuild (%d,%d) (%d,%d)", ai.first, ai.second, bi.first, bi.second);
    int tilesNotYetLoaded = 0;
    for(int i = ai.second; i <= bi.second; ++i) // y axis
    {
        for(int j = ai.first; j <= bi.first; ++j) // x axis
        {
            Coordinate position = Coordinate(j*size.x, i*size.y);
            const MapTileInfo& tileInfo = loader->getMapTileInfo({j, i}, lod);
            tilesNotYetLoaded += addTile(tileInfo, position, size, async);
        }
    }
    return tilesNotYetLoaded;
}

bool MapTileLayer::addTile(const MapTileInfo& info, const Coordinate& position, const Coordinate& size, bool async)
{
    if(!info.blank)
    {
        const MapTileInfo& blank = MapViewLayer::Context.Loader->getBlankTile();
        MapTile* tile = MapTile::create(blank, size);
        tile->setPosition(position);
        // push blank tiles all way to the back
        addChild(tile, -100);
    }
    MapTile* tile = nullptr;
    if(async)
    {
        tile = MapTile::createAsync(info, size);
    }
    else
    {
        tile = MapTile::create(info, size);
    }
    tile->setPosition(position);
    
    // draw tiles with lower LOD first
    addChild(tile, -info.lod);
    
    return !tile->isLoaded();
}
