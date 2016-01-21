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
        m_tileLods[lod].push_back(info);
    }
    
    return true;
}

const MapTileInfo& MapLoader::getMapTileInfo(const Coordinate& point, int lod)
{
    return _getMapTileInfo(point, getNearesLod(lod));
}

const MapTileInfo& MapLoader::_getMapTileInfo(const Coordinate& point, int lod)
{
    const TileInfos& tileInfoMap = m_tileLods[lod];
    auto iterator = std::find(tileInfoMap.cbegin(), tileInfoMap.cend(), point);
    if(iterator == tileInfoMap.end()){
        return m_defaultTile;
    }
    return *iterator;
}

MapTile* MapLoader::getMapTile(const Coordinate& point, int lod)
{
    int nearestLod = getNearesLod(lod);
    return MapTile::getOrCreate(getMapTileInfo(point, nearestLod));
}

int MapLoader::getNearesLod(int lod)
{
    if(lod < 0) return 0;
    if(lod >= m_maxLod) return m_maxLod-1;
    return lod;
}
