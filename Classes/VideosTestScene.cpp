#include "VideosTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/VideosX.h"

#include <string>

using namespace cocos2d;
using namespace playscape::ads;
using std::string;

class MyVideoListener : public VideoListener {

    /// <summary>
    /// Occurs when an video  display has ended - either user skipped it, watched until its end or display failed altogether.
    /// </summary>
    void onEnded(VideosX::State state, VideosX::Kind kind) {
    	dbgprint("VideoListener.onEnded state=%d, kind=%d", (int)state, (int)kind);
    }

    ~MyVideoListener() {}
};

// on "init" you need to initialize your instance
bool VideosTestScene::init()
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
			CCMenuItemFont::create("Show Video", this,menu_selector(VideosTestScene::showVideo)),
			CCMenuItemFont::create("Show Incetivised Video", this,menu_selector(VideosTestScene::showIncentivisedVideo)),
			NULL);

    CCObject* item;
    CCARRAY_FOREACH(mMainMenu->getChildren(), item) {
    	((CCMenuItemFont*)item)->setColor(ccBLACK);
    }

    mMainMenu->alignItemsVertically();

    addChild(mMainMenu, 1);
    addChild(label);
    showInGameMenuLayer();

    VideosX::setListener(new MyVideoListener());
    return true;
}

void VideosTestScene::showVideo(CCObject* sender) {
	VideosX::display(VideosX::NonIncentivised, "video");
}

void VideosTestScene::showIncentivisedVideo(CCObject* sender) {
	VideosX::display(VideosX::Incentivised, "video-incentivised");
}


void VideosTestScene::showInGameMenuLayer() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	CCMenuItemFont *menuButton = CCMenuItemFont::create("Menu", this,menu_selector(VideosTestScene::menuButtonCallback));
	menuButton->setColor(ccc3(0,0,0));

	menuButton->setPosition(ccp(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	CCMenu *pMenu = CCMenu::create(menuButton,NULL);
	pMenu->setPosition(CCPointZero);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void VideosTestScene::menuButtonCallback(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}


CCScene* VideosTestScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    VideosTestScene *layer = VideosTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}




