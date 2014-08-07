#include "BannersTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/Report.h"
#include "playscape/BannersX.h"

#include <string>

using namespace cocos2d;

using namespace playscape::ads;
using std::string;

// on "init" you need to initialize your instance
bool BannersTestScene::init()
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
			CCMenuItemFont::create("Show Banner Top Middle", this,menu_selector(BannersTestScene::showBannerTopMiddle)),
			CCMenuItemFont::create("Show Banner Top Left", this,menu_selector(BannersTestScene::showBannerTopLeft)),
			CCMenuItemFont::create("Show Banner Top Right", this,menu_selector(BannersTestScene::showBannerTopRight)),

			CCMenuItemFont::create("Show Banner Bottom Middle", this,menu_selector(BannersTestScene::showBannerBottomMiddle)),
			CCMenuItemFont::create("Show Banner Bottom Left", this,menu_selector(BannersTestScene::showBannerBottomLeft)),
			CCMenuItemFont::create("Show Banner Bottom Right", this,menu_selector(BannersTestScene::showBannerBottomRight)),

			CCMenuItemFont::create("Hide Banner", this,menu_selector(BannersTestScene::hideBanner)),
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

void BannersTestScene::showBannerTopMiddle(CCObject* sender) {
	BannersX::display(BannersX::topMiddle, "top-middle");
}

void BannersTestScene::showBannerTopLeft(CCObject* sender) {
	BannersX::display(BannersX::topLeft, "top-left");
}
void BannersTestScene::showBannerTopRight(CCObject* sender) {
	BannersX::display(BannersX::topRight, "top-right");
}

void BannersTestScene::showBannerBottomMiddle(CCObject* sender) {
	BannersX::display(BannersX::bottomMiddle, "bottom-middle");
}

void BannersTestScene::showBannerBottomLeft(CCObject* sender) {
	BannersX::display(BannersX::bottomLeft, "bottom-left");
}

void BannersTestScene::showBannerBottomRight(CCObject* sender) {
	BannersX::display(BannersX::bottomRight, "bottom-right");
}

void BannersTestScene::hideBanner(CCObject* sender) {
	BannersX::hide();
}

void BannersTestScene::showInGameMenuLayer() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	CCMenuItemFont *menuButton = CCMenuItemFont::create("Menu", this,menu_selector(BannersTestScene::menuButtonCallback));
	menuButton->setColor(ccc3(0,0,0));

	menuButton->setPosition(ccp(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	CCMenu *pMenu = CCMenu::create(menuButton,NULL);
	pMenu->setPosition(CCPointZero);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void BannersTestScene::menuButtonCallback(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}


CCScene* BannersTestScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    BannersTestScene *layer = BannersTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}




