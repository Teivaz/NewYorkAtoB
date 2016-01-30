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

class MapPoint;

class MapPointsLayer : public cocos2d::Node
{
public:
    MapPointsLayer();
    ~MapPointsLayer();
    CREATE_FUNC(MapPointsLayer);
    
    void onTap(const cocos2d::Vec2& point);
    void onLongTap(const cocos2d::Vec2& point);
    
    void onParentScaleChanged(float scale);
    void onViewChanged(const Coordinate& a, const Coordinate& b);
    
private:
    bool init();
    void createPoint(const cocos2d::Vec2& point);
    void editPoint(MapPoint* p);
    MapPoint* tryOpenPoint(const cocos2d::Vec2& point);
    void openPoint(const MapPoint& point);
    void closeActivePoint();
    void createUserPoint(const cocos2d::Vec2& point);
    void saveActivePointAsUserPoint();
    void addPoint(MapPoint* point);
    
private:
    bool m_newPoint;
    bool m_hasActivePoint;
    
    cocos2d::ui::Widget* m_widget;
    
    float m_parentScale;
    
    cocos2d::SpriteFrame* m_pointIcon;
    cocos2d::Node* m_points;
};
