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

    CCMenu *pMenu = CCMenu::create(item1, item2, item3, item4, NULL);

    pMenu->alignItemsVertically();
    addChild(pMenu, 1);

    showInGameMenuLayer();
    return true;
}

void StoreScene::buyNinjaStarsCallback(CCObject* sender) {
	dbgprint("Buying ninja stars");
}

void StoreScene::buyNinjaSwordCallback(CCObject* sender) {
	dbgprint("Buying ninja sword and failing");
}

void StoreScene::buySamuraiShieldCallback(CCObject* sender) {
	dbgprint("Buying samurai shield");
}
void StoreScene::buyShogunKatanaCallback(CCObject* sender) {
	dbgprint("Buying samurai shield");
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

