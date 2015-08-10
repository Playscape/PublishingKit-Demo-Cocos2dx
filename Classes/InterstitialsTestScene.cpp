#include "InterstitialsTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/Report.h"
#include "playscape/InterstitialsX.h"
#include "ui/CocosGUI.h"

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
    if ( !LayerColor::initWithColor(cocos2d::Color4B(255, 255, 255, 255)) )
    {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    setTouchEnabled(true);
    LabelTTF* label = LabelTTF::create("Ads Test", "Marker Felt", 22);
    label->setPosition(Point(winSize.width/2-label->getContentSize().width/2, winSize.height-label->getContentSize().height));

    mMainMenu =
		Menu::create(
			MenuItemFont::create("Show Interstitial Overlay", this,menu_selector(InterstitialsTestScene::showInterstitialOverlay)),
			MenuItemFont::create("Show Interstitial NonOverlay", this,menu_selector(InterstitialsTestScene::showInterstitialNonOverlay)),
			MenuItemFont::create("Show Interstitial Overlay/NonOverlay", this,menu_selector(InterstitialsTestScene::showInterstitialOverlayOrNonOverlay)),
			NULL);

	for(Ref* item : mMainMenu->getChildren()) {
    	((MenuItemFont*)item)->setColor(Color3B::BLACK);
    }

    mMainMenu->alignItemsVertically();

    addChild(mMainMenu, 1);
    addChild(label);
    showInGameMenuLayer();

    InterstitialsX::setListener(new MyInterstitialListener());
    return true;
}

void InterstitialsTestScene::showInterstitialOverlay(Ref* sender) {
	InterstitialsX::display(InterstitialsX::Overlay, "test-scene-overlay");
}

void InterstitialsTestScene::showInterstitialNonOverlay(Ref* sender) {
	InterstitialsX::display(InterstitialsX::NonOverlay, "test-scene-non-overlay");
}

void InterstitialsTestScene::showInterstitialOverlayOrNonOverlay(Ref* sender) {
	InterstitialsX::display(InterstitialsX::Both, "test-scene-both");
}

void InterstitialsTestScene::showInGameMenuLayer() {
	Size winSize = Director::getInstance()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	MenuItemFont *menuButton = MenuItemFont::create("Menu", this,menu_selector(InterstitialsTestScene::menuButtonCallback));
	menuButton->setColor(Color3B(0,0,0));

	menuButton->setPosition(Point(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	Menu *pMenu = Menu::create(menuButton,NULL);
	pMenu->setPosition(Point::ZERO);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void InterstitialsTestScene::menuButtonCallback(Ref* sender) {
	Director::getInstance()->replaceScene(HelloWorld::scene());
}


Scene* InterstitialsTestScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();

    // 'layer' is an autorelease object
    InterstitialsTestScene *layer = InterstitialsTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
