#include "FacebookTestScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "ui/CocosGUI.h"
#include "FacebookAgent.h"


#include <string>

using namespace cocos2d;
using namespace cocos2d::plugin;
using std::string;

// on "init" you need to initialize your instance
bool FacebookTestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(cocos2d::Color4B(255, 255, 255, 255)) )
    {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    setTouchEnabled(true);
    cocos2d::ui::Text* label = cocos2d::ui::Text::create("Facebook test", "Marker Felt", 22);

    label->setPosition(Point(winSize.width/2-label->getContentSize().width/2, winSize.height-label->getContentSize().height));

    mMainMenu =
		Menu::create(
			MenuItemFont::create("Login Test", this,menu_selector(FacebookTestScene::login)),
			MenuItemFont::create("Logout Test", this,menu_selector(FacebookTestScene::logout)),
			MenuItemFont::create("Share Test", this,menu_selector(FacebookTestScene::share)),
            MenuItemFont::create("Invite Test", this,menu_selector(FacebookTestScene::invite)),
            MenuItemFont::create("Get Requests Amount Test", this,menu_selector(FacebookTestScene::getRequestsAmount)),
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

void FacebookTestScene::login(Ref* sender) {
	dbgprint("login");
    cocos2d::plugin::FacebookAgent::getInstance()->login([=](int ret, std::string& msg){
                    // CCLOG(StringUtils::format("type is %d, msg is %s", ret, msg.c_str()));
                    dbgprint("logged in");
                    // dbgprint(StringUtils::format("type is %d, msg is %s", ret, msg.c_str()));
                });
}

void FacebookTestScene::logout(Ref* sender) {
    cocos2d::plugin::FacebookAgent::getInstance()->logout();
	dbgprint("logout");
}

void FacebookTestScene::share(Ref* sender) {
    FacebookAgent::FBInfo params;
    params.insert(std::make_pair("dialog", "shareLink"));
    params.insert(std::make_pair("link", "http://playscape.com"));
    if (FacebookAgent::getInstance()->canPresentDialogWithParams(params)) {
        FacebookAgent::getInstance()->dialog(params, [=](int ret ,std::string& msg){
            CCLOG("%s", msg.c_str());
        });
        dbgprint("share");
    } else {
        dbgprint("can't share");
    }

}

void FacebookTestScene::invite(Ref* sender) {
    FacebookAgent::FBInfo params;
    params.insert(std::make_pair("message", "Cocos2d-x is a great game engine"));
    params.insert(std::make_pair("title", "Cocos2d-x title"));

    FacebookAgent::getInstance()->appRequest(params, [=](int ret, std::string& msg){
            CCLOG("%s", msg.c_str());
            dbgprint("invited");
    });
}

void FacebookTestScene::getRequestsAmount(Ref* sender) {
    std::string path = "/me/apprequests";
       FacebookAgent::FBInfo params;
       FacebookAgent::getInstance()->api(path, FacebookAgent::HttpMethod::Get, params, [=](int ret, std::string& msg){
           CCLOG("%s", msg.c_str());
           dbgprint("got apprequests");
       });
}

void FacebookTestScene::showInGameMenuLayer() {
	Size winSize = Director::getInstance()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	MenuItemFont *menuButton = MenuItemFont::create("Menu", this,menu_selector(FacebookTestScene::menuButtonCallback));
	menuButton->setColor(Color3B(0,0,0));

	menuButton->setPosition(Point(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	Menu *pMenu = Menu::create(menuButton,NULL);
	pMenu->setPosition(Point::ZERO);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void FacebookTestScene::menuButtonCallback(Ref* sender) {
	Director::getInstance()->replaceScene(HelloWorld::scene());
}


Scene* FacebookTestScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();

    // 'layer' is an autorelease object
    FacebookTestScene *layer = FacebookTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
