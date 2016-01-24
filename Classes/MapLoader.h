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
    typedef std::pair<int, int> TileIndex;
    MapLoader();
    
    bool LoadMapConfig(const std::string& mapConfigPath);
    
//    MapTile* getMapTile(const Coordinate& point, int lod);
    
    const MapTileInfo& getMapTileInfo(const TileIndex& index, int lod);
    TileIndex getTileIndex(const Coordinate& point, int lod) const;
    Coordinate getOffsetForTile(const Coordinate& point, int lod) const;
    Coordinate getTileSize(int lod) const;
private:
    
    // Requires existing lod
    const MapTileInfo& _getMapTileInfo(const TileIndex& index, int lod);
    
    int getNearesLod(int lod) const;
    
    typedef std::map<int, MapTileInfo> TileColumn;
    typedef std::map<int, TileColumn> TileMap;
    typedef std::vector<TileMap> TileLod;
    
    TileLod m_tileLods;
    int m_maxLod;
    int m_tileWidth;
    int m_tileHeight;
    
    MapTileInfo m_defaultTile;
};
