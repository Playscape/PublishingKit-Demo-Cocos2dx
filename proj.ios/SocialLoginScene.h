#ifndef __SocialLoginScene_h__
#define __SocialLoginScene_h__

#include <iostream>
#include <string>
#include <map>
#include "cocos2d.h"
#include "playscape/PurchaseItem.h"
#include "playscape/FlowInstance.h"

using playscape::FlowInstance;
using playscape::PurchaseItem;
using std::string;
using std::map;
using namespace cocos2d;
class SocialLoginScene : public cocos2d::CCLayerColor
{
protected:
	void showInGameMenuLayer();

	// Ingame menu Callbacks
	void menuButtonCallback(CCObject* sender);

	// Social Callbacks
	void loginButtonCallback(CCObject* sender);
	void logoutButtonCallback(CCObject* sender);
    
    void nativeShareDialogButtonCallback(CCObject* sender);
    void shareDialogButtonCallback(CCObject* sender);
    
    void submitScoreButtonCallback(CCObject* sender);
    void sendRequestButtonCallback(CCObject* sender);
	void showLoginDialog();

	void okButtonCallback(CCObject* sender);
	CCLayerColor* mLoginInfoDialog;
	CCMenu* mItemsMenu;

public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(SocialLoginScene);
};

#endif /* defined(__StoreScene_h__) */
