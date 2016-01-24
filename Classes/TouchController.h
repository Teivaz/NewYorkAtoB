//
//  TouchController.hpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/24/16.
//
//

#include "cocos2d.h"

enum class TouchControllerState
{
    None,
    Drag,
    Pinch
};

class TouchController: public cocos2d::Layer{
public:
    TouchController();
    ~TouchController();
    bool init() override;
    CREATE_FUNC(TouchController);
    
    virtual void setParent(cocos2d::Node* parent) override;
    
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event) override;
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event) override;
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event) override;
    virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>&touches, cocos2d::Event *unused_event) override;
    
    typedef std::function<void(const cocos2d::Vec2&, float, const cocos2d::Vec2&)> PinchCallback;
    typedef std::function<void(const cocos2d::Vec2&)> DragCallback;
    typedef std::function<void()> VoidCallback;
    
    PinchCallback onPinch;
    VoidCallback onPinchEnd;
    DragCallback onDrag;
    VoidCallback onDragEnd;
    
private:
    void beginDrag(cocos2d::Touch* touch);
    void onDragMoved();
    void endDrag();
    
    void beginPinch(cocos2d::Touch* touch1, cocos2d::Touch* touch2);
    void onPinchMoved();
    void endPinch();
    
    bool isTouchUsed(cocos2d::Touch* touch);
    void startUsingTouch(cocos2d::Touch* touch);
private:
    TouchControllerState m_state;
    int m_touchCount;
    cocos2d::__Set* m_usedTouches;
};
