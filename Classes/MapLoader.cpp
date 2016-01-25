//
//  MapLoader.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/19/16.
//
//

#include "MapLoader.h"
#include "MapTile.h"
#include "json/Document.h"

static const std::string s_blankTileName("blank.png");

MapLoader::MapLoader()
: m_tileLods()
, m_maxLod(0)
, m_tileWidth(1)
, m_tileHeight(1)
, m_defaultTile(0, 0, 0, 0, 0, s_blankTileName)
{
}

bool MapLoader::LoadMapConfig(const std::string& mapConfigPath)
{
    std::string configString = cocos2d::FileUtils::getInstance()->getStringFromFile(mapConfigPath);
    rapidjson::Document jsonDocument;
    jsonDocument.Parse(configString.c_str());
    
    if(jsonDocument.HasParseError())
    {
        cocos2d::log("Error parsing map config file: %s", mapConfigPath.c_str());
        cocos2d::log("Error code %d", jsonDocument.GetParseError());
        return false;
    }
    
    if(!jsonDocument.IsObject() ||
       !jsonDocument.HasMember("tiles") ||
       !jsonDocument.HasMember("map"))
    {
        cocos2d::log("Error in file structure: %s", mapConfigPath.c_str());
        return false;
    }
    
    // Pars map info
    auto& mapInfo = jsonDocument.FindMember("map")->value;
    if(mapInfo.HasMember("maxLod")){
        m_maxLod = mapInfo.FindMember("maxLod")->value.GetInt();
    }
    if(mapInfo.HasMember("tileHeight")){
        m_tileHeight = mapInfo.FindMember("tileHeight")->value.GetInt();
    }
    if(mapInfo.HasMember("tileWidth")){
        m_tileWidth = mapInfo.FindMember("tileWidth")->value.GetInt();
    }
    if(mapInfo.HasMember("bounds")){
        auto& bounds = mapInfo.FindMember("bounds")->value;
        int x1 = bounds[0].GetInt();
        int y1 = bounds[1].GetInt();
        int x2 = bounds[2].GetInt();
        int y2 = bounds[3].GetInt();
        float mapWidth = x2 - x1;
        float mapHeight = y2 - y1;
        m_mapRect = CoordinateRegion(x1, y1, mapWidth, mapHeight);
    }
    
    m_tileLods.resize(m_maxLod);
    m_defaultTile = MapTileInfo(0, 0, 0, m_tileWidth, m_tileHeight, s_blankTileName);
    m_defaultTile.blank = true;
    
    // Parse tile infos
    auto& tiles = jsonDocument.FindMember("tiles")->value;
    
    if(!tiles.IsArray()){
        cocos2d::log("Error in file structure: %s", mapConfigPath.c_str());
        return false;
    }
    
    for(int i = 0, end = tiles.Size(); i < end; ++i)
    {
        auto& tile = tiles[i];
        int lod = tile.FindMember("lod")->value.GetInt();
        auto& bounds = tile.FindMember("bounds")->value;
        int x1 = bounds[0].GetInt();
        int y1 = bounds[1].GetInt();
        int x2 = bounds[2].GetInt();
        int y2 = bounds[3].GetInt();
        auto& nameValue = tile.FindMember("name")->value;
        std::string name(nameValue.GetString(), nameValue.GetStringLength());
        MapTileInfo info = MapTileInfo(lod, x1, y1, x2-x1, y2-y1, name);
        TileIndex index = getTileIndex(info.region.origin, lod);
        m_tileLods[lod][index.first][index.second] = info;
    }
    
    return true;
}

Coordinate MapLoader::getTileSize(int _lod) const
{
    int lod = getNearesLod(_lod);
    float tileWidth = (1 << lod) * m_tileWidth;
    float tileHeight = (1 << lod) * m_tileHeight;
    return {tileWidth, tileHeight};
}

Coordinate MapLoader::getOffsetForTile(const Coordinate& point, int _lod) const
{
    int lod = getNearesLod(_lod);
    int tileWidth = (1 << lod) * m_tileWidth;
    int tileHeight = (1 << lod) * m_tileHeight;
    int x = (int)point.x % tileWidth;
    int y = (int)point.y % tileHeight;
    return Coordinate(x, y);
}

std::pair<int, int> MapLoader::getTileIndex(const Coordinate& point, int _lod) const
{
    int lod = getNearesLod(_lod);
    float tileWidth = (1 << lod) * m_tileWidth;
    float tileHeight = (1 << lod) * m_tileHeight;
    int x = floor(point.x / tileWidth);
    int y = floor(point.y / tileHeight);
    return {x, y};
}

const MapTileInfo& MapLoader::getMapTileInfo(const TileIndex& index, int lod)
{
    return _getMapTileInfo(index, getNearesLod(lod));
}

const MapTileInfo& MapLoader::_getMapTileInfo(const TileIndex& index, int lod)
{
    const TileMap& map = m_tileLods[lod];
    auto column = map.find(index.first);
    if(column == map.end())
    {
        return m_defaultTile;
    }
    auto info = column->second.find(index.second);
    if(info == column->second.end())
    {
        return m_defaultTile;
    }
    return info->second;
}

int MapLoader::getNearesLod(int lod) const
{
    if(lod < 0) return 0;
    if(lod >= m_maxLod) return m_maxLod-1;
    return lod;
}
