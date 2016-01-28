//
//  TextBubble.cpp
//  NewYorkAtoB
//
//  Created by teivaz on 1/28/16.
//
//

#include "TextBubble.h"

USING_NS_CC;

TextBubble::TextBubble()
{
    
}

bool TextBubble::init(const std::string& text)
{
    m_text = text;
    return true;
}

TextBubble* TextBubble::create(const std::string& text)
{
    TextBubble* t = new TextBubble();
    t->autorelease();
    if(t->init(text));
    {
        return t;
    }
    return nullptr;
}
