#include <sys/time.h>
#include "SocialLoginScene.h"
#include "dbgprint.h"
#include "HelloWorldScene.h"
#include "InGameMenuLayer.h"
#include <map>

#import <FacebookSDK/FacebookSDK.h>

using namespace cocos2d;
using std::map;

// on "init" you need to initialize your instance
bool SocialLoginScene::init() {
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(cocos2d::ccc4(255, 255, 255, 255)) )
    {
        return false;
    }
    
    setTouchEnabled(true);
    
    mItemsMenu = CCMenu::create(
                                CCMenuItemFont::create("Facebook Login", this, menu_selector(SocialLoginScene::loginButtonCallback)),
                                CCMenuItemFont::create("Show Native Dialog", this, menu_selector(SocialLoginScene::nativeShareDialogButtonCallback)),
                                CCMenuItemFont::create("Share Dialog", this, menu_selector(SocialLoginScene::shareDialogButtonCallback)),
                                CCMenuItemFont::create("Submit score", this, menu_selector(SocialLoginScene::submitScoreButtonCallback)),
                                CCMenuItemFont::create("Send Request", this, menu_selector(SocialLoginScene::sendRequestButtonCallback)),
                                CCMenuItemFont::create("Facebook Logout", this, menu_selector(SocialLoginScene::logoutButtonCallback)),
                                NULL);
    
    CCObject* item;
    CCARRAY_FOREACH(mItemsMenu->getChildren(), item) {
        ((CCMenuItemFont*)item)->setColor(ccBLACK);
    }
    mItemsMenu->alignItemsVertically();
    addChild(mItemsMenu, 1);

    showInGameMenuLayer();

    mLoginInfoDialog = NULL;

    return true;
}

void SocialLoginScene::loginButtonCallback(CCObject* sender) {
    if (FBSession.activeSession.isOpen == NO && [FBSession openActiveSessionWithAllowLoginUI:YES] == NO) {
        [FBSession openActiveSessionWithPublishPermissions:@[@"public_profile", @"publish_actions", @"publish_stream"] defaultAudience:FBSessionDefaultAudienceEveryone allowLoginUI:YES completionHandler:^(FBSession *session, FBSessionState status, NSError *error) {
            showLoginDialog();
        }];
    } else {
        showLoginDialog();
    }
}

void SocialLoginScene::logoutButtonCallback(CCObject* sender) {
    /*NSHTTPCookie *cookie;
    NSHTTPCookieStorage *storage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    
    for (cookie in [storage cookies]) {
        NSString *domainName = [cookie domain];
        NSRange domainRange = [domainName rangeOfString:@"facebook"];
        
        if (domainRange.length > 0) {
            [storage deleteCookie:cookie];
        }
    }*/
    
    [FBSession.activeSession closeAndClearTokenInformation];
}

void SocialLoginScene::shareDialogButtonCallback(CCObject* sender) {
    if (FBSession.activeSession.isOpen == NO && [FBSession openActiveSessionWithAllowLoginUI:YES] == NO) {
        NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                       @"Sharing Tutorial", @"name",
                                       @"Build great social apps and get more installs.", @"caption",
                                       @"Allow your users to share stories on Facebook from your app using the iOS SDK.", @"description",
                                       @"https://developers.facebook.com/docs/ios/share/", @"link",
                                       @"http://i.imgur.com/g3Qc1HN.png", @"picture",
                                       nil];
        
        [FBRequestConnection startWithGraphPath:@"/me/feed"
                                     parameters:params
                                     HTTPMethod:@"POST"
                              completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
                                  if (!error) {
                                  } else {
                                  }
                              }];
    } else {
        NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                       @"Sharing Tutorial", @"name",
                                       @"Build great social apps and get more installs.", @"caption",
                                       @"Allow your users to share stories on Facebook from your app using the iOS SDK.", @"description",
                                       @"https://developers.facebook.com/docs/ios/share/", @"link",
                                       @"http://i.imgur.com/g3Qc1HN.png", @"picture",
                                       nil];
        
        [FBRequestConnection startWithGraphPath:@"/me/feed"
                                     parameters:params
                                     HTTPMethod:@"POST"
                              completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
                                  if (!error) {
                                  } else {
                                  }
                              }];
    }
}

