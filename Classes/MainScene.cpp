#include "MainScene.h"
#include "MapLoader.h"
#include "PointsLoader.h"

#include "MapLayer.h"

#include "TouchController.h"

USING_NS_CC;

MapContext MapViewLayer::Context;

MapViewLayer::MapViewLayer()
: m_mapLoader(nullptr)
{
    
}

MapViewLayer::~MapViewLayer(){
    if(Context.Loader == m_mapLoader){
        Context.Loader = nullptr; // remove pointer to my loader
    }
    if(Context.PointsLoader == m_pointsLoader)
    {
        Context.PointsLoader = nullptr;
    }
    delete m_mapLoader;
    delete m_pointsLoader;
}

Scene* MapViewLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MapViewLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MapViewLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto touchController = TouchController::create();
    addChild(touchController);
    
    touchController->onPinchEnd = [this](){this->onPinchEnded();};
    touchController->onDragEnd = [this](){this->onPanEnded();};
    touchController->onPinch = [this](const cocos2d::Vec2& delta, float scale, const cocos2d::Vec2& scalePivot){this->onPinch(delta, scale, scalePivot);};
    touchController->onDrag = [this](const cocos2d::Vec2& delta){this->onPan(delta);};
    touchController->onTap = [this](const Vec2& position){this->onTap(position);};
    
    m_mapLoader = new MapLoader();
    m_mapLoader->loadMapConfig("map/map.json");
    Context.Loader = m_mapLoader; // Set this loader as current
    
    m_pointsLoader = new PointsLoader();
    m_pointsLoader->loadPointsConfig("map/map_points.json");
    Context.PointsLoader = m_pointsLoader;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    m_layer = MapLayer::create();
    m_layer->setAnchorPoint({0, 0});
    m_layer->setViewSize(visibleSize);
    
    auto boundA = m_mapLoader->getMapRect().origin;
    auto boundB = boundA + m_mapLoader->getMapRect().size;
    m_layer->setBounds(boundA, boundB, 0.088, 1.7);
    m_layer->setBoundsEnabled(true);
    this->addChild(m_layer);
    

    
    
    
    return true;
}


void MapViewLayer::onPan(const cocos2d::Vec2& delta)
{
    m_layer->adjustPosition(delta);
}

void MapViewLayer::onPanEnded()
{
    m_layer->applyAdjust();
}

void MapViewLayer::onPinch(const cocos2d::Vec2& delta, float scale, const cocos2d::Vec2& scalePivot)
{
    m_layer->adjustScaleAndPosition(scale, scalePivot, delta);
}

void MapViewLayer::onPinchEnded()
{
    m_layer->applyAdjust();
}

void MapViewLayer::onTap(const Vec2& position)
{
    m_layer->onTap(position);
}
