#ifndef __InviteFriendsScene_h__
#define __InviteFriendsScene_h__

#include <iostream>
#include <string>
#include "cocos2d.h"

using namespace cocos2d;
using std::string;
class InviteFriendsScene : public CCLayerColor
{
protected:
	void showInGameMenuLayer();

	// Ingame menu Callbacks
	void menuButtonCallback(CCObject* sender);

	// Friend Invite Callbacks
	void inviteJohnSnow(CCObject* sender);
    void inviteJackSparrow(CCObject* sender);
	void invitePaulAtreides(CCObject* sender);
	void inviteButtonCallback(CCObject* sender);
	void cancelInviteButtonCallback(CCObject* sender);

	void showInviteDialog(const string& friendName);

	void hideInviteDialog();

	CCMenu* mPlaysWithMenu;

	CCLayerColor* mInviteDialog;
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(InviteFriendsScene);
};


#endif /* defined(__InviteFriendsScene_h__) */
