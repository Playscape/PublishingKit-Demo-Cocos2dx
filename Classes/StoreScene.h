#ifndef __StoreScene_h__
#define __StoreScene_h__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;
class StoreScene : public cocos2d::CCLayerColor
{
protected:
	void showInGameMenuLayer();

	// Ingame menu Callbacks
	void menuButtonCallback(CCObject* sender);

	// Store Callbacks
	void buyNinjaStarsCallback(CCObject* sender);
	void buyNinjaSwordCallback(CCObject* sender);
	void buySamuraiShieldCallback(CCObject* sender);
	void buyShogunKatanaCallback(CCObject* sender);

public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(StoreScene);
};

#endif /* defined(__StoreScene_h__) */
