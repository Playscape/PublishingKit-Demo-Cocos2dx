#include "InterstitialsTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/Report.h"
#include "playscape/InterstitialsX.h"

#include <string>

using namespace playscape::ads;
using namespace cocos2d;
using std::string;

class MyInterstitialListener : public InterstitialListener {
public:
	/// <summary>
	/// Occurs when an interstitial ad is shown on the screen.
	/// </summary>
	void onShown(InterstitialsX::Kind kind) {
		dbgprint("InterstitialsListener.onShown: Kind=%d", (int)kind);
	}

	/// <summary>
	/// Occurs when display has ended - either user closed it, clicked it or display failed altogether.
	/// </summary>
	void onEnded(InterstitialsX::State state, InterstitialsX::Kind kind) {
		dbgprint("InterstitialsListener.onEnded: State=%d, Kind=%d", (int)state, (int)kind);
	}

	~MyInterstitialListener() {}
};

// on "init" you need to initialize your instance
bool InterstitialsTestScene::init()
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
			CCMenuItemFont::create("Show Interstitial Overlay/NonOverlay", this,menu_selector(InterstitialsTestScene::showInterstitialOverlayOrNonOverlay)),
			NULL);

    CCObject* item;
    CCARRAY_FOREACH(mMainMenu->getChildren(), item) {
    	((CCMenuItemFont*)item)->setColor(ccBLACK);
    }

    mMainMenu->alignItemsVertically();

    addChild(mMainMenu, 1);
    addChild(label);
    showInGameMenuLayer();

    InterstitialsX::setListener(new MyInterstitialListener());
    return true;
}

void InterstitialsTestScene::showInterstitialOverlayOrNonOverlay(CCObject* sender) {
	InterstitialsX::display("test-scene-both");
}

void InterstitialsTestScene::showInGameMenuLayer() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	CCMenuItemFont *menuButton = CCMenuItemFont::create("Menu", this,menu_selector(InterstitialsTestScene::menuButtonCallback));
	menuButton->setColor(ccc3(0,0,0));

	menuButton->setPosition(ccp(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	CCMenu *pMenu = CCMenu::create(menuButton,NULL);
	pMenu->setPosition(CCPointZero);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void InterstitialsTestScene::menuButtonCallback(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}


CCScene* InterstitialsTestScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    InterstitialsTestScene *layer = InterstitialsTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}