void SocialLoginScene::nativeShareDialogButtonCallback(CCObject* sender) {
    if (FBSession.activeSession.isOpen == NO && [FBSession openActiveSessionWithAllowLoginUI:YES] == NO) {
        FBLinkShareParams *params = [[FBLinkShareParams alloc] init];
        params.link = [NSURL URLWithString:@"https://developers.facebook.com/docs/ios/share/"];
        
        if ([FBDialogs canPresentShareDialogWithParams:params]) {
            [FBDialogs presentShareDialogWithLink:params.link
                                          handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
                                              if (error) {
                                                  NSLog(@"Error publishing story: %@", error.description);
                                              } else {
                                                  NSLog(@"result %@", results);
                                              }
                                          }];
        } else {
            NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                           @"Sharing Tutorial", @"name",
                                           @"Build great social apps and get more installs.", @"caption",
                                           @"Allow your users to share stories on Facebook from your app using the iOS SDK.", @"description",
                                           @"https://developers.facebook.com/docs/ios/share/", @"link",
                                           @"http://i.imgur.com/g3Qc1HN.png", @"picture",
                                           nil];
            
            [FBWebDialogs presentFeedDialogModallyWithSession:nil
                                                   parameters:params
                                                      handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
                                                          if (error) {
                                                              NSLog(@"Error publishing story: %@", error.description);
                                                          } else {
                                                          }
                                                      }];
        }
    } else {
        FBLinkShareParams *params = [[FBLinkShareParams alloc] init];
        params.link = [NSURL URLWithString:@"https://developers.facebook.com/docs/ios/share/"];
        
        if ([FBDialogs canPresentShareDialogWithParams:params]) {
            [FBDialogs presentShareDialogWithLink:params.link
                                          handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
                                              if (error) {
                                                  NSLog(@"Error publishing story: %@", error.description);
                                              } else {
                                                  NSLog(@"result %@", results);
                                              }
                                          }];
        } else {
            NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                           @"Sharing Tutorial", @"name",
                                           @"Build great social apps and get more installs.", @"caption",
                                           @"Allow your users to share stories on Facebook from your app using the iOS SDK.", @"description",
                                           @"https://developers.facebook.com/docs/ios/share/", @"link",
                                           @"http://i.imgur.com/g3Qc1HN.png", @"picture",
                                           nil];
            
            [FBWebDialogs presentFeedDialogModallyWithSession:nil
                                                   parameters:params
                                                      handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
                                                          if (error) {
                                                              NSLog(@"Error publishing story: %@", error.description);
                                                          } else {
                                                          }
                                                      }];
        }
    }
}

void SocialLoginScene::submitScoreButtonCallback(CCObject* sender) {
    if (FBSession.activeSession.isOpen == NO && [FBSession openActiveSessionWithAllowLoginUI:YES] == NO) {
        NSMutableDictionary* params = [NSMutableDictionary dictionaryWithObjectsAndKeys:@"100", @"score", nil];
        [FBRequestConnection startWithGraphPath:@"/me/scores"
                                     parameters:params
                                     HTTPMethod:@"POST"
                              completionHandler: ^(FBRequestConnection *connection, id result, NSError *error) {
                                  if (error) {
                                  } else {
                                  }
                              }];
    } else {
        NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:@"100", @"score", nil];
        
        [FBRequestConnection startWithGraphPath:@"/me/scores"
                                     parameters:params
                                     HTTPMethod:@"POST"
                              completionHandler: ^(FBRequestConnection *connection, id result, NSError *error) {
                                  if (error) {
                                  } else {
                                  }
                              }];
    }
}

