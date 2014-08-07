#include "AdsTestScene.h"
#include "VideosTestScene.h"
#include "BannersTestScene.h"
#include "InterstitialsTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"


#include <string>

using namespace cocos2d;
using std::string;

// on "init" you need to initialize your instance
bool AdsTestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(cocos2d::ccc4(255, 255, 255, 255)) )
    {
        return false;
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    setTouchEnabled(true);
    CCLabelTTF* label = CCLabelTTF::create("Ads Test", "Marker Felt", 22);

    label->setPosition(ccp(winSize.width/2-label->getContentSize().width/2, winSize.height-label->getContentSize().height));

    mMainMenu =
		CCMenu::create(
			CCMenuItemFont::create("Open Banners Test", this,menu_selector(AdsTestScene::openBannersTest)),
			CCMenuItemFont::create("Open Video Test", this,menu_selector(AdsTestScene::openVideoTest)),
			CCMenuItemFont::create("Open Interstitial Test", this,menu_selector(AdsTestScene::openInterstitialTest)),
			NULL);

    CCObject* item;
    CCARRAY_FOREACH(mMainMenu->getChildren(), item) {
    	((CCMenuItemFont*)item)->setColor(ccBLACK);
    }

    mMainMenu->alignItemsVertically();

    addChild(mMainMenu, 1);
    addChild(label);
    showInGameMenuLayer();
    return true;
}

void AdsTestScene::openBannersTest(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(BannersTestScene::scene());
	dbgprint("openBannersTest");
}

void AdsTestScene::openVideoTest(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(VideosTestScene::scene());
	dbgprint("openVideoTest");
}

void AdsTestScene::openInterstitialTest(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(InterstitialsTestScene::scene());
	dbgprint("openInterstitialTest");
}

void AdsTestScene::showInGameMenuLayer() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	CCMenuItemFont *menuButton = CCMenuItemFont::create("Menu", this,menu_selector(AdsTestScene::menuButtonCallback));
	menuButton->setColor(ccc3(0,0,0));

	menuButton->setPosition(ccp(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	CCMenu *pMenu = CCMenu::create(menuButton,NULL);
	pMenu->setPosition(CCPointZero);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void AdsTestScene::menuButtonCallback(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}


CCScene* AdsTestScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    AdsTestScene *layer = AdsTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
