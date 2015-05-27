#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "InGameMenuLayer.h"
#include "appwarp.h"
#include "dbgprint.h"
#include "StoreScene.h"
#include "AdsTestScene.h"

#include "playscape/Report.h"
#include "InviteFriendsScene.h"
#include "playscape/AdManagerX.h"

#include <sstream>

using namespace cocos2d;
using namespace CocosDenshion;
using playscape::Report;
using namespace playscape::ads;
using std::ostringstream;

#define FAKE_FACEBOOK_ID "fake_facebook_id"

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)) )
    {
        return false;
    }

    mMessageLayer = NULL;
    mIsInRoom = false;

    mIsFirstLaunch = true;
    setTouchEnabled(true);
    showStartGameLayer();

    mAppWarpMultiplayerProvider = new AppWarpMultiplayerProvider();
    mFacebookSocialAnalyticsProvider = new FacebookSocialAnalyticsProvider();

    Report::getInstance().InitMultiplayer(*mAppWarpMultiplayerProvider);
    Report::getInstance().InitSocial(*mFacebookSocialAnalyticsProvider);

    mStartedGameFromSimulatedInvite = false;
    mIsSocialNetworkLoggedIn = false;
    mHostGameButtonClicked = false;

    // Here's an example of setting some custom auxilary vars which are reported alongside each report -
    setSomeCustomVars();
    return true;
}

void HelloWorld::setSomeCustomVars() {
	int value = rand() % 20000;
	ostringstream is;
	is << "value_" << value;
	value++;
	is << value;

	dbgprint("setting custom vars: var1, var2, var3, var4 is set to empty which means it should not be reported.");

	is << value;
	Report::getInstance().setCustomVariable("var1", is.str());
	is << value;
	Report::getInstance().setCustomVariable("var2", is.str());
	is << value;
	Report::getInstance().setCustomVariable("var3", is.str());
	Report::getInstance().setCustomVariable("var4", "");
}

void HelloWorld::showStartGameLayer()
{
    mStartGameLayer = StartGameLayer::create();
    addChild(mStartGameLayer);

    CCMenu *pMenu =
		CCMenu::create(
			CCMenuItemFont::create("Host Game", this, menu_selector(HelloWorld::hostGameButtonCallback)),
			CCMenuItemFont::create("Join Game Game", this, menu_selector(HelloWorld::joinGameButtonCallback)),
			CCMenuItemFont::create("Open Store", this, menu_selector(HelloWorld::openStoreButtonCallback)),
			CCMenuItemFont::create("Play With Friends", this, menu_selector(HelloWorld::inviteFriendsButtonCallback)),
			CCMenuItemFont::create("Simulate Received Invite", this, menu_selector(HelloWorld::simulateReceivedInviteButtonCallback)),
			CCMenuItemFont::create("Simulate SocialNetwork Login/Logout", this, menu_selector(HelloWorld::simulateSocialNetworkLoginCallback)),
            CCMenuItemFont::create("Open Ads Test", this, menu_selector(HelloWorld::openAdsTest)),
            CCMenuItemFont::create("Disable Ads", this, menu_selector(HelloWorld::disableAds)),
            CCMenuItemFont::create("Enable Ads", this, menu_selector(HelloWorld::enableAds)),
			NULL);

    CCObject* item;
    CCARRAY_FOREACH(pMenu->getChildren(), item) {
    	((CCMenuItemFont*)item)->setColor(ccBLACK);
    }

    pMenu->alignItemsVertically();
    mStartGameLayer->addChild(pMenu, 1);
}

void HelloWorld::disableAds(CCObject* sender) {
    AdManagerX::disableAds();
}

void HelloWorld::enableAds(CCObject* sender) {
    AdManagerX::enableAds();
}

void HelloWorld::openAdsTest(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(AdsTestScene::scene());
}

void HelloWorld::inviteFriendsButtonCallback(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(InviteFriendsScene::scene());
}

void HelloWorld::simulateSocialNetworkLoginCallback(CCObject* pSender) {
	mIsSocialNetworkLoggedIn = !mIsSocialNetworkLoggedIn;

	if (mIsSocialNetworkLoggedIn) {
		Report::getInstance().ReportSocialLoginSuccess(false, FAKE_FACEBOOK_ID);
	} else {
		Report::getInstance().ReportSocialLogout();
	}
}

