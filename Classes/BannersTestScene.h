#ifndef __BannersTestScene_h__
#define __BannersTestScene_h__

#include <iostream>
#include <string>
#include "cocos2d.h"

using namespace cocos2d;
using std::string;
class BannersTestScene : public CCLayerColor
{
protected:
	void showInGameMenuLayer();

	// Ingame menu Callbacks
	void menuButtonCallback(CCObject* sender);

	void showBannerTopMiddle(CCObject* sender);
    void showBannerTopLeft(CCObject* sender);
    void showBannerTopRight(CCObject* sender);

    void showBannerBottomMiddle(CCObject* sender);
    void showBannerBottomLeft(CCObject* sender);
    void showBannerBottomRight(CCObject* sender);
    
    void hideBanner(CCObject* sender);

    CCMenu* mMainMenu;
	
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(BannersTestScene);
};


#endif /* defined(__BannersTestScene_h__) */
