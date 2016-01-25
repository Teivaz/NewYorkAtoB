//
//  MapTileInfo.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/20/16.
//
//

#include "MapTileInfo.h"

USING_NS_CC;

MapTileInfo::MapTileInfo()
: lod(0)
, region()
, name()
{}

bool MapTileInfo::operator==(const Coordinate& point) const
{
    // Region contains point
    if(blank) return true;
    return region == point;
}

