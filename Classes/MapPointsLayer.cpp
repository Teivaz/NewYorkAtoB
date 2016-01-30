//
//  MapPointsLayer.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/26/16.
//
//

#include "MapPointsLayer.h"
#include "UIEditBox/UIEditBox.h"
#include "UIButton.h"
#include "MapPoint.h"
#include "MainScene.h"
#include "PointsLoader.h"

USING_NS_CC;

static const std::string s_popupSprite("bubble.9.png");
static const Color3B s_textColor(0, 0, 0);
static const int s_textSize = 20;
static const Size s_boxSize(130, 90);
static const Size s_boxPadding(20, 15);

static const float s_touchRadius = 30;

MapPointsLayer::MapPointsLayer()
: m_newPoint(false)
, m_hasActivePoint(false)
, m_widget(nullptr)
, m_parentScale(1.0)
, m_pointIcon(nullptr)
, m_points(nullptr)
{
}

MapPointsLayer::~MapPointsLayer()
{
    CC_SAFE_RELEASE_NULL(m_pointIcon);
}

bool MapPointsLayer::init()
{
    Sprite* s = Sprite::create("point.png");
    m_pointIcon = s->getSpriteFrame();
    m_pointIcon->retain();
    m_points = Node::create();
    addChild(m_points);
    return true;
}

void MapPointsLayer::saveActivePointAsUserPoint()
{
    MapPoint p;
    p.point = m_widget->getPosition();
    
    // Map doesn't use coordinate points of fraction of 1
    // so we can round it up to int;
    p.point.x = roundf(p.point.x);
    p.point.y = roundf(p.point.y);
    p.name = ((ui::EditBox*)m_widget)->getText();
    
    // Save only points with text
    if(p.name.length())
    {
        MapViewLayer::Context.PointsLoader->addPoint(p);
    }
    addPoint(&p);
}

void MapPointsLayer::createUserPoint(const Vec2& p)
{
    Vec2 point = convertToNodeSpace(p);
    
    ui::EditBox* box = ui::EditBox::create(Size(160, 140), s_popupSprite);
    //box->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    box->setAnchorPoint({0.5, 0.5});
    box->setScale(1.0/m_parentScale);
    box->setFontColor(s_textColor);
    box->setPosition(point);
    box->setFontSize(s_textSize);
    addChild(box);
    m_widget = box;
}

void MapPointsLayer::closeActivePoint()
{
    if(!m_hasActivePoint)
    {
        return;
    }
    
    if(m_newPoint)
    {
        saveActivePointAsUserPoint();
        m_newPoint = false;
    }
    removeChild(m_widget);
    m_widget = nullptr;
    m_hasActivePoint = false;
    m_points->setVisible(true);
}

void MapPointsLayer::openPoint(const MapPoint& point)
{
    ui::Button* button = ui::Button::create(s_popupSprite, s_popupSprite, s_popupSprite);
    button->setEnabled(false);
    button->setTitleText(point.name);
    button->setPosition(point.point);
    button->setSize(s_boxSize);
    button->setScale9Enabled(true);
    button->setAnchorPoint({0.5, 0.5});
    button->setScale(1.0/m_parentScale);
    button->setOpacity(190);
    button->setTitleColor(s_textColor);
    button->setTitleFontSize(s_textSize);
    auto size = button->getTitleRenderer()->getContentSize();
    button->setContentSize(size + s_boxPadding);
    
    m_widget = button;
    addChild(button);
    m_hasActivePoint = true;
    m_points->setVisible(false);
}

void MapPointsLayer::createPoint(const Vec2& _position)
{
    if(m_hasActivePoint)
    {
        return;
    }
    
    createUserPoint(_position);
    m_newPoint = true;
    m_hasActivePoint = true;
}

MapPoint* MapPointsLayer::tryOpenPoint(const Vec2& _position)
{
    if(m_hasActivePoint)
    {
        return nullptr;
    }
    
    Vec2 position = convertToNodeSpace(_position);
    MapPoint* p = MapViewLayer::Context.PointsLoader->findNearestPointWithinRadius(position,s_touchRadius / m_parentScale);
    if(p)
    {
        openPoint(*p);
    }
    return p;
}

void MapPointsLayer::onTap(const Vec2 &point)
{
    if(m_hasActivePoint)
    {
        closeActivePoint();
    }
    else
    {
        tryOpenPoint(point);
    }
}

void MapPointsLayer::onLongTap(const Vec2 &point)
{
    if(m_hasActivePoint)
    {
        closeActivePoint();
    }
    else
    {
        MapPoint* p = tryOpenPoint(point);
        if(m_hasActivePoint)
        {
            editPoint(p);
        }
        else
        {
            createPoint(point);
        }
    }
}

void MapPointsLayer::editPoint(MapPoint* p)
{
    ;
}

void MapPointsLayer::onParentScaleChanged(float scale)
{
    m_parentScale = scale;
    
    float scaleToSet = 1.0/m_parentScale;
    if(m_hasActivePoint)
    {
        m_widget->setScale(scaleToSet);
    }
    for(const auto c : m_points->getChildren())
    {
        c->setScale(scaleToSet);
    }
}

void MapPointsLayer::onViewChanged(const Coordinate& a, const Coordinate& b)
{
    if(m_hasActivePoint)
    {
        // don't draw points
        return;
    }
    
    // TODO: update point on demand every half a second or so
    // will not significantly affect UX but will improve performance
    // Don't forget to resize all children of m_points
    
    Coordinate center = (a + b)/2;
    float radius = center.distance(a);
    std::list<MapPoint*> list;
    MapViewLayer::Context.PointsLoader->findAllPointsInRadius(center, radius, list);
    
    m_points->removeAllChildren();
    for(auto p : list)
    {
        addPoint(p);
    }
}

void MapPointsLayer::addPoint(MapPoint* point)
{
    Sprite* s = Sprite::createWithSpriteFrame(m_pointIcon);
    s->setAnchorPoint({0.5, 0.5});
    s->setPosition(point->point);
    s->setScale(1.0/m_parentScale);
    s->setColor(Color3B::RED);
    m_points->addChild(s);
}
