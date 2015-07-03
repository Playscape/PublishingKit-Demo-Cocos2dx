#include <sys/time.h>
#include "StoreScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/Report.h"
#include "CCStoreInventory.h"
#include "CCError.h"
#include "CCSoomlaUtils.h"
#include <map>
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
using std::map;

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

    CCMenuItemFont *item1 = CCMenuItemFont::create("$10 Ninja Stars", this,menu_selector(StoreScene::buyNinjaStarsCallback));
    item1->setColor(ccBLACK);

    CCMenuItemFont *item2 = CCMenuItemFont::create("$15 Ninja Sword and Simulate Fail", this,menu_selector(StoreScene::buyNinjaSwordCallback));
    item2->setColor(ccBLACK);

	CCMenuItemFont *item3 = CCMenuItemFont::create("$20 Samurai Shield", this,menu_selector(StoreScene::buySamuraiShieldCallback));
	item3->setColor(ccBLACK);

	CCMenuItemFont *item4 = CCMenuItemFont::create("$25 Shogun Katana", this,menu_selector(StoreScene::buyShogunKatanaCallback));
	item4->setColor(ccBLACK);

    mItemsMenu = CCMenu::create(item1, item2, item3, item4, NULL);

    mItemsMenu->alignItemsVertically();
    addChild(mItemsMenu, 1);

    showInGameMenuLayer();

    mBuyDialog = NULL;
    mShouldFail = false;


    registerStoreFlow();
    initReportableItems();

    Report::getInstance().ReportFlowStep(mStoreFlow , OPEN_STORE_FLOW_STEP, "ok", mDummyFlowDetails);

    return true;
}

void StoreScene::buyNinjaStarsCallback(CCObject* sender) {
	mCurrentItemPurchasing = mItemNinjaStars;
	mCurrentItemPrice = 10.0;

	showBuyDialog("Ninja Stars");
	dbgprint("Buying ninja stars");
}

void StoreScene::buyNinjaSwordCallback(CCObject* sender) {
	mCurrentItemPurchasing = mItemNinjaSword;
	mCurrentItemPrice = 15.0;

	showBuyDialog("Ninja Stars");
	mShouldFail = true;
	dbgprint("Buying ninja sword and failing");
}

void StoreScene::buySamuraiShieldCallback(CCObject* sender) {
	mCurrentItemPurchasing = mItemSamuraiShield;
	mCurrentItemPrice = 20.0;

	showBuyDialog("Samurai Shield");
	dbgprint("Buying samurai shield");
}
void StoreScene::buyShogunKatanaCallback(CCObject* sender) {
	mCurrentItemPurchasing = mItemShogunKatana;
	mCurrentItemPrice = 25.0;

	showBuyDialog("Shogun Katana");
	dbgprint("Shogun katana");
}

