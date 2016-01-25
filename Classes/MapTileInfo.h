//
//  MapTileInfo.hpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/20/16.
//
//

#pragma once
#include "Coordinates.h"

struct MapTileInfo{
    MapTileInfo();
    MapTileInfo(int _lod, int _x1, int _y1, int _x2, int _y2, const std::string& _name):
    lod(_lod), region(_x1, _y1, _x2, _y2), name(_name), blank(false){}
    
    int lod;
    CoordinateRegion region;
    std::string name;
    
    bool blank;
    
    // If point is inside the tile
    bool operator==(const Coordinate& point) const;
};
