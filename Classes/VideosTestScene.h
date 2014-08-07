#ifndef __VideosTestScene_h__
#define __VideosTestScene_h__

#include <iostream>
#include <string>
#include "cocos2d.h"

using namespace cocos2d;
using std::string;
class VideosTestScene : public CCLayerColor
{
protected:
	void showInGameMenuLayer();

	// Ingame menu Callbacks
	void menuButtonCallback(CCObject* sender);

	void showVideo(CCObject* sender);
	void showIncentivisedVideo(CCObject* sender);


    CCMenu* mMainMenu;
	
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(VideosTestScene);
};


#endif /* defined(__VideosTestScene_h__) */
