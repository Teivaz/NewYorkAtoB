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
    Vec3 offset;
    
    m_transform.getScale(&scale);
    m_transform.getTranslation(&offset);
    
    Vec3 a;
    Vec3 b(getContentSize(), 0);
    Mat4 inv = m_transform.getInversed();
    inv.transformPoint(&a);
    inv.transformPoint(&b);
    
    int lod = lodForScale(scale.x);
    if(lod != m_lod)
    {
        m_lod = lod;
        onLodChanged();
    }
    m_tileLayer->tileRegion({a.x, a.y}, {b.x, b.y}, m_lod);
}

int MapLayer::lodForScale(float scale)
{
    float targetScale = 1 / (CC_CONTENT_SCALE_FACTOR() * scale);
    unsigned int v = targetScale;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    unsigned int l = v >> 1;
    if( (v - targetScale) < (targetScale - l) )
    {
        l = v;
    }
    if(l <= 0) return 0;
    return log2f(l);
}

void MapLayer::onLodChanged()
{
    cocos2d::log("LOD changed to: %d", m_lod);
}
