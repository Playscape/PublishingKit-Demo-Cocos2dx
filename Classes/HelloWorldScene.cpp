#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "InGameMenuLayer.h"
#include "appwarp.h"
#include "dbgprint.h"
#include "StoreScene.h"

using namespace cocos2d;
using namespace CocosDenshion;


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

    _messageLayer = NULL;
    _isInRoom = false;

    _isFirstLaunch = true;
    setTouchEnabled(true);
    showStartGameLayer();

    return true;
}

void HelloWorld::showStartGameLayer()
{
    _startGameLayer = StartGameLayer::create();
    addChild(_startGameLayer);

    CCMenuItemFont *startGameButton = CCMenuItemFont::create("Play Game", this,menu_selector(HelloWorld::playGameButtonCallback));
    startGameButton->setColor(ccBLACK);

    CCMenuItemFont *openStoreButton = CCMenuItemFont::create("Open Store", this,menu_selector(HelloWorld::openStoreButtonCallback));
    openStoreButton->setColor(ccBLACK);

    CCMenu *pMenu = CCMenu::create(startGameButton, openStoreButton, NULL);

    pMenu->alignItemsVertically();
    _startGameLayer->addChild(pMenu, 1);
}

void HelloWorld::showInGameMenuLayer()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    _inGameMenuLayer = InGameMenuLayer::create();
    addChild(_inGameMenuLayer);

    CCMenuItemFont *menuButton = CCMenuItemFont::create("Menu", this,menu_selector(HelloWorld::mainMenuButtonCallback));
    menuButton->setColor(ccc3(0,0,0));

    menuButton->setPosition(ccp(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

    CCMenu *pMenu = CCMenu::create(menuButton,NULL);
    pMenu->setPosition(CCPointZero);

    _inGameMenuLayer->addChild(pMenu, 1);
}


void HelloWorld::createRoom()
{
    dbgprint("createRooms");
    
    std::map<std::string, std::string> properties;
    properties["playscape"] = "room123";
    
    AppWarp::Client::getInstance()->createRoom("room1", "max", 2, properties);
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

void HelloWorld::getRoomProperties()
{
    std::map<std::string, std::string> properties;
    properties["a"] = "a";
    AppWarp::Client::getInstance()->getRoomWithProperties(properties);
}

void HelloWorld::removeStartGameLayer()
{
    removeChild(_startGameLayer,true);
}

void HelloWorld::startGame()
{
    showInGameMenuLayer();

    // Initialize arrays
    _targets = new CCArray();
    _projectiles = new CCArray();
    
    // Get the dimensions of the window for calculation purposes
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    _player = (Player*)CCSprite::create("Player.png");
    _player->setPosition(ccp(_player->getContentSize().width/2, winSize.height/2));
    _player->isEnemy = false;
    addChild(_player);
    
    _enemy = (Player*)CCSprite::create("Enemy.png");
    _enemy->setPosition(ccp(winSize.width-_enemy->getContentSize().width/2, winSize.height/2));
    _enemy->isEnemy = true;
    _enemy->setOpacity(100);
    addChild(_enemy);
    _isConnected = true;
    if (_isFirstLaunch)
    {
        scheduleUpdate();
    }
    
}

void HelloWorld::stopGame() {
	_isInRoom = false;
	removeMessageLayer();
	AppWarp::Client::getInstance()->disconnect();
	removeChild(_enemy);
	removeChild(_player);
	unscheduleUpdate();
	unscheduleRecover();
	showMessageLayer("Disconnecting...");
}

void HelloWorld::update(float time)
{
    
    if (!_isEnemyAdded)
    {
        return;
    }
    
	CCArray *projectilesToDelete = CCArray::create();
    if (_projectiles->count())
    {
        CCObject *pObj = NULL;
        CCARRAY_FOREACH(_projectiles, pObj)
        {
            CCSprite *projectile = (CCSprite*)pObj;
            CCRect projectileRect = CCRectMake(projectile->getPosition().x - (projectile->getContentSize().width/2),
                                              projectile->getPosition().y - (projectile->getContentSize().height/2),
                                              projectile->getContentSize().width,
                                              projectile->getContentSize().height);
            
            CCArray *targetsToDelete = CCArray::create();
            
            CCObject *tObj = NULL;
            CCARRAY_FOREACH(_targets, tObj)
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
                else if(!targetsToDelete->containsObject(target) && _player->boundingBox().intersectsRect(targetRect))
                {
                     targetsToDelete->addObject(target);
                }
                
            }
            
            if (!projectilesToDelete->containsObject(projectile) && projectileRect.intersectsRect(_enemy->boundingBox()))
            {
                projectilesToDelete->addObject(projectile);
            }
            tObj=NULL;
            CCARRAY_FOREACH(targetsToDelete, tObj)
            {
                CCSprite *target = (CCSprite*)tObj;
                _targets->removeObject(target);
                removeChild(target, true);
                _projectilesDestroyed++;

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
            _projectiles->removeObject(projectile);
            removeChild(projectile, true);

        }
    }
    else
    {
        CCArray *targetsToDelete = CCArray::create();
        CCObject *tObj=NULL;
        CCARRAY_FOREACH(_targets, tObj)
        {
            CCSprite *target = (CCSprite*)tObj;
            CCRect targetRect = CCRectMake(target->getPosition().x - (target->getContentSize().width/2),
                                           target->getPosition().y - (target->getContentSize().height/2),
                                           target->getContentSize().width,
                                           target->getContentSize().height);
            
            if (!targetsToDelete->containsObject(target) && _player->boundingBox().intersectsRect(targetRect))
            {
                targetsToDelete->addObject(target);
            }
            
        }
        
       tObj=NULL;
        CCARRAY_FOREACH(targetsToDelete, tObj)
        {
            CCSprite *target = (CCSprite*)tObj;
            _targets->removeObject(target);
            removeChild(target, true);
            _projectilesDestroyed++;
        }
    }
    	
}

void HelloWorld::pauseGame()
{
    
}

void HelloWorld::updateEnemyStatus(CCPoint destination,float actualDuration)
{
    _enemy->setOpacity(255);
    _isEnemyAdded = true;
	CCSprite *target = CCSprite::create("Bullet-blue.png");
	target->setPosition(ccp(_enemy->getPosition().x-_enemy->getContentSize().width/2, _enemy->getPosition().y));
	addChild(target,10);
	
    // Move projectile to actual endpoint
    CCActionInterval* move = CCMoveTo::create(actualDuration, destination);
    CCCallFuncN* moveFinished = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    CCSequence* seq = CCSequence::create(move,moveFinished,  NULL);
    target->runAction(seq);

	// Add to targets array
	target->setTag(3);
	_targets->addObject(target);
}

void HelloWorld::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if (!_isConnected)
    {
        return;
    }
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    CCPoint location = touch->getLocation();

    // Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *projectile = CCSprite::create("Bullet-red.png");
	projectile->setPosition(ccp(_player->getPosition().x+_player->getContentSize().width/2, _player->getPosition().y));
	
    CCPoint projectilePos = projectile->getPosition();
	// Determine offset of location to projectile
	int offX = location.x - projectilePos.x;
	int offY = location.y - projectilePos.y;
	
	// Bail out if we are shooting down or backwards
	if (offX <= 0) return;
    
    // Ok to add now - we've double checked position
    addChild(projectile,10);
    
	// Play a sound!
    SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.caf");
	
	
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
    _projectiles->addObject(projectile);
	
}



void HelloWorld::spriteMoveFinished(CCSprite* pSender)
{
    CCSprite *sprite = (CCSprite *)pSender;
	removeChild(sprite, true);
	
	if (sprite->getTag() == 3)
    { // target
		_targets->removeObject(sprite);
		
	}
    else if (sprite->getTag() == 2)
    { // projectile
		_projectiles->removeObject(sprite);
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


/***
 * AppWarp Helper Methods
 */

void HelloWorld::disconnect()
{
    AppWarp::Client::getInstance()->disconnect();
    _isInRoom = false;
}

/********* Menu Callbacks *********/

void HelloWorld::openStoreButtonCallback() {
	CCDirector::sharedDirector()->replaceScene(StoreScene::scene());
}

void HelloWorld::playGameButtonCallback()
{
    _isConnected = false;
    AppWarp::Client *warpClientRef;
    if (_isFirstLaunch)
    {
        _isFirstLaunch = !_isFirstLaunch;
        AppWarp::Client::initialize(APPWARP_APP_KEY,APPWARP_SECRET_KEY);
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->setRecoveryAllowance(60);
        warpClientRef->setConnectionRequestListener(this);
        warpClientRef->setNotificationListener(this);
        warpClientRef->setRoomRequestListener(this);
        warpClientRef->setZoneRequestListener(this);
        warpClientRef->setLobbyRequestListener(this);
        _userName = genRandom();
    }


    if (!_isConnected) {
    	AppWarp::Client::getInstance()->connect(_userName);
    	showMessageLayer("Connecting...");
    }

    removeStartGameLayer();

}

void HelloWorld::onDisconnectDone(int res)
{
	removeMessageLayer();
    _isInRoom = false;
    showStartGameLayer();
    _isConnected = false;
}

void HelloWorld::joinRoomIfNeeded() {
	if (!_isInRoom) {
		AppWarp::Client *warpClientRef;
		warpClientRef = AppWarp::Client::getInstance();

	    std::map<std::string, std::string> properties;
	    properties["playscape"] = "room123";

		warpClientRef->joinRoomWithProperties(properties);
		showMessageLayer("Joining Room. Please wait...");
	}
}

void HelloWorld::onConnectDone(int res)
{
	removeMessageLayer();

    if (res==AppWarp::ResultCode::success)
    {

    	showMessageLayer("Connection Succeeded. Please wait...");

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
        scheduleRecover();
        dbgprint("\nonConnectDone .. FAILED..connection_error_recoverable..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (res==AppWarp::ResultCode::bad_request)
    {
        unscheduleRecover();
        dbgprint("\nonConnectDone .. FAILED with bad request..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (res==AppWarp::ResultCode::connection_error)
    {
        unscheduleRecover();
        dbgprint("\nonConnectDone .. FAILED with connection error..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else
    {
    	removeStartGameLayer();
		unscheduleRecover();
		showStartGameLayer();

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

    if (revent.result==0)
    {
    	_isInRoom = true;
        dbgprint("\nonJoinRoomDone .. SUCCESS\n");

        AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->subscribeRoom(revent.roomId);
        startGame();
        removeStartGameLayer();
    }
    else {
    	createRoom();
        dbgprint("\nonJoinRoomDone .. FAILED, will try to create a room\n");
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
    if(chatevent.sender != _userName)
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
    
    _messageLayer = CCLayerColor::create();
    _messageLayer->setColor(ccc3(0, 0, 0));
    _messageLayer->setOpacity(50);
    addChild(_messageLayer);
    
    CCLabelTTF *buttonTitle = CCLabelTTF::create(message.c_str(), "Marker Felt", 30);
    buttonTitle->setColor(ccBLUE);
    _messageLayer->addChild(buttonTitle);
    buttonTitle->setPosition(ccp(winSize.width/2,winSize.height/2));
    
}

void HelloWorld::removeMessageLayer() {
	if (_messageLayer) {
		removeChild(_messageLayer);
		_messageLayer = NULL;
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
        dbgprint("\nonCreateRoomDone...success\n");

    }
    joinRoomIfNeeded();
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
