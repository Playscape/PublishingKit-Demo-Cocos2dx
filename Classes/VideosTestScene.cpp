#include "VideosTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/VideosX.h"
#include "ui/CocosGUI.h"

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
			MenuItemFont::create("Show Video", this,menu_selector(VideosTestScene::showVideo)),
			MenuItemFont::create("Show Incetivised Video", this,menu_selector(VideosTestScene::showIncentivisedVideo)),
			NULL);

    for(Ref* item : mMainMenu->getChildren()) {
    	((MenuItemFont*)item)->setColor(Color3B::BLACK);
    }

    mMainMenu->alignItemsVertically();

    addChild(mMainMenu, 1);
    addChild(label);
    showInGameMenuLayer();

    VideosX::setListener(new MyVideoListener());
    return true;
}

void VideosTestScene::showVideo(Ref* sender) {
	VideosX::display(VideosX::NonIncentivised, "video");
}

void VideosTestScene::showIncentivisedVideo(Ref* sender) {
	VideosX::display(VideosX::Incentivised, "video-incentivised");
}


void VideosTestScene::showInGameMenuLayer() {
	Size winSize = Director::getInstance()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	MenuItemFont *menuButton = MenuItemFont::create("Menu", this,menu_selector(VideosTestScene::menuButtonCallback));
	menuButton->setColor(Color3B(0,0,0));

	menuButton->setPosition(Point(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	Menu *pMenu = Menu::create(menuButton,NULL);
	pMenu->setPosition(Point::ZERO);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void VideosTestScene::menuButtonCallback(Ref* sender) {
	Director::getInstance()->replaceScene(HelloWorld::scene());
}


Scene* VideosTestScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();

    // 'layer' is an autorelease object
    VideosTestScene *layer = VideosTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
