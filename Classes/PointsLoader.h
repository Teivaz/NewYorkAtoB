//
//  PointsLoader.h
//  NewYorkAtoB
//
//  Created by teivaz on 1/26/16.
//
//

#pragma once
#include "MapPoint.h"

class PointsLoader
{
    typedef std::list<MapPoint> MapPointList;
public:
    PointsLoader();
    bool loadPointsConfig(const std::string& mapPointsConfigPath);
    
    void addPoint(const MapPoint& point);
    
    void saveUserPoints();
    
private:
    void loadMapPoints(const std::string& configPath);
    void loadUserPoints(const std::string& configPath);
    
    std::string userConfigPath(const std::string& mapPointsConfigPath) const;
    
    void insertPoint(const MapPoint& p);
    void insertUserPoint(const MapPoint& p);
    
    void serializePoints(const MapPointList& points, std::string& outString);
    void deserializePoints(MapPointList& outPoints, const std::string& );
    
private:
    
    std::string m_userConfigPath;
    MapPointList m_mapPoints;
    MapPointList m_userPoints;
    bool m_userPointsDirty;
};
