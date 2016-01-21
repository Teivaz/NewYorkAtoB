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

Coordinate MapTileInfo::getPointFromNextRegion(int x, int y) const
{
    // Take point in the middle and add size multiplied by x and y
    auto midX = region.size.width / 2 + (x * region.size.width);
    auto midY = region.size.height / 2 + (y * region.size.height);
    
    return region.origin + Coordinate(midX, midY);
}
