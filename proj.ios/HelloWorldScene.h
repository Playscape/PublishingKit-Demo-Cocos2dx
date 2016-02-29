#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "StartGameLayer.h"
#include "InGameMenuLayer.h"
#include "appwarp.h"
#include "playscape/Report.h"

#define APPWARP_APP_KEY     "1a7bd5e37ca03fa987fd3ab3c119892f8a624bce4b3525533785e8c4f9d5b96d"
#define APPWARP_SECRET_KEY  "8a88fbc2c03da4c3096965cc1089e9294b728023b7fc9d0aa62f591731ec7b22"
#define GAME_NAME           "pspubkitdemo"
#define ROOM_ID             "596896358"
#define ROOM_NAME           "testroom"
#define ROOM_OWNER          "tester"
#define MAX_PLAYER          2

using namespace cocos2d;
using playscape::MPAnalyticsProvider;
using playscape::SocialAnalyticsProvider;

class AppWarpMultiplayerProvider : public MPAnalyticsProvider {

public:
	// this should be the time since the network session started in seconds, a time that is synchronized between all players is preferable.
	inline int32_t getCurrentNetworkTime() {
		return 42;
	}
};

class FacebookSocialAnalyticsProvider : public SocialAnalyticsProvider {
public:
	playscape::SocialNetwork getCurrentNetwork() const {
		return playscape::Facebook;
	}
};

class HelloWorld : public LayerColor,
				   public AppWarp::ConnectionRequestListener,
				   public AppWarp::RoomRequestListener,
				   public AppWarp::NotificationListener,
				   public AppWarp::ZoneRequestListener,
				   public AppWarp::LobbyRequestListener
{
    
public:

    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static Scene* scene();
    void showStartGameLayer();
    void updateRoomProperties();
    void createRoom();
    void getRooms();
    void removeStartGameLayer();
    void removeMessageLayer();

    // menu callbacks
    void menuCloseCallback(Ref* pSender);
    void mainMenuButtonCallback(Ref* pSender);
    void openAdsTest(Ref* sender);
    void inviteFriendsButtonCallback(Ref* pSender);
    void simulateReceivedInviteButtonCallback(Ref* pSender);
    void joinGameButtonCallback(Ref* pSender);
    void hostGameButtonCallback(Ref* pSender);
    void openStoreButtonCallback(Ref* pSender);
    void simulateSocialNetworkLoginCallback(Ref* pSender);
    void setCustomTagsCallback(Ref* pSender);
    void openReportTest(Ref* pSender);

    void joinRoomIfNeeded();

    void startGame();
    void updateEnemyStatus(Vec2 pos,float duration);
    void spriteMoveFinished(Sprite* pSender);
    void sendData(float x, float y, float duration);
    virtual void onTouchesEnded(__Set *pTouches, Event *pEvent);
    virtual void update(float time);
    
    void joinLobby();
    void subscribeLobby();
    void leaveLobby();
    void unsubscribeLobby();
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
    
    void onConnectDone(int res);
    void onDisconnectDone(int res);
    void onJoinRoomDone(AppWarp::room revent);
    void onSubscribeRoomDone(AppWarp::room revent);
    void onChatReceived(AppWarp::chat chatevent);
    void onUserPaused(std::string user,std::string locId,bool isLobby);
    void onUserResumed(std::string user,std::string locId,bool isLobby);
    void showMessageLayer(std::string message);
    void onUpdatePropertyDone(AppWarp::liveroom revent);
    void onGetAllRoomsDone(AppWarp::liveresult res);
    void onCreateRoomDone(AppWarp::room revent);
    void onGetMatchedRoomsDone(AppWarp::matchedroom mevent);
    void onLeaveLobbyDone(AppWarp::lobby levent);
    void onJoinLobbyDone(AppWarp::lobby levent);
    void onSubscribeLobbyDone(AppWarp::lobby levent);
    void onUnsubscribeLobbyDone(AppWarp::lobby levent);
    void onUserLeftRoom(AppWarp::room rData, std::string user);
    void onLeaveRoomDone (AppWarp::room revent);
    void showInGameMenuLayer();

    void scheduleRecover();
    void unscheduleRecover();
    void recover(float d);
    void stopGame();

    void setSomeCustomVars();
    void connect(bool force);

private:
    AppWarpMultiplayerProvider *mAppWarpMultiplayerProvider;
    FacebookSocialAnalyticsProvider *mFacebookSocialAnalyticsProvider;
    __Array *mTargets;
    __Array *mProjectiles;

    std::string mRoomId;

    int mProjectilesDestroyed;
    Player *mPlayer;
    Player *mEnemy;
    int mScore;
    bool mIsEnemyAdded;
    bool mIsConnected;
    std::string mUserName;
    bool mIsFirstLaunch;
    StartGameLayer *mStartGameLayer;
    InGameMenuLayer *mInGameMenuLayer;
    LayerColor *mMessageLayer;
    Layer *mGameLayer;
    bool mIsInRoom;
    bool mStartedGameFromSimulatedInvite;

    bool mIsSocialNetworkLoggedIn;

    bool mHostGameButtonClicked;
};


#endif // __HELLOWORLD_SCENE_H__
