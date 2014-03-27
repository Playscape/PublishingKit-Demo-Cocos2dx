#include "StoreScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"

using namespace cocos2d;

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
    return true;
}

void StoreScene::buyNinjaStarsCallback(CCObject* sender) {
	showBuyDialog("Ninja Stars");
	dbgprint("Buying ninja stars");
}

void StoreScene::buyNinjaSwordCallback(CCObject* sender) {
	showBuyDialog("Ninja Stars");
	mShouldFail = true;
	dbgprint("Buying ninja sword and failing");
}

void StoreScene::buySamuraiShieldCallback(CCObject* sender) {
	showBuyDialog("Samurai Shield");
	dbgprint("Buying samurai shield");
}
void StoreScene::buyShogunKatanaCallback(CCObject* sender) {
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
	if (mBuyDialog) {
		mBuyDialog->release();
	}

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	mBuyDialog = CCLayerColor::create(ccc4(0xcc, 0xcc, 0xcc, 0xff), (winSize.width * 0.8f), (winSize.height * 0.8f));
	mBuyDialog->retain();
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

	if (mShouldFail) {
		dbgprint("purchase FAILED!");
	}
	mShouldFail = false;
}

void StoreScene::cancelBuyButtonCallback(CCObject* sender) {
	hideBuyDialog();
	mShouldFail = false;
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

