#include "MainScene.h"
#include "MapLoader.h"

#include "MapLayer.h"

#include "TouchController.h"

USING_NS_CC;

MapContext MapViewLayer::Context;

MapViewLayer::MapViewLayer()
: m_loader(nullptr)
{
    
}

MapViewLayer::~MapViewLayer(){
    if(Context.Loader == m_loader){
        Context.Loader = nullptr; // remove pointer to my loader
    }
    delete m_loader;
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
    
    m_loader = new MapLoader();
    m_loader->LoadMapConfig("map/map.json");
    Context.Loader = m_loader; // Set this loader as current
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    m_layer = MapLayer::create();
    m_layer->setAnchorPoint({0, 0});
    m_layer->setViewSize(visibleSize);
    
    auto boundA = m_loader->getMapRect().origin;
    auto boundB = boundA + m_loader->getMapRect().size;
    m_layer->setBounds(boundA, boundB, 0.04, 1.7);
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
