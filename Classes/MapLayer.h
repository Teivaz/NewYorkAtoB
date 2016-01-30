//
//  MapLodLevel.h
//  NewYorkAtoB
//
//  Created by teivaz on 1/19/16.
//
//

#pragma once

#include "cocos2d.h"
#include "Coordinates.h"

class MapTileLayer;
class MapPointsLayer;

// MapLayer is responsible for converting pixels to coordinates,
// representing a tiled map and map points
// This layer is scaled and moved when navigating a map
class MapLayer : public cocos2d::Node
{
public:
    CREATE_FUNC(MapLayer);
    
    // size of the view in pixels
    void setViewSize(const cocos2d::Size& size);
    void setBoundsEnabled(bool val){m_enableBounds = val;}
    void setBounds(const Coordinate& a, const Coordinate& b, float minScale, float maxScale);
    
    // Map controls
    void adjustPosition(const cocos2d::Vec2& pos);
    void adjustScale(float s, const cocos2d::Vec2& pivot);
    void adjustScaleAndPosition(float s, const cocos2d::Vec2& pivot, const cocos2d::Vec2& position);
    void applyAdjust();
    void onTap(const cocos2d::Vec2& point);
    void onLongTap(const cocos2d::Vec2& point);
    
    // scale of the map. Pixels per coordinate unit
    void setMapScale(float scale);
    float getMapScale() const;
    
    // focus coordinate point
    void setMapFocus(const Coordinate& focus);
    
private:
    MapLayer();
    bool init() override;
    
    // Use setMapFocus instead
    void setPosition(const cocos2d::Vec2& pos) override;
    
    // Use setMapScale instead
    void setScale(float scale) override;
    
    void rebuildMap();
    void calculateTransformation();
    void applyBounds(cocos2d::Vec3 offset, float scale);
    int lodForScale(float scale) const;
    void onLodChanged();
    
private:
    MapTileLayer* m_tileLayer;
    MapPointsLayer* m_pointsLayer;
    cocos2d::Vec2 m_position;
    cocos2d::Vec2 m_adjustedPosition;
    cocos2d::Vec2 m_adjustedScalePivot;
    float m_scale;
    float m_adjustedScale;
    cocos2d::Mat4 m_transform;
    int m_lod;
    
    bool m_enableBounds;
    Coordinate m_boundA;
    Coordinate m_boundB;
    float m_minScale;
    float m_maxScale;
};