void HelloWorld::showInGameMenuLayer()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    mInGameMenuLayer = InGameMenuLayer::create();
    addChild(mInGameMenuLayer);

    CCMenuItemFont *menuButton = CCMenuItemFont::create("Menu", this,menu_selector(HelloWorld::mainMenuButtonCallback));
    menuButton->setColor(ccc3(0,0,0));

    menuButton->setPosition(ccp(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

    CCMenu *pMenu = CCMenu::create(menuButton,NULL);
    pMenu->setPosition(CCPointZero);

    mInGameMenuLayer->addChild(pMenu, 1);
}


void HelloWorld::createRoom()
{
    dbgprint("createRoom");

    std::map<std::string, std::string> properties;
    properties["playscape"] = "room12345";

    AppWarp::Client::getInstance()->createRoom(ROOM_NAME, mUserName, 2, properties);
}

void HelloWorld::joinLobby()
{
    AppWarp::Client::getInstance()->joinLobby();
}

void HelloWorld::subscribeLobby()
{
    AppWarp::Client::getInstance()->subscribeLobby();
}

void HelloWorld::leaveLobby()
{
    AppWarp::Client::getInstance()->leaveLobby();
}

void HelloWorld::unsubscribeLobby()
{
    AppWarp::Client::getInstance()->unsubscribeLobby();
}

void HelloWorld::getRooms()
{
    AppWarp::Client::getInstance()->getAllRooms();
}

void HelloWorld::updateRoomProperties()
{

}

void HelloWorld::removeStartGameLayer()
{
    removeChild(mStartGameLayer,true);
}

void HelloWorld::startGame()
{
    showInGameMenuLayer();

    // Initialize arrays
    mTargets = new CCArray();
    mProjectiles = new CCArray();

    // Get the dimensions of the window for calculation purposes
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    mPlayer = (Player*)CCSprite::create("Player.png");
    mPlayer->setPosition(ccp(mPlayer->getContentSize().width/2, winSize.height/2));
    mPlayer->isEnemy = false;
    addChild(mPlayer);

    mEnemy = (Player*)CCSprite::create("Enemy.png");
    mEnemy->setPosition(ccp(winSize.width-mEnemy->getContentSize().width/2, winSize.height/2));
    mEnemy->isEnemy = true;
    mEnemy->setOpacity(100);
    addChild(mEnemy);
    mIsConnected = true;
    if (mIsFirstLaunch)
    {
        scheduleUpdate();
    }

    std::map<std::string, double> additionalParams;
    additionalParams["meaningOfLife"] = 42.0;

    Report::getInstance().ReportMPStartGame(MAX_PLAYER);
    Report::getInstance().ReportLevelStarted("1", additionalParams);
}

void HelloWorld::stopGame() {
	mIsInRoom = false;
	removeMessageLayer();

	AppWarp::Client::getInstance()->leaveRoom(mRoomId);

	mRoomId = "";
	removeChild(mEnemy);
	removeChild(mPlayer);
	unscheduleUpdate();
	unscheduleRecover();
	showMessageLayer("Disconnecting...");

    std::map<std::string, double> additionalParams;
    additionalParams["meaningOfLife"] = 42.0;
    additionalParams["luckyNumber"] = 7.0;
	Report::getInstance().ReportLevelCompleted("1", additionalParams);

}

void HelloWorld::update(float time)
{

    if (!mIsEnemyAdded)
    {
        return;
    }

	CCArray *projectilesToDelete = CCArray::create();
    if (mProjectiles->count())
    {
        CCObject *pObj = NULL;
        CCARRAY_FOREACH(mProjectiles, pObj)
        {
            CCSprite *projectile = (CCSprite*)pObj;
            CCRect projectileRect = CCRectMake(projectile->getPosition().x - (projectile->getContentSize().width/2),
                                              projectile->getPosition().y - (projectile->getContentSize().height/2),
                                              projectile->getContentSize().width,
                                              projectile->getContentSize().height);

            CCArray *targetsToDelete = CCArray::create();

            CCObject *tObj = NULL;
            CCARRAY_FOREACH(mTargets, tObj)
            {
                CCSprite *target = (CCSprite*)tObj;
                CCRect targetRect = CCRectMake(target->getPosition().x - (target->getContentSize().width/2),
                                               target->getPosition().y - (target->getContentSize().height/2),
                                               target->getContentSize().width,
                                               target->getContentSize().height);

                if (projectileRect.intersectsRect(targetRect))
                {
                    targetsToDelete->addObject(target);
                }
                else if(!targetsToDelete->containsObject(target) && mPlayer->boundingBox().intersectsRect(targetRect))
                {
                     targetsToDelete->addObject(target);
                }

            }

            if (!projectilesToDelete->containsObject(projectile) && projectileRect.intersectsRect(mEnemy->boundingBox()))
            {
                projectilesToDelete->addObject(projectile);
            }
            tObj=NULL;
            CCARRAY_FOREACH(targetsToDelete, tObj)
            {
                CCSprite *target = (CCSprite*)tObj;
                mTargets->removeObject(target);
                removeChild(target, true);
                mProjectilesDestroyed++;

            }

            if (!projectilesToDelete->containsObject(projectile) && targetsToDelete->count() > 0)
            {
                projectilesToDelete->addObject(projectile);
            }
        }

         pObj=NULL;
        CCARRAY_FOREACH(projectilesToDelete, pObj)
        {
            CCSprite *projectile = (CCSprite*)pObj;
            mProjectiles->removeObject(projectile);
            removeChild(projectile, true);

        }
    }
    else
    {
        CCArray *targetsToDelete = CCArray::create();
        CCObject *tObj=NULL;
        CCARRAY_FOREACH(mTargets, tObj)
        {
            CCSprite *target = (CCSprite*)tObj;
            CCRect targetRect = CCRectMake(target->getPosition().x - (target->getContentSize().width/2),
                                           target->getPosition().y - (target->getContentSize().height/2),
                                           target->getContentSize().width,
                                           target->getContentSize().height);

            if (!targetsToDelete->containsObject(target) && mPlayer->boundingBox().intersectsRect(targetRect))
            {
                targetsToDelete->addObject(target);
            }

        }

       tObj=NULL;
        CCARRAY_FOREACH(targetsToDelete, tObj)
        {
            CCSprite *target = (CCSprite*)tObj;
            mTargets->removeObject(target);
            removeChild(target, true);
            mProjectilesDestroyed++;
        }
    }

}

void HelloWorld::updateEnemyStatus(CCPoint destination,float actualDuration)
{
    mEnemy->setOpacity(255);
    mIsEnemyAdded = true;
	CCSprite *target = CCSprite::create("Bullet-blue.png");
	target->setPosition(ccp(mEnemy->getPosition().x-mEnemy->getContentSize().width/2, mEnemy->getPosition().y));
	addChild(target,10);

    // Move projectile to actual endpoint
    CCActionInterval* move = CCMoveTo::create(actualDuration, destination);
    CCCallFuncN* moveFinished = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    CCSequence* seq = CCSequence::create(move,moveFinished,  NULL);
    target->runAction(seq);

	// Add to targets array
	target->setTag(3);
	mTargets->addObject(target);
}

void HelloWorld::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if (!mIsConnected)
    {
        return;
    }
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    CCPoint location = touch->getLocation();

    // Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *projectile = CCSprite::create("Bullet-red.png");
	projectile->setPosition(ccp(mPlayer->getPosition().x+mPlayer->getContentSize().width/2, mPlayer->getPosition().y));

    CCPoint projectilePos = projectile->getPosition();
	// Determine offset of location to projectile
	int offX = location.x - projectilePos.x;
	int offY = location.y - projectilePos.y;

	// Bail out if we are shooting down or backwards
	if (offX <= 0) return;

    // Ok to add now - we've double checked position
    addChild(projectile,10);


	// Determine where we wish to shoot the projectile to
	int realX = winSize.width + (projectile->getContentSize().width/2);
	float ratio = (float) offY / (float) offX;
	int realY = (realX * ratio) + projectilePos.y;
	CCPoint realDest = ccp(realX, realY);

	// Determine the length of how far we're shooting
	int offRealX = realX - projectilePos.x;
	int offRealY = realY - projectilePos.y;
	float length = sqrtf((offRealX*offRealX)+(offRealY*offRealY));
	float velocity = 480/1; // 480pixels/1sec
	float realMoveDuration = length/velocity;

    //CCPoint destination = CCPointMake(winSize.width-realDest.x, realDest.y);

    sendData(winSize.width-realDest.x, realDest.y, realMoveDuration);

	// Move projectile to actual endpoint
    CCActionInterval* move = CCMoveTo::create(realMoveDuration, realDest);
    CCCallFuncN* moveFinished = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    CCSequence* seq = CCSequence::create(move,moveFinished,  NULL);
    projectile->runAction(seq);
	// Add to projectiles array
	projectile->setTag(2);
    mProjectiles->addObject(projectile);

}

