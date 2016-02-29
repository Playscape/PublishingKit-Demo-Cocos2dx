#include "AdsTestScene.h"
#include "VideosTestScene.h"
#include "BannersTestScene.h"
#include "InterstitialsTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "ui/CocosGUI.h"


#include <string>

using namespace cocos2d;
using std::string;

// on "init" you need to initialize your instance
bool AdsTestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(cocos2d::Color4B(255, 255, 255, 255)) )
    {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    setTouchEnabled(true);
    cocos2d::ui::Text* label = cocos2d::ui::Text::create("Ads Test", "Marker Felt", 22);

    label->setPosition(Point(winSize.width/2-label->getContentSize().width/2, winSize.height-label->getContentSize().height));

    mMainMenu =
		Menu::create(
			MenuItemFont::create("Open Banners Test", CC_CALLBACK_1(AdsTestScene::openBannersTest, this)),
			MenuItemFont::create("Open Video Test", CC_CALLBACK_1(AdsTestScene::openVideoTest, this)),
			MenuItemFont::create("Open Interstitial Test", CC_CALLBACK_1(AdsTestScene::openInterstitialTest, this)),
			NULL);

    for(auto item : mMainMenu->getChildren()) {
    	((MenuItemFont*)item)->setColor(Color3B::BLACK);
    }

    mMainMenu->alignItemsVertically();

    addChild(mMainMenu, 1);
    addChild(label);
    showInGameMenuLayer();
    return true;
}

void AdsTestScene::openBannersTest(Ref* sender) {
	Director::getInstance()->replaceScene(BannersTestScene::scene());
	dbgprint("openBannersTest");
}

void AdsTestScene::openVideoTest(Ref* sender) {
	Director::getInstance()->replaceScene(VideosTestScene::scene());
	dbgprint("openVideoTest");
}

void AdsTestScene::openInterstitialTest(Ref* sender) {
	Director::getInstance()->replaceScene(InterstitialsTestScene::scene());
	dbgprint("openInterstitialTest");
}

void AdsTestScene::showInGameMenuLayer() {
	Size winSize = Director::getInstance()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	MenuItemFont *menuButton = MenuItemFont::create("Menu", CC_CALLBACK_1(AdsTestScene::menuButtonCallback, this));
    menuButton->setColor(Color3B::BLACK);

	menuButton->setPosition(Point(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	Menu *pMenu = Menu::create(menuButton,NULL);
	pMenu->setPosition(Point::ZERO);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void AdsTestScene::menuButtonCallback(Ref* sender) {
	Director::getInstance()->replaceScene(HelloWorld::scene());
}


Scene* AdsTestScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();

    // 'layer' is an autorelease object
    AdsTestScene *layer = AdsTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
