//
//  TextBubble.h
//  NewYorkAtoB
//
//  Created by teivaz on 1/28/16.
//
//

#pragma once

#include "cocos2d.h"

class TextBubble : public cocos2d::Node
{
public:
    
    static TextBubble* create(const std::string& text);
    TextBubble();
    bool init(const std::string& text);
    
    
private:
    std::string m_text;
    //cocos2d::Sprite*
};
