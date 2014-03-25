#ifndef __InGameMenuLayer__h__
#define __InGameMenuLayer__h__

#include <iostream>
#include "cocos2d.h"

class InGameMenuLayer : public cocos2d::CCLayerColor
{
protected:
    
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(InGameMenuLayer);
};



#endif // __InGameMenuLayer__h__
