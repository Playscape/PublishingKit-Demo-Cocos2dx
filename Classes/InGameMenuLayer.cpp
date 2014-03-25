#include "InGameMenuLayer.h"

// on "init" you need to initialize your instance
bool InGameMenuLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(cocos2d::ccc4(255, 255, 255, 255)) )
    {
        return false;
    }
    
    setTouchEnabled(true);
    
    return true;
}
