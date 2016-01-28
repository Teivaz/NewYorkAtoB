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

static const float s_touchRadius = 30;

MapPointsLayer::MapPointsLayer()
: m_editMode(false)
, m_hasActivePoint(false)
, m_widget(nullptr)
, m_parentScale(1.0)
{
    
}

MapPointsLayer::~MapPointsLayer()
{}

bool MapPointsLayer::init()
{
    return true;
}

void MapPointsLayer::LoadPoints()
{
}

void MapPointsLayer::setEditModeEnabled(bool val)
{
    if(m_editMode != val)
    {
        closeActivePoint();
        m_editMode = val;
    }
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
}

void MapPointsLayer::createUserPoint(const Vec2& p)
{
    Vec2 point = convertToNodeSpace(p);
    
    {
        ui::EditBox* box = ui::EditBox::create(Size(160, 140), s_popupSprite);
        box->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        box->setAnchorPoint({0.5, 0.5});
        box->setScale(1.0/m_parentScale);
        box->setFontColor(s_textColor);
        box->setPosition(point);
        box->setFontSize(s_textSize);
        addChild(box);
        m_widget = box;
    }
}

void MapPointsLayer::closeActivePoint()
{
    if(!m_hasActivePoint)
    {
        return;
    }
    
    if(m_editMode)
    {
        saveActivePointAsUserPoint();
    }
    removeChild(m_widget);
    m_widget = nullptr;
    m_hasActivePoint = false;
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
    m_widget = button;
    addChild(button);
    m_hasActivePoint = true;
}

void MapPointsLayer::tryOpenPoint(const Vec2& _position)
{
    if(m_hasActivePoint)
    {
        return;
    }
    
    if(m_editMode)
    {
        // in edit mode we create new user point
        createUserPoint(_position);
        m_hasActivePoint = true;
    }
    else
    {
        Vec2 position = convertToNodeSpace(_position);
        MapPoint* p = MapViewLayer::Context.PointsLoader->findNearestPointWithinRadius(position, s_touchRadius / m_parentScale);
        if(p)
        {
            openPoint(*p);
        }
    }
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

void MapPointsLayer::onParentScaleChanged(float scale)
{
    m_parentScale = scale;
    if(m_hasActivePoint)
    {
        m_widget->setScale(1.0/m_parentScale);
    }
}
