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
    m_tileLayer->setPosition(Vec2::ZERO);
    m_adjustedScale = 1;
    m_scale = 1;
    
    m_enableBounds = false;
    m_boundA;
    m_boundB;
    m_minScale = 0;
    m_maxScale = 1;
    
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
    Vec3 scale;
    m_transform.decompose(&scale, nullptr, &translation);
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
    
    // adjust scale around pivot
    m_transform.translate({m_adjustedScalePivot, 0});
    m_transform.scale(m_adjustedScale);
    m_transform.translate({-m_adjustedScalePivot, 0});
    
    // adjust position
    m_transform.translate({m_adjustedPosition, 0});
    
    // apply base position and scale
    m_transform.translate({m_position, 0});
    m_transform.scale(m_scale);
}

void MapLayer::applyBounds(Vec3 offset, float scale)
{
    bool scaleBound = false;
    if(scale < m_minScale)
    {
        // m_minScale == m_adjustedScale * m_scale
        m_adjustedScale = m_minScale / m_scale;
        scaleBound = true;
    }
    else if(scale > m_maxScale)
    {
        m_adjustedScale = m_maxScale / m_scale;
        scaleBound = true;
    }
    if(scaleBound)
    {
        Vec3 vecScale;
        calculateTransformation();
        m_transform.decompose(&vecScale, nullptr, &offset);
        scale = vecScale.x;
    }
    
    Vec2 center((-offset.x + getContentSize().width / 2) / scale,
                (-offset.y + getContentSize().height / 2) / scale);
    
    Vec3 delta;
    bool bound = false;

    if(center.x < m_boundA.x)
    {
        delta.x = m_boundA.x - center.x;
        bound = true;
    }
    else if(center.x > m_boundB.x)
    {
        delta.x = m_boundB.x - center.x;
        bound = true;
    }
    
    if(center.y < m_boundA.y)
    {
        delta.y = m_boundA.y - center.y;
        bound = true;
    }
    else if(center.y > m_boundB.y)
    {
        delta.y = m_boundB.y - center.y;
        bound = true;
    }
    if(!bound)
    {
        return;
    }
    
    delta *= scale;
    Vec3 newOffset = offset - delta;
    Mat4 m;
    m.translate({m_adjustedScalePivot, 0});
    m.scale(m_adjustedScale);
    m.translate({-m_adjustedScalePivot, 0});
    m.inverse();
    m.transformPoint(&newOffset);
    
    Mat4 m3;
    m3.translate({m_position, 0});
    m3.scale(m_scale);
    Vec3 a;
    m3.transformPoint(&a);
    a = newOffset - a;
    m_adjustedPosition.x = a.x;
    m_adjustedPosition.y = a.y;
    
    calculateTransformation();
}

void MapLayer::adjustScaleAndPosition(float s, const cocos2d::Vec2& pivot, const cocos2d::Vec2& position)
{
    m_adjustedScale = s;
    m_adjustedPosition = position;
    m_adjustedScalePivot = pivot;
    rebuildMap();
}

void MapLayer::adjustScale(float s, const Vec2& pivot)
{
    m_adjustedScale = s;
    m_adjustedScalePivot = pivot;
    rebuildMap();
}

void MapLayer::adjustPosition(const cocos2d::Vec2& pos)
{
    m_adjustedPosition = pos;
    rebuildMap();
}

void MapLayer::rebuildMap()
{
    calculateTransformation();
    
    // decompose matrix and apply
    Vec3 offset;
    Vec3 scale;
    m_transform.decompose(&scale, nullptr, &offset);
    
    if(m_enableBounds)
    {
        applyBounds(offset, scale.x);
        m_transform.decompose(&scale, nullptr, &offset);
    }
    //m_position.x = offset.x;
    //m_position.y = offset.y;
    //m_adjustedPosition = Vec2::ZERO;
    
    setScale(scale.x);
    setPosition(Vec2(offset.x, offset.y));
    
    Vec3 a(0, 0, 0);
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
    // Take into account device content scale factor
    float targetScale = 1 / (CC_CONTENT_SCALE_FACTOR() * scale);
    
    // Find next POT
    unsigned int v = targetScale;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    
    // Find previous POT
    unsigned int l = v >> 1;

    // Choose closest
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

void MapLayer::setBounds(const Coordinate& a, const Coordinate& b, float minScale, float maxScale)
{
    m_boundA = a;
    m_boundB = b;
    m_minScale = minScale;
    m_maxScale = maxScale;
}
