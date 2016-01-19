//
//  MapLoader.hpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/19/16.
//
//

#pragma once
#include "cocos2d.h"

class MapTile;

class MapLoader{
public:
    MapLoader();
    
    bool LoadMapConfig(const std::string& mapConfigPath);
    
    MapTile* getMapTileForCoordinates(const cocos2d::Vec2& coordinate, int lodLevel);

private:
    
    struct MapTileInfo{
        int lod;
        int x1, y1, x2, y2;
        std::string name;
    };
    
    std::string calculateTileName(int x, int y, int l);
    std::string calculateTileName(const MapLoader::MapTileInfo& tileInfo);
    std::string calculateTileName(const cocos2d::Vec2& point, int lod);
    int getNearesLod(int lod);
    
    std::map<std::string, MapTileInfo> m_tileInfoMap;
    int m_maxLod;
    int m_tileWidth;
    int m_tileHeight;
};
