//
// Created by Fedor Shubin on 5/24/13.
//


#include "MuffinRushAssets.h"

#include "CCVirtualCurrency.h"
#include "CCVirtualCurrencyPack.h"
#include "CCVirtualGood.h"
#include "CCSingleUseVG.h"
#include "CCPurchaseWithMarket.h"
#include "CCPurchaseWithVirtualItem.h"
#include "CCVirtualCategory.h"
#include "CCUpgradeVG.h"
#include "CCSingleUsePackVG.h"
#include "CCEquippableVG.h"

USING_NS_CC;
using namespace soomla;

MuffinRushAssets *MuffinRushAssets::create() {
    MuffinRushAssets *ret = new MuffinRushAssets();
    ret->autorelease();
    ret->init();

    return ret;
}

bool MuffinRushAssets::init() {
    /** Virtual Currencies **/
    CCVirtualCurrency *muffinCurrency = CCVirtualCurrency::create(
            String::create("Muffins"),
            String::create(""),
            String::create(MUFFIN_CURRENCY_ITEM_ID)
    );


    /** Virtual Currency Packs **/

    CCVirtualCurrencyPack *samuraiShield = CCVirtualCurrencyPack::create(
            String::create("Samurai Shield"),                                   // name
            String::create("Test refund of an item"),                       // description
            String::create("samurai_shield"),                                   // item id
            Integer::create(10),                                              // number of currencies in the pack
            String::create(MUFFIN_CURRENCY_ITEM_ID),                        // the currency associated with this pack
            CCPurchaseWithMarket::create(String::create("samurai_shield"), Double::create(0.99))
    );

    CCVirtualCurrencyPack *ninjaStars = CCVirtualCurrencyPack::create(
            CCString::create("Ninjs Stars"),                                   // name
            CCString::create("Test refund of an item"),                       // description
            CCString::create("ninja_stars"),                                   // item id
            CCInteger::create(10),                                              // number of currencies in the pack
            CCString::create(MUFFIN_CURRENCY_ITEM_ID),                        // the currency associated with this pack
            CCPurchaseWithMarket::create(String::create("ninja_stars"), Double::create(0.99))
    );

    CCVirtualCurrencyPack *ninjaSword = CCVirtualCurrencyPack::create(
            CCString::create("Ninja sword"),                                   // name
            CCString::create("Test refund of an item"),                       // description
            CCString::create("ninjs_sword"),                                   // item id
            CCInteger::create(1),                                               // number of currencies in the pack
            CCString::create(MUFFIN_CURRENCY_ITEM_ID),                        // the currency associated with this pack
            CCPurchaseWithMarket::create(String::create("ninjs_sword"), Double::create(0.99))
    );

    CCVirtualCurrencyPack *shougunKatana = CCVirtualCurrencyPack::create(
            CCString::create("Shougun Katana"),                                   // name
            CCString::create("Test cancellation of an item"),                 // description
            CCString::create("shogun_katana"),                                   // item id
            CCInteger::create(50),                                              // number of currencies in the pack
            CCString::create(MUFFIN_CURRENCY_ITEM_ID),                        // the currency associated with this pack
            CCPurchaseWithMarket::create(String::create("shogun_katana"), Double::create(1.99))
    );

    /** Google MANAGED Items **/

    CCVirtualGood *noAdsLTVG = CCLifetimeVG::create(
            CCString::create("No Ads"),
            CCString::create("No More Ads!"),
            CCString::create("no_ads"),
            CCPurchaseWithMarket::createWithMarketItem(CCMarketItem::create(
                    String::create(NO_ADDS_LTVG_PRODUCT_ID),
                    Double::create(1.99))
            )
    );

    mCurrencies = __Array::create(muffinCurrency, NULL);
    mCurrencies->retain();

    mCurrencyPacks = __Array::create(samuraiShield, ninjaStars, shougunKatana, ninjaSword, NULL);
    mCurrencyPacks->retain();

    return true;
}

MuffinRushAssets::~MuffinRushAssets() {
    CC_SAFE_RELEASE(mCurrencies);
    CC_SAFE_RELEASE(mCurrencyPacks);
    CC_SAFE_RELEASE(mCategories);
}

int MuffinRushAssets::getVersion() {
    return 2;
}

cocos2d::__Array *MuffinRushAssets::getCurrencies() {
    return mCurrencies;
}

cocos2d::__Array *MuffinRushAssets::getGoods() {
    return NULL;
}

cocos2d::__Array *MuffinRushAssets::getCurrencyPacks() {
    return mCurrencyPacks;
}

cocos2d::__Array *MuffinRushAssets::getCategories() {
    return mCategories;
}
