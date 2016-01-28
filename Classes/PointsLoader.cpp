//
//  PointsLoader.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/26/16.
//
//

#include "PointsLoader.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "json/rapidjson.h"

USING_NS_CC;

PointsLoader::PointsLoader()
: m_mapPoints()
, m_userPoints()
, m_userPointsDirty(false)
{
    
}

void PointsLoader::serializePoints(const PointsLoader::MapPointList& points, std::string& outString)
{
    rapidjson::MemoryPoolAllocator<> allocator;
    rapidjson::CrtAllocator allocatorCrt;
    
    rapidjson::Document jsonDocument(&allocator);
    jsonDocument.SetArray();
    for(const auto& point : points)
    {
        rapidjson::Value element;
        element.SetObject();
        
        rapidjson::Value position;
        position.SetArray();
        position.Reserve(2, allocator);
        position.PushBack(point.point.x, allocator);
        position.PushBack(point.point.y, allocator);
        
        rapidjson::Value name;
        name.SetString(point.name.c_str(), point.name.size());
        
        rapidjson::Value desc;
        desc.SetString(point.description.c_str(), point.description.size());
        
        element.AddMember("position", position, allocator);
        element.AddMember("name", name, allocator);
        element.AddMember("description", desc, allocator);
        
        jsonDocument.PushBack(element, allocator);
    }
    rapidjson::StringBuffer buf(&allocatorCrt);
    rapidjson::Writer<rapidjson::StringBuffer> writer (buf, &allocatorCrt);

    jsonDocument.Accept(writer);
    outString = std::string(buf.GetString(), buf.GetSize());

}

MapPoint deserializePoint(rapidjson::Value& value)
{
    MapPoint p;
    rapidjson::Value& position = value.FindMember("position")->value;
    if(position.IsArray())
    {
        p.point.x = position[0].GetDouble();
        p.point.y = position[1].GetDouble();
    }
    rapidjson::Value& name = value.FindMember("name")->value;
    if(name.IsString())
    {
        p.name = name.GetString();
    }
    rapidjson::Value& desc = value.FindMember("description")->value;
    if(desc.IsString())
    {
        p.description = desc.GetString();
    }
    return p;
}

void PointsLoader::deserializePoints(PointsLoader::MapPointList& outPoints, const std::string& content)
{
    rapidjson::Document jsonDocument;
    jsonDocument.Parse(content.c_str());
    
    if(jsonDocument.HasParseError())
    {
        cocos2d::log("Error parsing map points config");
        cocos2d::log("Error code %d", jsonDocument.GetParseError());
        return;
    }
    if(!jsonDocument.IsArray())
    {
        return;
    }
    
    int size = jsonDocument.Size();
    for(int i = 0; i < size; ++i)
    {
        outPoints.push_back(deserializePoint( jsonDocument[i] ));
    }

}


void PointsLoader::loadMapPoints(const std::string& configPath)
{
    std::string configString = cocos2d::FileUtils::getInstance()->getStringFromFile(configPath);

    if(configString.length())
    {
        deserializePoints(m_mapPoints, configString);
    }
}

void PointsLoader::loadUserPoints(const std::string& configPath)
{
    std::string configString = FileUtils::getInstance()->getStringFromFile(configPath);
    
    if(configString.length())
    {
        deserializePoints(m_userPoints, configString);
    }
}

std::string PointsLoader::userConfigPath(const std::string& mapPointsConfigPath) const
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    std::string userPath(mapPointsConfigPath.begin(), mapPointsConfigPath.end() - 5);
    userPath = path + userPath + "_user" + std::string(mapPointsConfigPath.end()-5, mapPointsConfigPath.end());
    return userPath;
}

bool PointsLoader::loadPointsConfig(const std::string &mapPointsConfigPath)
{
    m_mapPoints.clear();
    m_userPoints.clear();
    m_userPointsDirty = false;
    
    m_userConfigPath = userConfigPath(mapPointsConfigPath);
    
    loadMapPoints(mapPointsConfigPath);
    loadUserPoints(m_userConfigPath);
    return true;
}

void PointsLoader::addPoint(const MapPoint& point)
{
    m_userPointsDirty = true;
    m_userPoints.push_back(point);
}

void PointsLoader::saveUserPoints()
{
    if(!m_userPointsDirty)
    {
        return;
    }
    std::string serialized;
    serializePoints(m_userPoints, serialized);
    
    FileUtils* fu = FileUtils::getInstance();
    
    std::string dir = fu->getFileDir(m_userConfigPath);
    if(dir.length())
    {
        if(!fu->isDirectoryExist(dir))
        {
            fu->createDirectory(dir);
        }
    }
    fu->writeStringToFile(serialized, m_userConfigPath);
    m_userPointsDirty = false;
}

void PointsLoader::insertPoint(const MapPoint& p)
{
    m_mapPoints.push_back(p);
}

void PointsLoader::insertUserPoint(const MapPoint& p)
{
    m_userPoints.push_back(p);
}

MapPoint* PointsLoader::findNearestPointWithinRadius(const Coordinate& point, float radius)
{
    radius *= radius;// square radius for futher usage
    
    MapPoint* result = nullptr;
    if(m_mapPoints.empty() && m_userPoints.empty())
    {
        return nullptr;
    }
    
    for(auto& i : m_mapPoints)
    {
        float distanceSq = point.distanceSquared(i.point);
        if(distanceSq <= radius)
        {
            result = &i;
            radius = distanceSq;
        }
    }
    for(auto& i : m_userPoints)
    {
        float distanceSq = point.distanceSquared(i.point);
        if(distanceSq <= radius)
        {
            result = &i;
            radius = distanceSq;
        }
    }
    return result;
}


void PointsLoader::findAllPointsInRadius(const Coordinate& point, float radius, std::list<MapPoint*>& outList)
{
    radius *= radius;// square radius for futher usage
    if(m_mapPoints.empty() && m_userPoints.empty())
    {
        return;
    }
    
    for(auto& i : m_mapPoints)
    {
        float distanceSq = point.distanceSquared(i.point);
        if(distanceSq <= radius)
        {
            outList.push_back(&i);
        }
    }
       
    for(auto& i : m_userPoints)
    {
        float distanceSq = point.distanceSquared(i.point);
        if(distanceSq <= radius)
        {
            outList.push_back(&i);
        }
    }
}
