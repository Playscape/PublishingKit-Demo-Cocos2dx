//
//  AppWarpCocos2dxSampleAppDelegate.cpp
//  AppWarpCocos2dxSample
//
//  Created by shephertz technologies on 11/07/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

#include "MuffinRushAssets.h"

#include "CCSoomla.h"
#include "playscape/PlayscapeX.h"

USING_NS_CC;
using namespace CocosDenshion;
//using namespace soomla;
using playscape::PlayscapeX;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    soomla::CCSoomla::initialize("soomlasecretsecret");

    // We initialize CCSoomlaStore and the event handler before
    // we open the store.
//    CCStoreEventDispatcher::getInstance()->addEventHandler(handler);
    
    // We initialize CCSoomlaStore and the event handler before
    // we open the store.
    MuffinRushAssets *assets = MuffinRushAssets::create();

    CCDictionary *storeParams = CCDictionary::create();
    storeParams->setObject(CCString::create("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAn5eClGHzFHjk741g5Mm7jFhFA0tQL0w/0NDx8sPHta+eScyLdgBM9SEkjSFJpDvQs4BHI9hhhCLfOG94gv3dN094mOzCKod1jdnFs0E0gsA24HHhQYnVnBhgFYM4vmD3mXAsxrceUr6KISlfkqkNrnoyQ/xTpzI97Z0EaoMLnKc7h/9w5jtLuaJnsVMdac3K8Ou0JJmoYyQZsOogkTeCUjyula8hPDDvj3h8bBCMJBxRAvi5QWtKQoDxwHhq2hNt1tSi07sJKvq5qKqtMnN+UC7O54kFhBFbe0lLfLkM24KFHcRBAiqFIVnGMTlZ/yCjJX8wNMXvyLDKoui9A0IpUwIDAQAB"), "androidPublicKey");
//    storeParams->setObject(CCBool::create(true), "testPurchases");
    
    soomla::CCSoomlaStore::initialize(assets, storeParams);
    
    /*
     * ** Set the amount of each currency to 10,000 if the **
     * ** balance drops under 1,000                        **
     *
     * ** Of course, this is just for testing...           **
     */
    
    CCArray *currencies =
    soomla::CCStoreInfo::sharedStoreInfo()->getCurrencies();
    CCObject *currencyObject;
    CCARRAY_FOREACH(currencies, currencyObject) {
        soomla::CCVirtualCurrency *vc =
        dynamic_cast<soomla::CCVirtualCurrency *>(currencyObject);
        int balance = soomla::CCStoreInventory::sharedStoreInventory()->
        getItemBalance(vc->getItemId()->getCString(), NULL);
        if (balance < 1000) {
            soomla::CCStoreInventory::sharedStoreInventory()->
            giveItem(vc->getItemId()->getCString(), 10000 - balance, NULL);
        }
    }

//    // initialize director
//    auto director = Director::getInstance();
//    auto glview = director->getOpenGLView();
//    if(!glview) {
//#if COCOS2D_VERSION > 0x00030200
//        glview = GLViewImpl::create("My Game");
//#else
//        glview = GLView::create("My Game");
//#endif
//        director->setOpenGLView(glview);
//    }

    PlayscapeX::onApplicationLaunched();

    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
