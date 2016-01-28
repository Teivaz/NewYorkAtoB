//
//  MapPointsLayer.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/26/16.
//
//

#include "MapPointsLayer.h"
#include "UIEditBox/UIEditBox.h"
#include "MapPoint.h"
#include "MainScene.h"
#include "PointsLoader.h"

USING_NS_CC;

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
    MapViewLayer::Context.PointsLoader;
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
        ui::EditBox* box = ui::EditBox::create(Size(160, 140), "bubble.9.png");
        box->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        box->setAnchorPoint({0.5, 0.5});
        box->setScale(1.0/m_parentScale);
        //box->closeKeyboard();
        box->setPosition(point);
        box->setFontSize(15);
        //box->label
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

void MapPointsLayer::tryOpenPoint(const Vec2& point)
{
    if(m_hasActivePoint)
    {
        return;
    }
    
    if(m_editMode)
    {
        // in edit mode we create new user point
        createUserPoint(point);
        m_hasActivePoint = true;
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
