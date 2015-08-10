#ifndef __FacebookTestScene_h__
#define __FacebookTestScene_h__

#include <iostream>
#include <string>
#include "cocos2d.h"


using namespace cocos2d;
using std::string;
class FacebookTestScene : public LayerColor
{
protected:
	void showInGameMenuLayer();

	// Ingame menu Callbacks
	void menuButtonCallback(Ref* sender);

	void login(Ref* sender);
    void logout(Ref* sender);
    void share(Ref* sender);

    Menu* mMainMenu;

public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::Scene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(FacebookTestScene);
};


#endif /* defined(__FacebookTestScene_h__) */
