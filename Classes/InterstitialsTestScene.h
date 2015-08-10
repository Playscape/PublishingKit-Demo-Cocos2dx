#ifndef __InterstitialsTestScene_h__
#define __InterstitialsTestScene_h__

#include <iostream>
#include <string>
#include "cocos2d.h"

using namespace cocos2d;
using std::string;
class InterstitialsTestScene : public LayerColor
{
protected:
	void showInGameMenuLayer();

	// Ingame menu Callbacks
	void menuButtonCallback(Ref* sender);

	void showInterstitialOverlay(Ref* sender);
    void showInterstitialNonOverlay(Ref* sender);
    void showInterstitialOverlayOrNonOverlay(Ref* sender);

    Menu* mMainMenu;

public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::Scene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(InterstitialsTestScene);
};


#endif /* defined(__InterstitialsTestScene_h__) */
