#pragma once

#include "cocos2d.h"
#include "MapContext.h"

class MapLoader;
class MapLayer;

class MapViewLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MapViewLayer);
    
    static MapContext Context;
    
    
    MapViewLayer();
    virtual ~MapViewLayer();
    
    void onPan(const cocos2d::Vec2& delta);
    void onPanEnded();
    void onPinch(const cocos2d::Vec2& delta, float scale, const cocos2d::Vec2& scalePivot);
    void onPinchEnded();
    
private:
    MapLoader* m_loader;
    
    MapLayer* m_layer;
};
