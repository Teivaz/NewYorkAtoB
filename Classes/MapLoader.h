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


// MapLoader parses map config and stores information about tiles and map
class MapLoader{
public:
    typedef std::pair<int, int> TileIndex;
    MapLoader();
    
    bool loadMapConfig(const std::string& mapConfigPath);
    
    const MapTileInfo& getBlankTile() const {return m_defaultTile;}
    const MapTileInfo& getMapTileInfo(const TileIndex& index, int lod);
    TileIndex getTileIndex(const Coordinate& point, int lod) const;
    Coordinate getOffsetForTile(const Coordinate& point, int lod) const;
    Coordinate getTileSize(int lod) const;
    
    const CoordinateRegion& getMapRect() const {return m_mapRect;}
private:
    
    // Requires existing lod
    const MapTileInfo& _getMapTileInfo(const TileIndex& index, int lod);
    
    int getNearesLod(int lod) const;
    
    // TODO: maybe rewrite to Morton numbers
    typedef std::map<int, MapTileInfo> TileColumn;
    typedef std::map<int, TileColumn> TileMap;
    typedef std::vector<TileMap> TileLod;
    
    TileLod m_tileLods;
    int m_maxLod;
    int m_tileWidth;
    int m_tileHeight;
    
    CoordinateRegion m_mapRect;
    
    MapTileInfo m_defaultTile;
};
