//
//  AppWarpCocos2dxSampleAppDelegate.h
//  AppWarpCocos2dxSample
//
//  Created by shephertz technologies on 11/07/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "CCApplication.h"
#include "CCSoomla.h"
#include "CCSoomlaEventDispatcher.h"
#include "CCSoomlaStore.h"
#include "CCStoreEventDispatcher.h"
#include "CCStoreInfo.h"
#include "CCStoreInventory.h"
#include "CCVirtualCurrency.h"

/**
@brief    The cocos2d Application.

The reason to implement with private inheritance is to hide some interface details of CCDirector.
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function is called when the application enters the background
    @param  the pointer of the application instance
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function is called when the application enters the foreground
    @param  the pointer of the application instance
    */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_
