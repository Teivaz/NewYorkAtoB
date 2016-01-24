//
//  MapLayer.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/19/16.
//
//

#include "MapLayer.h"

#include "MapTileLayer.h"

USING_NS_CC;

bool MapLayer::init(){
    m_lod = 0;
    //setAnchorPoint({0.5, 0.5});
    
    m_tileLayer = MapTileLayer::create();
    addChild(m_tileLayer);
    m_tileLayer->setPosition(cocos2d::Vec2::ZERO);
    m_adjustedScale = 1;
    m_scale = 1;
    return true;
}

// size of the view in pixels
void MapLayer::setViewSize(const cocos2d::Size& size)
{
    setContentSize(size);
    m_tileLayer->setPosition(cocos2d::Vec2(0, size.height));
    m_tileLayer->setAnchorPoint({0, 1});
    rebuildMap();
}

void MapLayer::applyAdjust()
{
    Vec3 translation;
    Quaternion rotation;
    Vec3 scale;
    m_transform.decompose(&scale, &rotation, &translation);
    m_position = {translation.x, translation.y};
    m_scale = scale.x;
    m_adjustedPosition = Vec2::ZERO;
    m_adjustedScale = 1;
    m_adjustedScalePivot = Vec2::ZERO;
}

// sets the center point of the map in coordinates
void MapLayer::setMapFocus(const Coordinate& center)
{
    
}


void MapLayer::calculateTransformation()
{
    m_transform = Mat4::IDENTITY;
    
    m_transform.translate({m_adjustedScalePivot, 0});
    m_transform.scale(m_adjustedScale);
    m_transform.translate({-m_adjustedScalePivot, 0});
    m_transform.translate({m_adjustedPosition, 0});
    m_transform.translate({m_position, 0});
    m_transform.scale(m_scale);
    
    Vec3 offset;
    Vec3 scale;
    m_transform.getScale(&scale);
    m_transform.getTranslation(&offset);
    setScale(scale.x);
    setPosition(Vec2(offset.x, offset.y));
}

void MapLayer::adjustScale(float s, const cocos2d::Vec2& pivot)
{
    m_adjustedScale = s;
    m_adjustedScalePivot = pivot;
    calculateTransformation();
    rebuildMap();
}

void MapLayer::adjustPosition(const cocos2d::Vec2& pos)
{
    m_adjustedPosition = pos;
    calculateTransformation();
    rebuildMap();
}

void MapLayer::rebuildMap()
{
    Vec3 scale;
    
    m_transform.getScale(&scale);
    
    
    // create a layer for tiles in coordinates that will overlap view in coordinates
    // calculate lod level for
    
    //setScale(1/4.0);
    CoordinateRegion region;
    cocos2d::Rect rect(getPosition(), getContentSize());
    rect.size = rect.size / scale.x;

    //rect.origin.y -= rect.size.height;
    
    //rect.size = rect.size;
    m_lod = 0;
    m_tileLayer->tileRegion(rect, m_lod);
}

void MapLayer::onLodChanged()
{
    //
}
