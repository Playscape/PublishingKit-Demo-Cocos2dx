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
    storeParams->setObject(CCString::create("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAg2TaRDmaGqRiQA+XAeN/X5FfxtH+l6vwxBO4tgOlrC/M+qcTqmRfkdTJe6Y4ZVCPV2krdbp/HK3mhKVoFVhrS71hoxQW8LrXxiWgDcessd4uCg+ZDdF4Utg5GZgk3V3B1fIHHkb1Eb2I63Qp0Fyt3lPU4R0U0S261l4x4fxvolcHB44TlG1EXhOyC1p8uKSIjKgKtyy8bJ9oAL19K4zyqSAtYV9v2ogm7MgQW11z7HRXRjnn0IZqHe4aX18zb5vUM3GD/gpJM2Bi0koA2N/0fT3Nu/fwovCJZQfECFQD5qLWj5L79bys68YrBgW46lDIZGZ0JFqpD12QXjpeeou3xwIDAQAB"), "androidPublicKey");
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