void HelloWorld::spriteMoveFinished(CCSprite* pSender)
{
    CCSprite *sprite = (CCSprite *)pSender;
	removeChild(sprite, true);

	if (sprite->getTag() == 3)
    { // target
		mTargets->removeObject(sprite);

	}
    else if (sprite->getTag() == 2)
    { // projectile
		mProjectiles->removeObject(sprite);
	}
}

void HelloWorld::mainMenuButtonCallback(CCObject* pSender)
{
    stopGame();
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

std::string genRandom()
{
	std::string charStr;
	srand (time(NULL));

	for (int i = 0; i < 10; ++i) {
		charStr += (char)(65+(rand() % (26)));
	}

	return charStr;
}


/********* Menu Callbacks *********/

void HelloWorld::openStoreButtonCallback(CCObject* pSender) {
	CCDirector::sharedDirector()->replaceScene(StoreScene::scene());
}

void HelloWorld::hostGameButtonCallback(CCObject* pSender) {
	mHostGameButtonClicked = true;
	joinGameButtonCallback(pSender);
}

void HelloWorld::joinGameButtonCallback(CCObject* pSender)
{
    AppWarp::Client *warpClientRef;
    if (mIsFirstLaunch)
    {
        mIsFirstLaunch = !mIsFirstLaunch;
        AppWarp::Client::initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->setRecoveryAllowance(60);
        warpClientRef->setConnectionRequestListener(this);
        warpClientRef->setNotificationListener(this);
        warpClientRef->setRoomRequestListener(this);
        warpClientRef->setZoneRequestListener(this);
        warpClientRef->setLobbyRequestListener(this);
        mUserName = genRandom();
    }

    connect(false);
    removeStartGameLayer();
}

void HelloWorld::connect(bool force) {
	if (!mIsConnected || force) {
		Report::getInstance().ReportMPServerConnect("AppWarpServer");
		AppWarp::Client::getInstance()->connect(mUserName);
		showMessageLayer("Connecting...");
	}
}

void HelloWorld::onDisconnectDone(int res)
{
	Report::getInstance().ReportMPLeaveGame(ROOM_NAME);
	Report::getInstance().ReportMPServerDisconnect();
	removeMessageLayer();
    mIsInRoom = false;
    showStartGameLayer();
    mIsConnected = false;
    mHostGameButtonClicked = false;
}

void HelloWorld::joinRoomIfNeeded() {
	if (!mIsInRoom) {
		AppWarp::Client *warpClientRef;
		warpClientRef = AppWarp::Client::getInstance();

		warpClientRef->joinRoomInUserRange(1, 1, false);
		showMessageLayer("Joining Room. Please wait...");
	}
}


void HelloWorld::simulateReceivedInviteButtonCallback(CCObject* pSender) {
    mStartedGameFromSimulatedInvite = true;

    Report::getInstance().ReportSocialRequestsFound(1);
    Report::getInstance().ReportSocialRequestDetails("socialRequestId", "sender-social-id", 12345);
	joinGameButtonCallback(this);
	dbgprint("simulateReceivedInviteButtonCallback");
}

void HelloWorld::onConnectDone(int res)
{
	removeMessageLayer();

    if (res==AppWarp::ResultCode::success)
    {

    	showMessageLayer("Connection Succeeded. Please wait...");

    	Report::getInstance().ReportMPServerConnectSuccess("AppWarp", 123/* if available latency in ms goes here*/);

        unscheduleRecover();
        dbgprint("\nonConnectDone .. SUCCESS..session=%d\n",AppWarp::AppWarpSessionID);
        joinRoomIfNeeded();

    }
    else if (res==AppWarp::ResultCode::success_recovered)
    {
        unscheduleRecover();
        dbgprint("\nonConnectDone .. SUCCESS with success_recovered..session=%d\n",AppWarp::AppWarpSessionID);
        joinRoomIfNeeded();

    }
    else if (res==AppWarp::ResultCode::connection_error_recoverable)
    {
    	Report::getInstance().ReportMPServerConnectFailed("AppWarp::ResultCode::connection_error_recoverable");
        scheduleRecover();
        dbgprint("\nonConnectDone .. FAILED..connection_error_recoverable..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (res==AppWarp::ResultCode::bad_request)
    {
    	Report::getInstance().ReportMPServerConnectFailed("AppWarp::ResultCode::bad_request");
        unscheduleRecover();
        dbgprint("\nonConnectDone .. FAILED with bad request..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (res==AppWarp::ResultCode::connection_error)
    {
    	Report::getInstance().ReportMPServerConnectFailed("AppWarp::ResultCode::connection_error");
        unscheduleRecover();
        dbgprint("\nonConnectDone .. FAILED with connection error..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else
    {
    	Report::getInstance().ReportMPServerConnectFailed("Unknown error");
    	removeStartGameLayer();
		unscheduleRecover();
		showStartGameLayer();

		if (AppWarp::AppWarpSessionID == 0) {
			connect(true);
		}
        dbgprint("\nonConnectDone .. FAILED with unknown reason..session=%d\n",AppWarp::AppWarpSessionID);

    }
}

void HelloWorld::scheduleRecover()
{
    schedule(schedule_selector(HelloWorld::recover), 5.0f);
    showMessageLayer("Reconnecting ...");
}

void HelloWorld::unscheduleRecover()
{
    unschedule(schedule_selector(HelloWorld::recover));
}

void HelloWorld::recover()
{
    dbgprint("\nHelloWorld::recover");
    AppWarp::Client::getInstance()->recoverConnection();
}

void HelloWorld::onJoinRoomDone(AppWarp::room revent)
{
	removeMessageLayer();

    if (revent.result==0 && !mHostGameButtonClicked)
    {
    	mRoomId = revent.roomId;
    	dbgprint("mRoomId = %s", mRoomId.c_str());

    	mIsInRoom = true;
        dbgprint("\nonJoinRoomDone .. SUCCESS\n");

        AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();

        warpClientRef->subscribeRoom(revent.roomId);

        startGame();
        removeStartGameLayer();

        // Public games are games played with random people, private games are game played with predefined friends.
        if (mStartedGameFromSimulatedInvite) {
        	// The session id must be kept through out a network game and be unique per game,
        	// when we created the room, we used the roomId field which is good enough - it is unique through out the session
        	Report::getInstance().ReportMPJoinedPrivateGame(revent.roomId/*sessionId*/, ROOM_NAME, mUserName.c_str()[0]);
        } else {
        	Report::getInstance().ReportMPJoinedPublicGame(revent.roomId/*sessionId*/, ROOM_NAME, mUserName.c_str()[0]);
        }
    }
    else {
    	createRoom();
    	if (!mHostGameButtonClicked) {
    		dbgprint("\nonJoinRoomDone .. FAILED, will try to create a room\n");
    	} else {
    		dbgprint("Host button clicked... creating room.");
    	}

    	mHostGameButtonClicked = false;
    }
}

void HelloWorld::onSubscribeRoomDone(AppWarp::room revent)
{
    if (revent.result==0)
    {
        dbgprint("\nonSubscribeRoomDone .. SUCCESS\n");
    }
    else
        dbgprint("\nonSubscribeRoomDone .. FAILED\n");
}


void HelloWorld::sendData(float x, float y, float duration)
{
    AppWarp::Client *warpClientRef;
	warpClientRef = AppWarp::Client::getInstance();

    std::stringstream str;
    str <<x << "x" <<y << "d" << duration;
    warpClientRef->sendChat(str.str());
}

void HelloWorld::onUpdatePropertyDone(AppWarp::liveroom revent)
{
    if (revent.result==0)
    {
        dbgprint("onUpdatePropertyDone....Success\n");
        std::map<std::string, std::string> properties = revent.properties;
        std::map<std::string,std::string>::iterator it;
        for(it = properties.begin(); it != properties.end(); ++it)
		{
			//cJSON_AddStringToObject(propJSON, it->first.c_str(),it->second.c_str());
            dbgprint("key= %s...value= %s\n",it->first.c_str(),it->second.c_str());
		}


    }
    else
    {
        dbgprint("\nonUpdatePropertyDone....Failed");
    }
}


void HelloWorld::onChatReceived(AppWarp::chat chatevent)
{
    dbgprint("onChatReceived..");
    if(chatevent.sender != mUserName)
	{
		std::size_t loc = chatevent.chat.find('x');
		std::string str1 = chatevent.chat.substr(0,loc);
		std::string str2 = chatevent.chat.substr(loc+1);
        loc = chatevent.chat.find('d');
        std::string str3 = chatevent.chat.substr(loc+1);
		float x = (float)std::atof (str1.c_str());
		float y = (float)std::atof(str2.c_str());
        float dest = (float)std::atof(str3.c_str());
        updateEnemyStatus(ccp(x,y), dest);
    }
}

void HelloWorld::showMessageLayer(std::string message)
{
	removeMessageLayer();

    // Get the dimensions of the window for calculation purposes
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    mMessageLayer = CCLayerColor::create();
    mMessageLayer->setColor(ccc3(0, 0, 0));
    mMessageLayer->setOpacity(50);
    addChild(mMessageLayer);

    CCLabelTTF *buttonTitle = CCLabelTTF::create(message.c_str(), "Marker Felt", 30);
    buttonTitle->setColor(ccBLUE);
    mMessageLayer->addChild(buttonTitle);
    buttonTitle->setPosition(ccp(winSize.width/2,winSize.height/2));

}

void HelloWorld::removeMessageLayer() {
	if (mMessageLayer) {
		removeChild(mMessageLayer);
		mMessageLayer = NULL;
	}
}

void HelloWorld::onUserPaused(std::string user,std::string locId,bool isLobby)
{
        dbgprint("\nonUserPaused...user=%s",user.c_str());
        dbgprint("\nonUserPaused...locId=%s",locId.c_str());
    std::string message = "Waiting for "+user+" response...";
    showMessageLayer(message);
}

void HelloWorld::onUserResumed(std::string user,std::string locId,bool isLobby)
{
        dbgprint("\nonUserResumed...user=%s",user.c_str());
        dbgprint("\nonUserResumed...locId=%s",locId.c_str());
    removeStartGameLayer();
}

void HelloWorld::onUserLeftRoom(AppWarp::room rData, std::string user)
{
    dbgprint("\nonUserLeftRoom...user=%s",user.c_str());
    removeStartGameLayer();
}



void HelloWorld::onCreateRoomDone(AppWarp::room revent)
{

    if (revent.result==0)
    {
    	mRoomId = revent.roomId;
    	dbgprint("mRoomId = %s", mRoomId.c_str());

        dbgprint("\nonCreateRoomDone...success\n");

    }

    if (mStartedGameFromSimulatedInvite) {

    	Report::getInstance().ReportMPCreatePrivateGame(revent.roomId, ROOM_NAME, MAX_PLAYER);
    } else {
    	std::map<std::string, std::string> gameParameters;
    	gameParameters["playscape"] = "room12345";

    	Report::getInstance().ReportMPCreatePublicGame(revent.roomId, MAX_PLAYER, gameParameters);
    }

    AppWarp::Client::getInstance()->joinRoom(revent.roomId);
}

void HelloWorld::onGetAllRoomsDone(AppWarp::liveresult res)
{
    if (res.result == 0)
    {
        std::vector<std::string>::iterator it;
        for(it = res.list.begin(); it != res.list.end(); ++it)
		{
            dbgprint("key= %s\n",it->c_str());
		}

    }
}
void HelloWorld::onGetMatchedRoomsDone(AppWarp::matchedroom mevent)
{
    if (mevent.result == 0)
    {
        std::vector<AppWarp::room>::iterator it;
        for(it = mevent.roomData.begin(); it != mevent.roomData.end(); ++it)
		{
            dbgprint("\nonGetMatchedRoomsDone..roomId= %s\n",it->roomId.c_str());
		}

    }
}

void HelloWorld::onLeaveRoomDone (AppWarp::room revent) {
	dbgprint("onLeaveRoom %s", revent.roomId.c_str());

	AppWarp::Client::getInstance()->disconnect();

}

void HelloWorld::onLeaveLobbyDone(AppWarp::lobby levent)
{
    if (levent.result == 0)
    {
        dbgprint("onLeaveLobbyDone .... success");
    }
    else
    {
        dbgprint("onLeaveLobbyDone .... Failed");
    }
}

void HelloWorld::onJoinLobbyDone(AppWarp::lobby levent)
{
    if (levent.result == 0)
    {
        dbgprint("onJoinLobbyDone .... success");
    }
    else
    {
        dbgprint("onJoinLobbyDone .... Failed");

    }
}

void HelloWorld::onSubscribeLobbyDone(AppWarp::lobby levent)
{
    if (levent.result == 0)
    {
        dbgprint("onSubscribeLobbyDone .... success");
    }
    else
    {
        dbgprint("onSubscribeLobbyDone .... Failed");

    }
}

void HelloWorld::onUnsubscribeLobbyDone(AppWarp::lobby levent)
{
    if (levent.result == 0)
    {
        dbgprint("onUnsubscribeLobbyDone .... success");
    }
    else
    {
        dbgprint("onUnsubscribeLobbyDone .... Failed");

    }
}
