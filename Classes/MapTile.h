//
//  MapTile.hpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/17/16.
//
//

#pragma once
#include "cocos2d.h"

class MapTile : public cocos2d::Node{
private:
    MapTile();
    bool init(const cocos2d::Rect& coordinateRect, cocos2d::Sprite* mapFragment);
public:
    static MapTile* create(const cocos2d::Rect& coordinateRect, cocos2d::Sprite* mapFragment);
    static MapTile* getOrCreate(const cocos2d::Rect& coordinateRect, const std::string& name);
};
