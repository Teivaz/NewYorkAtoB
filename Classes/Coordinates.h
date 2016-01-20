//
//  Coordinates.h
//  NewYorkAtoB
//
//  Created by teivaz on 1/20/16.
//
//

#pragma once
#include "cocos2d.h"

typedef cocos2d::Vec2 Coordinate;
typedef cocos2d::Rect CoordinateRegion;

inline bool operator==(const CoordinateRegion& region, const Coordinate& point)
{
    return region.containsPoint(point);
}
