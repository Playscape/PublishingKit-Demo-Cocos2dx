#include "ReportTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/Report.h"

#include <string>
#include <map>

using namespace cocos2d;
using playscape::Report;
using std::string;


// on "init" you need to initialize your instance
bool ReportTestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(cocos2d::ccc4(255, 255, 255, 255)) )
    {
        return false;
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    setTouchEnabled(true);
    CCLabelTTF* label = CCLabelTTF::create("Report Test", "Marker Felt", 22);
    label->setPosition(
			ccp(winSize.width / 2 - label->getContentSize().width / 2,
			winSize.height - label->getContentSize().height)
		);

    mMainMenu = CCMenu::create(
			CCMenuItemFont::create("Report Custom Event", this, menu_selector(ReportTestScene::reportEventClicked)),
			CCMenuItemFont::create("Report Custom Event With Attributes", this, menu_selector(ReportTestScene::reportAttrEventClicked)),
			NULL);

    CCObject* item;
    CCARRAY_FOREACH(mMainMenu->getChildren(), item) {
    	((CCMenuItemFont*)item)->setColor(ccBLACK);
    }

    mMainMenu->alignItemsVertically();

    addChild(mMainMenu, 1);
    addChild(label, 2);
    showInGameMenuLayer();

    return true;
}

void ReportTestScene::reportEventClicked(CCObject *sender) {
	Report::getInstance().ReportEvent("custom_event");
}

void ReportTestScene::reportAttrEventClicked(CCObject *sender) {
	std::map<string, string> eventAttrs;
	
	eventAttrs["key_1"] = "value_1";
	eventAttrs["key_2"] = "value_2";
	
	Report::getInstance().ReportEvent("custom_event", eventAttrs);
}

void ReportTestScene::showInGameMenuLayer() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	CCMenuItemFont *menuButton = CCMenuItemFont::create("Menu", this,menu_selector(ReportTestScene::menuButtonCallback));
	menuButton->setColor(ccc3(0,0,0));

	menuButton->setPosition(ccp(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	CCMenu *pMenu = CCMenu::create(menuButton,NULL);
	pMenu->setPosition(CCPointZero);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void ReportTestScene::menuButtonCallback(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}


CCScene* ReportTestScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    ReportTestScene *layer = ReportTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}