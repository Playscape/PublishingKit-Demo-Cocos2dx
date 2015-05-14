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
            CCString::create("Muffins"),
            CCString::create(""),
            CCString::create(MUFFIN_CURRENCY_ITEM_ID)
    );


    /** Virtual Currency Packs **/

    CCVirtualCurrencyPack *samuraiShield = CCVirtualCurrencyPack::create(
            CCString::create("Samurai Shield"),                                   // name
            CCString::create("Test refund of an item"),                       // description
            CCString::create("dark_matter"),                                   // item id
            CCInteger::create(10),												// number of currencies in the pack
            CCString::create(MUFFIN_CURRENCY_ITEM_ID),                        // the currency associated with this pack
            CCPurchaseWithMarket::create(CCString::create("dark_matter"), CCDouble::create(0.99))
    );

    CCVirtualCurrencyPack *ninjaStars = CCVirtualCurrencyPack::create(
            CCString::create("Ninja stars"),                                   // name
            CCString::create("Test refund of an item"),                       // description
            CCString::create("diamond_item"),                                   // item id
            CCInteger::create(1),												// number of currencies in the pack
            CCString::create(MUFFIN_CURRENCY_ITEM_ID),                        // the currency associated with this pack
            CCPurchaseWithMarket::create(CCString::create("diamond_item"), CCDouble::create(0.99))
    );

    CCVirtualCurrencyPack *shougunKatana = CCVirtualCurrencyPack::create(
            CCString::create("Shougun Katana"),                                   // name
            CCString::create("Test cancellation of an item"),                 // description
            CCString::create("dark_matter_unmanaged"),                                   // item id
            CCInteger::create(50),												// number of currencies in the pack
            CCString::create(MUFFIN_CURRENCY_ITEM_ID),                        // the currency associated with this pack
            CCPurchaseWithMarket::create(CCString::create("dark_matter_unmanaged"), CCDouble::create(1.99))
    );

    /** Google MANAGED Items **/

    CCVirtualGood *noAdsLTVG = CCLifetimeVG::create(
            CCString::create("No Ads"),
            CCString::create("No More Ads!"),
            CCString::create("no_ads"),
            CCPurchaseWithMarket::createWithMarketItem(CCMarketItem::create(
                    CCString::create(NO_ADDS_LTVG_PRODUCT_ID),
                    CCInteger::create(CCMarketItem::NONCONSUMABLE), CCDouble::create(1.99))
            )
    );

    mCurrencies = CCArray::create(muffinCurrency, NULL);
    mCurrencies->retain();

    mCurrencyPacks = CCArray::create(samuraiShield, ninjaStars, shougunKatana, NULL);
    mCurrencyPacks->retain();

    return true;
}

MuffinRushAssets::~MuffinRushAssets() {
    CC_SAFE_RELEASE(mCurrencies);
    CC_SAFE_RELEASE(mCurrencyPacks);
    CC_SAFE_RELEASE(mCategories);
}

int MuffinRushAssets::getVersion() {
    return 1;
}

cocos2d::CCArray *MuffinRushAssets::getCurrencies() {
    return mCurrencies;
}

cocos2d::CCArray *MuffinRushAssets::getGoods() {
    return NULL;
}

cocos2d::CCArray *MuffinRushAssets::getCurrencyPacks() {
    return mCurrencyPacks;
}

cocos2d::CCArray *MuffinRushAssets::getCategories() {
    return mCategories;
}
