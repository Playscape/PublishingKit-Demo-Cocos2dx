#include "InviteFriendsScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/Report.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace cocos2d;
using std::string;
using playscape::Report;

#define FAKE_REQUEST_ID "FakeRequestId"
#define FAKE_REQUESTED_USER_ID "FakeRequestedUserId"
#define FAKE_RANDOM_REQUEST_ID 31

// on "init" you need to initialize your instance
bool InviteFriendsScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(cocos2d::Color4B(255, 255, 255, 255)) )
    {
        return false;
    }

    mInviteDialog = NULL;

    Size winSize = Director::getInstance()->getWinSize();

    setTouchEnabled(true);
    cocos2d::ui::Text* label = cocos2d::ui::Text::create("Play Game with Friends (Mock Social)", "Marker Felt", 22);

    label->setPosition(Point(winSize.width/2-label->getContentSize().width/2, winSize.height-label->getContentSize().height));

    mPlaysWithMenu =
		Menu::create(
			MenuItemFont::create("Play with John Snow", this,menu_selector(InviteFriendsScene::inviteJohnSnow)),
			MenuItemFont::create("Play with Jack Sparrow", this,menu_selector(InviteFriendsScene::inviteJackSparrow)),
			MenuItemFont::create("Play with Paul Atreides", this,menu_selector(InviteFriendsScene::invitePaulAtreides)),
			NULL);

	for(Ref* item : mPlaysWithMenu->getChildren()) {
    	((MenuItemFont*)item)->setColor(Color3B::BLACK);
    }

    mPlaysWithMenu->alignItemsVertically();


    // this should actually be called social user when images done downloading
    Report::getInstance().ReportSocialGetImagesSuccess(3);

    addChild(mPlaysWithMenu, 1);
    addChild(label);
    showInGameMenuLayer();
    return true;
}

void InviteFriendsScene::inviteJohnSnow(Ref* sender) {
	showInviteDialog("John Snow");
	dbgprint("Invite John Snow");
}

void InviteFriendsScene::inviteJackSparrow(Ref* sender) {
	showInviteDialog("Jack Sparrow");
	dbgprint("Invite Jack Sparrow");
}

void InviteFriendsScene::invitePaulAtreides(Ref* sender) {
	showInviteDialog("Paul Atreides");
	dbgprint("Invite Paul Atreides");
}

void InviteFriendsScene::showInGameMenuLayer() {
	Size winSize = Director::getInstance()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	MenuItemFont *menuButton = MenuItemFont::create("Menu", this,menu_selector(InviteFriendsScene::menuButtonCallback));
	menuButton->setColor(Color3B(0,0,0));

	menuButton->setPosition(Point(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	Menu *pMenu = Menu::create(menuButton,NULL);
	pMenu->setPosition(Point::ZERO);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void InviteFriendsScene::menuButtonCallback(Ref* sender) {
	Director::getInstance()->replaceScene(HelloWorld::scene());
}


Scene* InviteFriendsScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();

    // 'layer' is an autorelease object
    InviteFriendsScene *layer = InviteFriendsScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void InviteFriendsScene::showInviteDialog(const string& friendName) {

	if (mInviteDialog) {
		removeChild(mInviteDialog);
	}
	Size winSize = Director::getInstance()->getWinSize();
	mInviteDialog = LayerColor::create(Color4B(0xcc, 0xcc, 0xcc, 0xff), (winSize.width * 0.8f), (winSize.height * 0.8f));
	addChild(mInviteDialog);

	string question = string("Would you like to invite ") + friendName + "?";
	cocos2d::ui::Text* questionLabel = cocos2d::ui::Text::create(question.c_str(), "Marker Felt", 26);
	questionLabel->setAnchorPoint(Point::ZERO);
	mInviteDialog->setAnchorPoint(Point::ZERO);
	mInviteDialog->setPosition(Point(winSize.width/2 - mInviteDialog->getContentSize().width/2,
								   winSize.height/2 - mInviteDialog->getContentSize().height/2));

	questionLabel->setPosition(Point(mInviteDialog->getContentSize().width/2 - questionLabel->getContentSize().width/2,
			mInviteDialog->getContentSize().height/2 - questionLabel->getContentSize().height/2));

	Menu *pMenu =
		Menu::create(
			MenuItemFont::create(" Invite ", this,menu_selector(InviteFriendsScene::inviteButtonCallback)),
			MenuItemFont::create(" Cancel", this,menu_selector(InviteFriendsScene::cancelInviteButtonCallback)),
			NULL);

	for(Ref* item : pMenu->getChildren()) {
		((MenuItemFont*)item)->setColor(Color3B::BLACK);
	}

	mInviteDialog->addChild(pMenu);
	pMenu->alignItemsHorizontally();
	pMenu->setAnchorPoint(Point::ZERO);
	pMenu->setPosition(Point(mInviteDialog->getPosition().x + mInviteDialog->getContentSize().width/2 , mInviteDialog->getPosition().y));

	mInviteDialog->addChild(questionLabel);


	mPlaysWithMenu->setEnabled(false);
	mPlaysWithMenu->setVisible(false);
	mInviteDialog->setVisible(true);
}

void InviteFriendsScene::inviteButtonCallback(Ref* sender) {
	Report::getInstance().ReportSocialRequestSent(FAKE_REQUEST_ID, // in a real game this should be the id of the sent request as given by the social network (fb/google+)
													  FAKE_REQUESTED_USER_ID, // this should be the social identifier of the user whom the request is sent to (e.g fb/google+ user id)
													  FAKE_RANDOM_REQUEST_ID // this should be a randomly generated number uniquely identifying the sent request which should be sent along side the request
													  );
	hideInviteDialog();
}

void InviteFriendsScene::cancelInviteButtonCallback(Ref* sender) {
	hideInviteDialog();
}


void InviteFriendsScene::hideInviteDialog() {
	mInviteDialog->setVisible(false);
	mPlaysWithMenu->setEnabled(true);
	mPlaysWithMenu->setVisible(true);
}
