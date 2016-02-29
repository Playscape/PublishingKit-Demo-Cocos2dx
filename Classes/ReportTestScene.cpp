#include "ReportTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/Report.h"


using namespace cocos2d;
using playscape::Report;


// on "init" you need to initialize your instance
bool ReportTestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(cocos2d::Color4B(255, 255, 255, 255)) )
    {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    setTouchEnabled(true);
    auto label = Label::createWithTTF("Report Test", "Marker Felt", 22);
    label->setPosition(
			Point(winSize.width / 2 - label->getContentSize().width / 2,
			winSize.height - label->getContentSize().height)
		);

    mMainMenu = Menu::create(
			MenuItemFont::create("Report Custom Event", CC_CALLBACK_1(ReportTestScene::reportEventClicked, this)),
			MenuItemFont::create("Report Custom Event With Attributes", CC_CALLBACK_1(ReportTestScene::reportAttrEventClicked, this)),
			NULL);

    for(auto item : mMainMenu->getChildren()) {
    	((MenuItemFont*)item)->setColor(Color3B::BLACK);
    }

    mMainMenu->alignItemsVertically();

    addChild(mMainMenu, 1);
    addChild(label, 2);
    showInGameMenuLayer();

    return true;
}

void ReportTestScene::reportEventClicked(Ref *sender) {
	Report::getInstance().ReportEvent("custom_event");
}

void ReportTestScene::reportAttrEventClicked(Ref *sender) {
    std::map<string, string> eventAttrs;
	
	eventAttrs["key_1"] = "value_1";
	eventAttrs["key_2"] = "value_2";
	
	Report::getInstance().ReportEvent("custom_event", eventAttrs);
}

void ReportTestScene::showInGameMenuLayer() {
	Size winSize = Director::getInstance()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	MenuItemFont *menuButton = MenuItemFont::create("Menu", CC_CALLBACK_1(ReportTestScene::menuButtonCallback, this));
	menuButton->setColor(Color3B(0,0,0));

	menuButton->setPosition(Point(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	Menu *pMenu = Menu::create(menuButton,NULL);
	pMenu->setPosition(Point::ZERO);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void ReportTestScene::menuButtonCallback(Ref* sender) {
	Director::getInstance()->replaceScene(HelloWorld::scene());
}


Scene* ReportTestScene::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    ReportTestScene *layer = ReportTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}