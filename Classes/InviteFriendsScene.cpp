#include "InviteFriendsScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include "playscape/Report.h"

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
    if ( !CCLayerColor::initWithColor(cocos2d::ccc4(255, 255, 255, 255)) )
    {
        return false;
    }

    mInviteDialog = NULL;

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    setTouchEnabled(true);
    CCLabelTTF* label = CCLabelTTF::create("Play Game with Friends (Mock Social)", "Marker Felt", 22);

    label->setPosition(ccp(winSize.width/2-label->getContentSize().width/2, winSize.height-label->getContentSize().height));

    mPlaysWithMenu =
		CCMenu::create(
			CCMenuItemFont::create("Play with John Snow", this,menu_selector(InviteFriendsScene::inviteJohnSnow)),
			CCMenuItemFont::create("Play with Jack Sparrow", this,menu_selector(InviteFriendsScene::inviteJackSparrow)),
			CCMenuItemFont::create("Play with Paul Atreides", this,menu_selector(InviteFriendsScene::invitePaulAtreides)),
			NULL);

    CCObject* item;
    CCARRAY_FOREACH(mPlaysWithMenu->getChildren(), item) {
    	((CCMenuItemFont*)item)->setColor(ccBLACK);
    }

    mPlaysWithMenu->alignItemsVertically();


    // this should actually be called social user when images done downloading
    Report::getInstance().ReportSocialGetImagesSuccess(3);

    addChild(mPlaysWithMenu, 1);
    addChild(label);
    showInGameMenuLayer();
    return true;
}

void InviteFriendsScene::inviteJohnSnow(CCObject* sender) {
	showInviteDialog("John Snow");
	dbgprint("Invite John Snow");
}

void InviteFriendsScene::inviteJackSparrow(CCObject* sender) {
	showInviteDialog("Jack Sparrow");
	dbgprint("Invite Jack Sparrow");
}

void InviteFriendsScene::invitePaulAtreides(CCObject* sender) {
	showInviteDialog("Paul Atreides");
	dbgprint("Invite Paul Atreides");
}

void InviteFriendsScene::showInGameMenuLayer() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
	addChild(_inGameMenuLayer);

	CCMenuItemFont *menuButton = CCMenuItemFont::create("Menu", this,menu_selector(InviteFriendsScene::menuButtonCallback));
	menuButton->setColor(ccc3(0,0,0));

	menuButton->setPosition(ccp(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	CCMenu *pMenu = CCMenu::create(menuButton,NULL);
	pMenu->setPosition(CCPointZero);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void InviteFriendsScene::menuButtonCallback(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}


CCScene* InviteFriendsScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

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
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	mInviteDialog = CCLayerColor::create(ccc4(0xcc, 0xcc, 0xcc, 0xff), (winSize.width * 0.8f), (winSize.height * 0.8f));
	addChild(mInviteDialog);

	string question = string("Would you like to invite ") + friendName + "?";
	CCLabelTTF* questionLabel = CCLabelTTF::create(question.c_str(), "Marker Felt", 26);
	questionLabel->setAnchorPoint(CCPointZero);
	mInviteDialog->setAnchorPoint(CCPointZero);
	mInviteDialog->setPosition(ccp(winSize.width/2 - mInviteDialog->getContentSize().width/2,
								   winSize.height/2 - mInviteDialog->getContentSize().height/2));

	questionLabel->setPosition(ccp(mInviteDialog->getContentSize().width/2 - questionLabel->getContentSize().width/2,
			mInviteDialog->getContentSize().height/2 - questionLabel->getContentSize().height/2));

	CCMenu *pMenu =
		CCMenu::create(
			CCMenuItemFont::create(" Invite ", this,menu_selector(InviteFriendsScene::inviteButtonCallback)),
			CCMenuItemFont::create(" Cancel", this,menu_selector(InviteFriendsScene::cancelInviteButtonCallback)),
			NULL);

	CCObject* item;
	CCARRAY_FOREACH(pMenu->getChildren(), item) {
		((CCMenuItemFont*)item)->setColor(ccBLACK);
	}

	mInviteDialog->addChild(pMenu);
	pMenu->alignItemsHorizontally();
	pMenu->setAnchorPoint(CCPointZero);
	pMenu->setPosition(ccp(mInviteDialog->getPosition().x + mInviteDialog->getContentSize().width/2 , mInviteDialog->getPosition().y));

	mInviteDialog->addChild(questionLabel);


	mPlaysWithMenu->setEnabled(false);
	mPlaysWithMenu->setVisible(false);
	mInviteDialog->setVisible(true);
}

void InviteFriendsScene::inviteButtonCallback(CCObject* sender) {
	Report::getInstance().ReportSocialRequestSent(FAKE_REQUEST_ID, // in a real game this should be the id of the sent request as given by the social network (fb/google+)
													  FAKE_REQUESTED_USER_ID, // this should be the social identifier of the user whom the request is sent to (e.g fb/google+ user id)
													  FAKE_RANDOM_REQUEST_ID // this should be a randomly generated number uniquely identifying the sent request which should be sent along side the request
													  );
	hideInviteDialog();
}

void InviteFriendsScene::cancelInviteButtonCallback(CCObject* sender) {
	hideInviteDialog();
}


void InviteFriendsScene::hideInviteDialog() {
	mInviteDialog->setVisible(false);
	mPlaysWithMenu->setEnabled(true);
	mPlaysWithMenu->setVisible(true);
}

