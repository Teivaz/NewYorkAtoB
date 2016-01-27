//
//  MapPoint.h
//  NewYorkAtoB
//
//  Created by teivaz on 1/26/16.
//
//

#pragma once
#include "Coordinates.h"

struct MapPoint
{
    Coordinate point;
    std::string name;
    std::string description;
};