void StoreScene::showInGameMenuLayer() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	CCMenuItemFont *menuButton = CCMenuItemFont::create("Menu", this,menu_selector(StoreScene::menuButtonCallback));
	menuButton->setColor(ccc3(0,0,0));

	menuButton->setPosition(ccp(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	CCMenu *pMenu = CCMenu::create(menuButton,NULL);
	pMenu->setPosition(CCPointZero);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void StoreScene::menuButtonCallback(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}


void StoreScene::showBuyDialog(const string& itemName) {
	reportStartPurchase();

	if (mBuyDialog) {
		removeChild(mBuyDialog);
	}

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	mBuyDialog = CCLayerColor::create(ccc4(0xcc, 0xcc, 0xcc, 0xff), (winSize.width * 0.8f), (winSize.height * 0.8f));
	addChild(mBuyDialog);

	string question = string("Are you sure you want to buy ") + itemName + "?";
	CCLabelTTF* questionLabel = CCLabelTTF::create(question.c_str(), "Marker Felt", 26);

	questionLabel->setAnchorPoint(CCPointZero);
	mBuyDialog->setAnchorPoint(CCPointZero);
	mBuyDialog->setPosition(ccp(winSize.width/2 - mBuyDialog->getContentSize().width/2,
								   winSize.height/2 - mBuyDialog->getContentSize().height/2));

	questionLabel->setPosition(ccp(mBuyDialog->getContentSize().width/2 - questionLabel->getContentSize().width/2,
			mBuyDialog->getContentSize().height/2 - questionLabel->getContentSize().height/2));

	CCMenu *pMenu =
		CCMenu::create(
			CCMenuItemFont::create(" Buy ", this,menu_selector(StoreScene::buyButtonCallback)),
			CCMenuItemFont::create(" Cancel", this,menu_selector(StoreScene::cancelBuyButtonCallback)),
			NULL);

	CCObject* item;
	CCARRAY_FOREACH(pMenu->getChildren(), item) {
		((CCMenuItemFont*)item)->setColor(ccBLACK);
	}

	mBuyDialog->addChild(pMenu);
	pMenu->alignItemsHorizontally();
	pMenu->setAnchorPoint(CCPointZero);
	pMenu->setPosition(ccp(mBuyDialog->getPosition().x + mBuyDialog->getContentSize().width/2 , mBuyDialog->getPosition().y));

	mBuyDialog->addChild(questionLabel);


	mItemsMenu->setEnabled(false);
	mItemsMenu->setVisible(false);
	mBuyDialog->setVisible(true);


}



void StoreScene::buyButtonCallback(CCObject* sender) {
	hideBuyDialog();

    string itemId = ((PurchaseItem)*mCurrentItemPurchasing).getName();
    soomla::CCError *soomlaError = NULL;
    soomla::CCStoreInventory::sharedStoreInventory()->buyItem(itemId.c_str(), &soomlaError);
        if (soomlaError) {
            soomla::CCSoomlaUtils::logException("StoreBScene::onBuy", soomlaError);
            return;
        }

/*
	if (mShouldFail) {
		Report::getInstance().ReportFlowStep(mStoreFlow, CLOSED_STORE_FLOW_STEP, "ok", mDummyFlowDetails);

		Report::getInstance().ReportPurchaseStarted(*mCurrentItemPurchasing);
		Report::getInstance().ReportPurchaseFailed(*mCurrentItemPurchasing, "User Canceled");
	} else {
		Report::getInstance().ReportFlowStep(mStoreFlow, PURCHASED_FLOW_STEP, "ok", mDummyFlowDetails);
		Report::getInstance().ReportFlowStep(mStoreFlow, CLOSED_STORE_FLOW_STEP, "ok", mDummyFlowDetails);

		Report::getInstance().ReportPurchaseStarted(*mCurrentItemPurchasing);
		Report::getInstance().ReportPurchaseSuccess(*mCurrentItemPurchasing, mCurrentItemPrice, "USD", currentTimeMillis(), "fake-tranaction-id");
	}
*/
	mShouldFail = false;
}

void StoreScene::cancelBuyButtonCallback(CCObject* sender) {
	hideBuyDialog();
	mShouldFail = false;

	Report::getInstance().ReportFlowStep(mStoreFlow, CANCELLED_FLOW_STEP, "ok", mDummyFlowDetails);
	Report::getInstance().ReportFlowStep(mStoreFlow, CLOSED_STORE_FLOW_STEP, "ok", mDummyFlowDetails);	
}

void StoreScene::hideBuyDialog() {
	mBuyDialog->setVisible(false);
	mItemsMenu->setEnabled(true);
	mItemsMenu->setVisible(true);
}

CCScene* StoreScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    StoreScene *layer = StoreScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


void StoreScene::registerStoreFlow() {
	map<string, int32_t> stepNameToId;
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
	mItemNinjaStars = new PurchaseItem("ninja_stars");
	mItemNinjaSword = new PurchaseItem("ninjs_sword");
	mItemSamuraiShield = new PurchaseItem("samurai_shield");
	mItemShogunKatana = new PurchaseItem("shogun_katana");
}
