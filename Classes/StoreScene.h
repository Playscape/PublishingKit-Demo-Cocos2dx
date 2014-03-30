#ifndef __StoreScene_h__
#define __StoreScene_h__

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
	void hideBuyDialog();
	void showBuyDialog();

	void buyButtonCallback(CCObject* sender);
	void cancelBuyButtonCallback(CCObject* sender);
	CCLayerColor* mBuyDialog;
	CCMenu* mItemsMenu;

	bool mShouldFail;

	void registerStoreFlow();
	void reportStartPurchase();
	void initReportableItems();

	FlowInstance mStoreFlow;

	// Reportable items
	PurchaseItem *mItemNinjaStars;
	PurchaseItem *mItemNinjaSword;
	PurchaseItem *mItemSamuraiShield;
	PurchaseItem *mItemShogunKatana;

	PurchaseItem *mCurrentItemPurchasing;
	double mCurrentItemPrice;

	// stub details map we'll use with various report calls, in a real game this should be relevant data.
	map<string, double> mDummyFlowDetails;
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(StoreScene);
};

#endif /* defined(__StoreScene_h__) */
