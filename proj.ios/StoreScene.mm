#include <sys/time.h>
#include "StoreScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/Report.h"

#import "IAPurchases.h"
#import <StoreKit/StoreKit.h>

/// <summary>
/// Analytics flow type for the store
/// </summary>
#define STORE_FLOW_TYPE "Store"

#define OPEN_STORE_FLOW_STEP      "OpenStore"
#define SELECT_CATEGORY_FLOW_STEP "SelectCategory"
#define SELECT_ITEM_FLOW_STEP     "SelectItem"
#define PURCHASED_FLOW_STEP       "Purchased"
#define CANCELLED_FLOW_STEP       "Cancelled"
#define CLOSED_STORE_FLOW_STEP    "ClosedStore"

using namespace cocos2d;
using playscape::Report;


static int64_t currentTimeMillis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t timeInMs = (((int64_t)tv.tv_sec) * ((int64_t)1000)) + (((int64_t)tv.tv_usec) / ((int64_t)1000));
    return timeInMs;
}

// on "init" you need to initialize your instance
bool StoreScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(cocos2d::ccc4(255, 255, 255, 255)) )
    {
        return false;
    }
    
    setTouchEnabled(true);
    
    Vector<MenuItem*> *vector = new Vector<MenuItem*>;
    
    IAPurchases *instance = [IAPurchases sharedInstance];
    [instance requestProductsWithCompletionHandler:^(BOOL success, NSArray *products) {
        if (success) {
            for (SKProduct *skProduct in products) {
                if ([skProduct.localizedTitle isEqualToString:@"Ninja Stars"]) {
                    MenuItemFont *item = MenuItemFont::create("$10 Ninja Stars", CC_CALLBACK_1(StoreScene::buyNinjaStarsCallback, this));
                    item->setColor(Color3B::BLACK);
                    vector->pushBack(item);
                } else if ([skProduct.localizedTitle isEqualToString:@"Ninja Sword and Simulate Fail"]) {
                    MenuItemFont *item = MenuItemFont::create("$15 Ninja Sword and Simulate Fail", CC_CALLBACK_1(StoreScene::buyNinjaSwordCallback, this));
                    item->setColor(Color3B::BLACK);
                    vector->pushBack(item);
                } else if ([skProduct.localizedTitle isEqualToString:@"Samurai Shield"]) {
                    MenuItemFont *item = MenuItemFont::create("$20 Samurai Shield", CC_CALLBACK_1(StoreScene::buySamuraiShieldCallback, this));
                    item->setColor(Color3B::BLACK);
                    vector->pushBack(item);
                } else if ([skProduct.localizedTitle isEqualToString:@"Shogun Katana"]) {
                    MenuItemFont *item = MenuItemFont::create("$25 Shogun Katana", CC_CALLBACK_1(StoreScene::buyShogunKatanaCallback, this));
                    item->setColor(Color3B::BLACK);
                    vector->pushBack(item);
                }
            }
        } else {
            MenuItemFont *item = MenuItemFont::create("No one product is available");
            item->setColor(Color3B::BLACK);
            item->setFontSize(40);
            vector->pushBack(item);
     
        }
    
        mItemsMenu = CCMenu::createWithArray(*vector);
    
        mItemsMenu->alignItemsVertically();
        addChild(mItemsMenu, 1);
    
        showInGameMenuLayer();
    
        mBuyDialog = NULL;
        mShouldFail = false;
     
        registerStoreFlow();
        initReportableItems();
     
        Report::getInstance().ReportFlowStep(mStoreFlow , OPEN_STORE_FLOW_STEP, "ok", mDummyFlowDetails);
    }];

    return true;
}

void StoreScene::buyNinjaStarsCallback(Ref* sender) {
	mCurrentItemPurchasing = mItemNinjaStars;
	mCurrentItemPrice = 10.0;

	showBuyDialog("Ninja Stars");
	dbgprint("Buying ninja stars");
}

