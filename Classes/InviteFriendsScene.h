#ifndef __InviteFriendsScene_h__
#define __InviteFriendsScene_h__

#include <iostream>
#include <string>
#include "cocos2d.h"

using namespace cocos2d;
using std::string;
class InviteFriendsScene : public LayerColor
{
protected:
	void showInGameMenuLayer();

	// Ingame menu Callbacks
	void menuButtonCallback(Ref* sender);

	// Friend Invite Callbacks
	void inviteJohnSnow(Ref* sender);
    void inviteJackSparrow(Ref* sender);
	void invitePaulAtreides(Ref* sender);
	void inviteButtonCallback(Ref* sender);
	void cancelInviteButtonCallback(Ref* sender);

	void showInviteDialog(const string& friendName);

	void hideInviteDialog();

	Menu* mPlaysWithMenu;

	LayerColor* mInviteDialog;
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::Scene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(InviteFriendsScene);
};


#endif /* defined(__InviteFriendsScene_h__) */
