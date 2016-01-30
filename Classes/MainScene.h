#pragma once

#include "cocos2d.h"
#include "MapContext.h"

class MapLoader;
class MapLayer;
class PointsLoader;

class MapViewLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MapViewLayer);
    
    static MapContext Context;
    
    
    MapViewLayer();
    virtual ~MapViewLayer();
    
    void onPan(const cocos2d::Vec2& delta);
    void onPanEnded();
    void onPinch(const cocos2d::Vec2& delta, float scale, const cocos2d::Vec2& scalePivot);
    void onPinchEnded();
    void onTap(const cocos2d::Vec2& position);
    void onLongTap(const cocos2d::Vec2& position);
    
private:
    MapLoader* m_mapLoader;
    PointsLoader* m_pointsLoader;
    
    MapLayer* m_layer;
};
