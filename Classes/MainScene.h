#pragma once

#include "cocos2d.h"
#include "MapContext.h"

class MapLoader;

class MapLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MapLayer);
    
    static MapContext Context;
    
    
    MapLayer();
    virtual ~MapLayer();
    
private:
    MapLoader* m_loader;
};