void StoreScene::buyNinjaSwordCallback(Ref* sender) {
	mCurrentItemPurchasing = mItemNinjaSword;
	mCurrentItemPrice = 15.0;

	showBuyDialog("Ninja Stars");
	mShouldFail = true;
	dbgprint("Buying ninja sword and failing");
}

void StoreScene::buySamuraiShieldCallback(Ref* sender) {
	mCurrentItemPurchasing = mItemSamuraiShield;
	mCurrentItemPrice = 20.0;

	showBuyDialog("Samurai Shield");
	dbgprint("Buying samurai shield");
}
void StoreScene::buyShogunKatanaCallback(Ref* sender) {
	mCurrentItemPurchasing = mItemShogunKatana;
	mCurrentItemPrice = 25.0;

	showBuyDialog("Shogun Katana");
	dbgprint("Shogun katana");
}

void StoreScene::showInGameMenuLayer() {
    cocos2d::Size winSize = Director::getInstance()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	MenuItemFont *menuButton = MenuItemFont::create("Menu", CC_CALLBACK_1(StoreScene::menuButtonCallback, this));
    menuButton->setColor(Color3B::BLACK);

	menuButton->setPosition(Vec2(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	Menu *pMenu = Menu::create(menuButton, NULL);
    pMenu->setPosition(Vec2::ZERO);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void StoreScene::menuButtonCallback(Ref* sender) {
	Director::getInstance()->replaceScene(HelloWorld::scene());
}


void StoreScene::showBuyDialog(const std::string& itemName) {
	reportStartPurchase();

	if (mBuyDialog) {
		removeChild(mBuyDialog);
    }

    
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
	mBuyDialog = LayerColor::create(Color4B(0xcc, 0xcc, 0xcc, 0xff), (winSize.width * 0.8f), (winSize.height * 0.8f));
	addChild(mBuyDialog);

    std::string question = std::string("Are you sure you want to buy ") + itemName + "?";
	auto questionLabel = Label::createWithTTF(question.c_str(), "Marker Felt", 26);

    questionLabel->setAnchorPoint(Vec2::ZERO);
    mBuyDialog->setAnchorPoint(Vec2::ZERO);
	mBuyDialog->setPosition(Vec2(winSize.width/2 - mBuyDialog->getContentSize().width/2,
								   winSize.height/2 - mBuyDialog->getContentSize().height/2));

	questionLabel->setPosition(Vec2(mBuyDialog->getContentSize().width/2 - questionLabel->getContentSize().width/2,
			mBuyDialog->getContentSize().height/2 - questionLabel->getContentSize().height/2));

    MenuItemFont *buyItem = MenuItemFont::create("Buy", CC_CALLBACK_1(StoreScene::buyButtonCallback, this));
    buyItem->setColor(Color3B::BLACK);
    buyItem->setFontSizeObj(50);
    
    MenuItemFont *cancelItem = MenuItemFont::create("Cancel", CC_CALLBACK_1(StoreScene::cancelBuyButtonCallback, this));
    cancelItem->setColor(Color3B::BLACK);
    cancelItem->setFontSizeObj(50);
    
	Menu *pMenu = Menu::create(buyItem, cancelItem, NULL);

    for (auto item : pMenu->getChildren()) {
        ((MenuItemFont*)item)->setColor(Color3B::BLACK);
    }

	mBuyDialog->addChild(pMenu);
	pMenu->alignItemsHorizontally();
    pMenu->setAnchorPoint(Vec2::ZERO);
	pMenu->setPosition(Vec2(mBuyDialog->getPosition().x + mBuyDialog->getContentSize().width/2 , mBuyDialog->getPosition().y));

	mBuyDialog->addChild(questionLabel);


	mItemsMenu->setEnabled(false);
	mItemsMenu->setVisible(false);
	mBuyDialog->setVisible(true);
}



void StoreScene::buyButtonCallback(Ref* sender) {
//	hideBuyDialog();


	if (mShouldFail) {
		Report::getInstance().ReportFlowStep(mStoreFlow, CLOSED_STORE_FLOW_STEP, "ok", mDummyFlowDetails);

//		Report::getInstance().ReportPurchaseStarted(*mCurrentItemPurchasing);
//		Report::getInstance().ReportPurchaseFailed(*mCurrentItemPurchasing, "User Canceled");
	} else {
		Report::getInstance().ReportFlowStep(mStoreFlow, PURCHASED_FLOW_STEP, "ok", mDummyFlowDetails);
		Report::getInstance().ReportFlowStep(mStoreFlow, CLOSED_STORE_FLOW_STEP, "ok", mDummyFlowDetails);

        IAPurchases *instance = [IAPurchases sharedInstance];
        NSString *productName = [NSString stringWithCString:mCurrentItemPurchasing->getName().c_str() encoding:NSUTF8StringEncoding];
        [instance buyProduct:productName];
        
//		Report::getInstance().ReportPurchaseStarted(*mCurrentItemPurchasing);
//		Report::getInstance().ReportPurchaseSuccess(*mCurrentItemPurchasing, mCurrentItemPrice, "USD", currentTimeMillis(), "fake-tranaction-id");
	}

	mShouldFail = false;
}

void StoreScene::cancelBuyButtonCallback(Ref* sender) {
	hideBuyDialog();
	mShouldFail = false;

	Report::getInstance().ReportFlowStep(mStoreFlow, CANCELLED_FLOW_STEP, "ok", mDummyFlowDetails);
	Report::getInstance().ReportFlowStep(mStoreFlow, CLOSED_STORE_FLOW_STEP, "ok", mDummyFlowDetails);

//	Report::getInstance().ReportPurchaseStarted(*mCurrentItemPurchasing);
//	Report::getInstance().ReportPurchaseCancelled(*mCurrentItemPurchasing);

}

void StoreScene::hideBuyDialog() {
	mBuyDialog->setVisible(false);
	mItemsMenu->setEnabled(true);
	mItemsMenu->setVisible(true);
}

Scene* StoreScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();

    // 'layer' is an autorelease object
    StoreScene *layer = StoreScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


void StoreScene::registerStoreFlow() {
    std::map<std::string, int32_t> stepNameToId;
	stepNameToId [OPEN_STORE_FLOW_STEP] = 1;
	stepNameToId [SELECT_CATEGORY_FLOW_STEP] = 2;
	stepNameToId [SELECT_ITEM_FLOW_STEP] = 3;
	stepNameToId [PURCHASED_FLOW_STEP] = 4;
	stepNameToId [CANCELLED_FLOW_STEP] = 5;
	stepNameToId[CLOSED_STORE_FLOW_STEP] = 6;

	Report::getInstance().RegisterFlow(STORE_FLOW_TYPE, stepNameToId);
	mStoreFlow = Report::getInstance().StartNewFlow(STORE_FLOW_TYPE);

	// In production these values will probably be relevant, this is just an example details map that we put alongside each report
	mDummyFlowDetails["PremiumCurrency"] = 1000;
	mDummyFlowDetails["TrashCurrency"] = 200;
	mDummyFlowDetails["Category"] = 1;
	mDummyFlowDetails["ItemID"] = 2;
	mDummyFlowDetails["Source"] = 42;
}

void StoreScene::reportStartPurchase() {

	Report::getInstance().ReportFlowStep(mStoreFlow, SELECT_CATEGORY_FLOW_STEP, "ok", mDummyFlowDetails);
	Report::getInstance().ReportFlowStep(mStoreFlow, SELECT_ITEM_FLOW_STEP, "ok", mDummyFlowDetails);

}

void StoreScene::initReportableItems() {
//	mItemNinjaStars = new PurchaseItem("com.playscape.cocos2dxdemo.ninja_stars_item");
//	mItemNinjaSword = new PurchaseItem("com.playscape.cocos2dxdemo.ninja_sword_item");
//	mItemSamuraiShield = new PurchaseItem("com.playscape.cocos2dxdemo.samurai_shield_item");
//	mItemShogunKatana = new PurchaseItem("com.playscape.cocos2dxdemo.shogun_katana_item");
}
