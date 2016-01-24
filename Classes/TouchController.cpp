//
//  TouchController.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/24/16.
//
//

#include "TouchController.h"

USING_NS_CC;


TouchController::~TouchController()
{
    delete m_usedTouches;
}

TouchController::TouchController()
: onPinch(nullptr)
, onPinchEnd(nullptr)
, onDrag(nullptr)
, onDragEnd(nullptr)
, m_state(TouchControllerState::None)
, m_touchCount(0)
, m_usedTouches(nullptr)
{
    
}

bool TouchController::init()
{
    m_usedTouches = new __Set();
    
    EventListenerTouchAllAtOnce* l = EventListenerTouchAllAtOnce::create();
    l->onTouchesBegan = [this](const std::vector<Touch*>& t, Event* e){this->onTouchesBegan(t, e);};
    l->onTouchesMoved = [this](const std::vector<Touch*>& t, Event* e){this->onTouchesMoved(t, e);};
    l->onTouchesEnded = [this](const std::vector<Touch*>& t, Event* e){this->onTouchesEnded(t, e);};
    l->onTouchesCancelled = [this](const std::vector<Touch*>& t, Event* e){this->onTouchesCancelled(t, e);};
    
    _eventDispatcher->addEventListenerWithFixedPriority(l, -255);
    return true;
}

void TouchController::setParent(Node* p)
{
    Layer::setParent(p);
    
    if (p)
    {
        Size size = p->getContentSize();
        setContentSize(size);
        setPosition(p->getPosition());
    }
}

void TouchController::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    int touchCount = m_usedTouches->count();
    if(touchCount == 0){
        // first touch
        if(touches.size() == 1)
        {
            // one touch. Start drag
            beginDrag(touches[0]);
        }
        else
        {
            // multiple touches. Start pinch on first two
            beginPinch(touches[0], touches[1]);
        }
    }
    else if(touchCount == 1)
    {
        // When we have second touch we finish drag and start pinch
        endDrag();
        Touch* firstTouch = (Touch*)(*m_usedTouches->begin());
        beginPinch(firstTouch, touches[0]);
    }
    else
    {
        // ignore all other touches
    }
    
}

void TouchController::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
    bool didActiveTouchMoved = false;
    for(auto touch : touches)
    {
        if(isTouchUsed(touch))
        {
            didActiveTouchMoved = true;
            break;
        }
    }
    
    // our touch was not affected
    if(!didActiveTouchMoved)
    {
        return;
    }
    
    if(m_state == TouchControllerState::Drag)
    {
        onDragMoved();
    }
    else if (m_state == TouchControllerState::Pinch)
    {
        onPinchMoved();
    }
}

void TouchController::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    int oldUsedTouchesNum = m_usedTouches->count();
    for(auto touch : touches)
    {
        m_usedTouches->removeObject(touch);
    }
    
    if(oldUsedTouchesNum == m_usedTouches->count())
    {
        // no changes in the used touches
        return;
    }
    
    if(m_state == TouchControllerState::Drag)
    {
        endDrag();
    }
    else if (m_state == TouchControllerState::Pinch)
    {
        endPinch();
        
        // If we have one touch then we need to start drag
        if(m_usedTouches->count() == 1)
        {
            beginDrag((Touch*)(*m_usedTouches->begin()));
        }
    }
}

void TouchController::onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event)
{
    
}

void TouchController::beginDrag(Touch* touch)
{
    // If touch is left from the previous gesture reset it and treat as a new one
    if(isTouchUsed(touch))
    {
        touch->resetTouch();
    }
    m_state = TouchControllerState::Drag;
    startUsingTouch(touch);
}

void TouchController::endDrag()
{
    m_state = TouchControllerState::None;
    if(onDragEnd)
    {
        onDragEnd();
    }
}

void TouchController::beginPinch(Touch* touch1, Touch* touch2)
{
    // If touch is left from the previous gesture reset it and treat as a new one
    if(isTouchUsed(touch1))
    {
        touch1->resetTouch();
    }
    if(isTouchUsed(touch2))
    {
        touch2->resetTouch();
    }
    
    m_state = TouchControllerState::Pinch;
    startUsingTouch(touch1);
    startUsingTouch(touch2);
}

void TouchController::endPinch()
{
    m_state = TouchControllerState::None;
    if(onPinchEnd)
    {
        onPinchEnd();
    }
}

bool TouchController::isTouchUsed(Touch* touch)
{
    return m_usedTouches->containsObject(touch);
}

void TouchController::startUsingTouch(Touch* touch)
{
    m_usedTouches->addObject(touch);
}

void TouchController::onDragMoved()
{
    auto iterator = m_usedTouches->begin();
    Touch* t = (Touch*)(*iterator);
    
    if(onDrag)
    {
        onDrag(t->getLocation() - t->getStartLocation());
    }
}

void TouchController::onPinchMoved()
{
    auto iterator = m_usedTouches->begin();
    Touch* t1 = (Touch*)(*iterator++);
    Touch* t2 = (Touch*)(*iterator);
    
    if(onPinch)
    {
        Vec2 mid = (t1->getLocation() + t2->getLocation()) / 2;
        Vec2 midStart = (t1->getStartLocation() + t2->getStartLocation()) / 2;
        Vec2 moveDelta = mid - midStart;
        
        Vec2 distance(t1->getLocation(), t2->getLocation());
        Vec2 distanceStart(t1->getStartLocation(), t2->getStartLocation());
        float scale = distance.lengthSquared() / distanceStart.lengthSquared();
        scale = sqrtf(scale);
        
        onPinch(moveDelta, scale, mid);
    }
}