void SocialLoginScene::sendRequestButtonCallback(CCObject *sender) {
    if (FBSession.activeSession.isOpen == NO && [FBSession openActiveSessionWithAllowLoginUI:YES] == NO) {
        NSMutableDictionary* params = [NSMutableDictionary dictionaryWithObjectsAndKeys: @"72014", @"to",
                                       @"send", @"action_type",
                                       @"0987654321", @"object_id", nil];
        
        [FBWebDialogs presentRequestsDialogModallyWithSession:nil message:@"Take this bomb to blast your way to victory!" title:nil parameters:params handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
            if (error) {
            } else {
            }
        }];
    } else {
        NSMutableDictionary* params = [NSMutableDictionary dictionaryWithObjectsAndKeys: @"72014", @"to",
                                       @"send", @"action_type",
                                       @"0987654321", @"object_id", nil];
        
        [FBWebDialogs presentRequestsDialogModallyWithSession:nil message:@"Take this bomb to blast your way to victory!" title:nil parameters:params handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
            if (error) {
            } else {
            }
        }];
    }
}

void SocialLoginScene::showInGameMenuLayer() {
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    InGameMenuLayer* _inGameMenuLayer = InGameMenuLayer::create();
    addChild(_inGameMenuLayer);

	CCMenuItemFont *menuButton = CCMenuItemFont::create("Menu", this,menu_selector(SocialLoginScene::menuButtonCallback));
	menuButton->setColor(ccc3(0,0,0));

	menuButton->setPosition(ccp(winSize.width - menuButton->getContentSize().width, winSize.height - menuButton->getContentSize().height));

	CCMenu *pMenu = CCMenu::create(menuButton,NULL);
	pMenu->setPosition(CCPointZero);

	_inGameMenuLayer->addChild(pMenu, 1);
}

void SocialLoginScene::menuButtonCallback(CCObject* sender) {
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void SocialLoginScene::showLoginDialog() {
	if (mLoginInfoDialog) {
		removeChild(mLoginInfoDialog);
	}

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	mLoginInfoDialog = CCLayerColor::create(ccc4(0xcc, 0xcc, 0xcc, 0xff), (winSize.width * 0.8f), (winSize.height * 0.8f));
	addChild(mLoginInfoDialog);
    
    [[FBRequest requestForMe] startWithCompletionHandler:^(FBRequestConnection *connection, NSDictionary<FBGraphUser> *FBuser, NSError *error) {
        string question;
        
        if (error == nil) {
            NSString *userName = [FBuser name];
            
            question = string("Your name is ") + [userName UTF8String];
        } else {
            question = string("Can't get username");
        }
        CCLabelTTF* questionLabel = CCLabelTTF::create(question.c_str(), "Marker Felt", 26);
        
        questionLabel->setAnchorPoint(CCPointZero);
        mLoginInfoDialog->setAnchorPoint(CCPointZero);
        mLoginInfoDialog->setPosition(ccp(winSize.width/2 - mLoginInfoDialog->getContentSize().width/2,
                                          winSize.height/2 - mLoginInfoDialog->getContentSize().height/2));
        
        questionLabel->setPosition(ccp(mLoginInfoDialog->getContentSize().width/2 - questionLabel->getContentSize().width/2,
                                       mLoginInfoDialog->getContentSize().height/2 - questionLabel->getContentSize().height/2));
        
        mLoginInfoDialog->removeAllChildren();
        mLoginInfoDialog->addChild(questionLabel);
        
        CCMenuItemFont *okItem = CCMenuItemFont::create("Ok", this,menu_selector(SocialLoginScene::okButtonCallback));
        okItem->setColor(ccBLACK);
        okItem->setFontSizeObj(50);
        
        CCMenu *pMenu =
        CCMenu::createWithItem(okItem);
        
        mLoginInfoDialog->addChild(pMenu);
        pMenu->alignItemsHorizontally();
        pMenu->setAnchorPoint(CCPointZero);
        pMenu->setPosition(ccp(mLoginInfoDialog->getPosition().x + mLoginInfoDialog->getContentSize().width/2 , mLoginInfoDialog->getPosition().y + 50));
    }];

	mItemsMenu->setEnabled(false);
	mItemsMenu->setVisible(false);
    mLoginInfoDialog->setVisible(true);
}

void SocialLoginScene::okButtonCallback(CCObject* sender) {
    mLoginInfoDialog->setVisible(false);
    mItemsMenu->setEnabled(true);
    mItemsMenu->setVisible(true);
}

CCScene* SocialLoginScene::scene() {
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    SocialLoginScene *layer = SocialLoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
