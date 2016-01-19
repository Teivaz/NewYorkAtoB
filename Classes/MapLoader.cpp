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

MapLoader::MapLoader()
: m_tileInfoMap()
, m_maxLod(0)
, m_tileWidth(1)
, m_tileHeight(1)
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
        cocos2d::Vec2 a(x1, y1);
        cocos2d::Vec2 b(x2, y2);
        MapLoader::MapTileInfo info = {lod, x1, y1, x2, y2, name};
        std::string key = calculateTileName(info);
        m_tileInfoMap[key] = info;
    }
    
    return true;
}

MapTile* MapLoader::getMapTileForCoordinates(const cocos2d::Vec2& coordinate, int lodLevel)
{
    std::string key = calculateTileName(coordinate, lodLevel);
    auto it = m_tileInfoMap.find(key);
    if(it == m_tileInfoMap.end())
        return nullptr;
    const MapLoader::MapTileInfo& info = it->second;
    return MapTile::getOrCreate(cocos2d::Rect(info.x1,info.y1,info.x2,info.y2), info.name);
}

std::string MapLoader::calculateTileName(int x, int y, int l)
{
    std::stringstream ss;
    ss << x << '_' << y << '_' << l;
    return ss.str();
}

std::string MapLoader::calculateTileName(const MapLoader::MapTileInfo& tileInfo)
{
    int tileWidthForLod = m_tileWidth * 1 << tileInfo.lod;
    int tileHeightForLod = m_tileWidth * 1 << tileInfo.lod;
    int x = (tileInfo.x1 + tileInfo.x2) / 2;
    int y = (tileInfo.y1 + tileInfo.y2) / 2;
    return calculateTileName(x / tileWidthForLod, y / tileHeightForLod, tileInfo.lod);
}

std::string MapLoader::calculateTileName(const cocos2d::Vec2& point, int lod)
{
    int nearestLod = getNearesLod(lod);
    int tileWidthForLod = m_tileWidth * 1 << lod;
    int tileHeightForLod = m_tileWidth * 1 << lod;
    int x = int(point.x + tileWidthForLod / 2) / tileWidthForLod;
    int y = int(point.y + tileHeightForLod / 2) / tileHeightForLod;
    return calculateTileName(x, y, nearestLod);
}

int MapLoader::getNearesLod(int lod)
{
    if(lod < 0) return 0;
    if(lod > m_maxLod) return m_maxLod;
    return lod;
}
