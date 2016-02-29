#include "VideosTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/VideosX.h"

USING_NS_CC;

using namespace playscape::ads;
//using std::string;

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
    auto label = Label::createWithTTF("Ads Test", "Marker Felt", 22);

    label->setPosition(Vec2(winSize.width/2-label->getContentSize().width/2, winSize.height-label->getContentSize().height));

    mMainMenu =
		Menu::create(
			MenuItemFont::create("Show Video", CC_CALLBACK_1(VideosTestScene::showVideo, this)),
			MenuItemFont::create("Show Incetivised Video", CC_CALLBACK_1(VideosTestScene::showIncentivisedVideo, this)),
			NULL);

    for(auto item : mMainMenu->getChildren()) {
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

	MenuItemFont *menuButton = MenuItemFont::create("Menu", CC_CALLBACK_1(VideosTestScene::menuButtonCallback, this));
    menuButton->setColor(Color3B::BLACK);

	menuButton->setPosition(Point(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	Menu *pMenu = Menu::create(menuButton,NULL);
	pMenu->setPosition(Vec2::ZERO);

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
