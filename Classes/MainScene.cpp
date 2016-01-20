#include "MainScene.h"
#include "MapLoader.h"

#include "MapTile.h"

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
    
    m_loader = new MapLoader();
    m_loader->LoadMapConfig("map/map.json");
    Context.Loader = m_loader; // Set this loader as current
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MapViewLayer::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    
    auto t = Context.Loader->getMapTile({10, 10}, 0);
    this->addChild(t, 2);
    
    return true;
}


void MapViewLayer::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
