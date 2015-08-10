#include "BannersTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/Report.h"
#include "playscape/BannersX.h"
#include "ui/CocosGUI.h"

#include <string>

using namespace cocos2d;

using namespace playscape::ads;
using std::string;

// on "init" you need to initialize your instance
bool BannersTestScene::init()
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
			MenuItemFont::create("Show Banner Top Middle", this,menu_selector(BannersTestScene::showBannerTopMiddle)),
			MenuItemFont::create("Show Banner Top Left", this,menu_selector(BannersTestScene::showBannerTopLeft)),
			MenuItemFont::create("Show Banner Top Right", this,menu_selector(BannersTestScene::showBannerTopRight)),

			MenuItemFont::create("Show Banner Bottom Middle", this,menu_selector(BannersTestScene::showBannerBottomMiddle)),
			MenuItemFont::create("Show Banner Bottom Left", this,menu_selector(BannersTestScene::showBannerBottomLeft)),
			MenuItemFont::create("Show Banner Bottom Right", this,menu_selector(BannersTestScene::showBannerBottomRight)),

			MenuItemFont::create("Hide Banner", this,menu_selector(BannersTestScene::hideBanner)),
			NULL);

    for(Ref* item : mMainMenu->getChildren()) {
    	((MenuItemFont*)item)->setColor(Color3B::BLACK);
    }

    mMainMenu->alignItemsVertically();

    addChild(mMainMenu, 1);
    addChild(label);
    showInGameMenuLayer();
    return true;
}

void BannersTestScene::showBannerTopMiddle(Ref* sender) {
	BannersX::display(BannersX::topMiddle, "top-middle");
}

void BannersTestScene::showBannerTopLeft(Ref* sender) {
	BannersX::display(BannersX::topLeft, "top-left");
}
void BannersTestScene::showBannerTopRight(Ref* sender) {
	BannersX::display(BannersX::topRight, "top-right");
}

void BannersTestScene::showBannerBottomMiddle(Ref* sender) {
	BannersX::display(BannersX::bottomMiddle, "bottom-middle");
}

void BannersTestScene::showBannerBottomLeft(Ref* sender) {
	BannersX::display(BannersX::bottomLeft, "bottom-left");
}

void BannersTestScene::showBannerBottomRight(Ref* sender) {
	BannersX::display(BannersX::bottomRight, "bottom-right");
}

void BannersTestScene::hideBanner(Ref* sender) {
	BannersX::hide();
}

void BannersTestScene::showInGameMenuLayer() {
	Size winSize = Director::getInstance()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	MenuItemFont *menuButton = MenuItemFont::create("Menu", this,menu_selector(BannersTestScene::menuButtonCallback));
	menuButton->setColor(Color3B(0,0,0));

	menuButton->setPosition(Point(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	Menu *pMenu = Menu::create(menuButton,NULL);
	pMenu->setPosition(Point::ZERO);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void BannersTestScene::menuButtonCallback(Ref* sender) {
	Director::getInstance()->replaceScene(HelloWorld::scene());
}


Scene* BannersTestScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();

    // 'layer' is an autorelease object
    BannersTestScene *layer = BannersTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
