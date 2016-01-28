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
    
    // Load points from the map context
    void LoadPoints();
    
    void onTap(const cocos2d::Vec2& point);
    
    void setEditModeEnabled(bool val);
    void onParentScaleChanged(float scale);
    
private:
    void tryOpenPoint(const cocos2d::Vec2& point);
    void closeActivePoint();
    void createUserPoint(const cocos2d::Vec2& point);
    void saveActivePointAsUserPoint();
    
private:
    bool m_editMode;
    bool m_hasActivePoint;
    
    cocos2d::ui::Widget* m_widget;
    
    float m_parentScale;
};
