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
class StoreScene : public cocos2d::LayerColor
{
protected:
	void showInGameMenuLayer();

	// Ingame menu Callbacks
	void menuButtonCallback(Ref* sender);

	// Store Callbacks
	void buyNinjaStarsCallback(Ref* sender);
	void buyNinjaSwordCallback(Ref* sender);
	void buySamuraiShieldCallback(Ref* sender);
	void buyShogunKatanaCallback(Ref* sender);
	void hideBuyDialog();
	void showBuyDialog(const string& itemName);

	void buyButtonCallback(Ref* sender);
	void cancelBuyButtonCallback(Ref* sender);
	LayerColor* mBuyDialog;
	Menu* mItemsMenu;

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
	static cocos2d::Scene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(StoreScene);
};

#endif /* defined(__StoreScene_h__) */
