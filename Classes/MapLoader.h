//
//  MapLoader.hpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/19/16.
//
//

#pragma once
#include "cocos2d.h"
#include "MapTileInfo.h"
#include "Coordinates.h"

class MapTile;

class MapLoader{
public:
    MapLoader();
    
    bool LoadMapConfig(const std::string& mapConfigPath);
    
    MapTile* getMapTile(const Coordinate& point, int lod);

private:
    
    // Requires existing lod
    const MapTileInfo& getMapTileInfo(const Coordinate& point, int lod);
    
    int getNearesLod(int lod);
    
    
    typedef std::list<MapTileInfo> TileInfos;
    typedef std::vector<TileInfos> TileInfoLod;
    
    TileInfoLod m_tileLods;
    int m_maxLod;
    int m_tileWidth;
    int m_tileHeight;
    
    MapTileInfo m_defaultTile;
};
