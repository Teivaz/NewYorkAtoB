//
//  MapTileLayer.hpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/21/16.
//
//

#pragma once
#include "cocos2d.h"
#include "Coordinates.h"

struct MapTileInfo;


// MapTileLayer is responsible for tiling a region with nearest LOD
class MapTileLayer : public cocos2d::Node
{
public:
    CREATE_FUNC(MapTileLayer);
    
    /** Fill region with tiles of nearest LOD
     * tiles of the required LOD will be loaded asynchronously
     * untill they fully loaded map will be tiled with greater LOD (loaded synchronously)
     */
    void tileRegion(const Coordinate& a, const Coordinate& b, int lod);
    
private:
    bool init();
    
    /** Fill rectangle from a to b with map tiles with nearest LOD to given lod
     * if async is true then return result is number of tiles that are not yet loaded
     */
    int _tileRegion(const Coordinate& a, const Coordinate& b, int lod, bool async);
    
    /* Load tile to the given position
     * returns true if tile is pending for async load
     */
    bool addTile(const MapTileInfo& info, const Coordinate& position, const Coordinate& size, bool async);
    
private:
    std::pair<int, int> m_indexA;
    std::pair<int, int> m_indexB;
    int m_lod;
};
