#ifndef __BannersTestScene_h__
#define __BannersTestScene_h__

#include <iostream>
#include <string>
#include "cocos2d.h"

using namespace cocos2d;
using std::string;
class BannersTestScene : public LayerColor
{
protected:
	void showInGameMenuLayer();

	// Ingame menu Callbacks
	void menuButtonCallback(Ref* sender);

	void showBannerTopMiddle(Ref* sender);
    void showBannerTopLeft(Ref* sender);
    void showBannerTopRight(Ref* sender);

    void showBannerBottomMiddle(Ref* sender);
    void showBannerBottomLeft(Ref* sender);
    void showBannerBottomRight(Ref* sender);

    void hideBanner(Ref* sender);

    Menu* mMainMenu;

public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::Scene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(BannersTestScene);
};


#endif /* defined(__BannersTestScene_h__) */
