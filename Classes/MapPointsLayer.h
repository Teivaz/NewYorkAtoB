//
//  MapPointsLayer.h
//  NewYorkAtoB
//
//  Created by teivaz on 1/26/16.
//
//

#pragma once

#include "cocos2d.h"
#include "Coordinates.h"

class MapPointsLayer : public cocos2d::Node
{
public:
    MapPointsLayer();
    ~MapPointsLayer();
    bool init();
    CREATE_FUNC(MapPointsLayer);
    
    void LoadPoints(const std::string& mapName);
};
