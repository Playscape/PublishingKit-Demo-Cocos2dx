/*
 Copyright (C) 2012-2014 Soomla Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */



#include "CCStoreInfo.h"
#include "CCSingleUseVG.h"
#include "CCEquippableVG.h"
#include "CCSingleUsePackVG.h"
#include "CCVirtualCurrency.h"
#include "CCVirtualCurrencyPack.h"
#include "CCMarketItem.h"
#include "CCPurchaseWithMarket.h"
#include "CCDomainFactory.h"
#include "CCDomainHelper.h"
#include "CCSoomlaUtils.h"
#include "CCKeyValueStorage.h"
#include "CCJsonHelper.h"
#include "CCNativeStoreInfo.h"
#include "CCSoomlaEventDispatcher.h"

namespace soomla {

    #define TAG "SOOMLA StoreInfo"
    
    #define KEY_META_STORE_INFO "meta.storeinfo"

    USING_NS_CC;

    static CCStoreInfo *s_SharedStoreInfo = NULL;

    CCStoreInfo *CCStoreInfo::sharedStoreInfo() {
        return s_SharedStoreInfo;
    }

    void CCStoreInfo::createShared(CCStoreAssets *storeAssets) {
        CCStoreInfo *ret = NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ret = new CCNativeStoreInfo();
#else
        ret = new CCStoreInfo();
#endif
        if (ret->init(storeAssets)) {
            s_SharedStoreInfo = ret;
        } else {
            delete ret;
        }
    }

    bool CCStoreInfo::init(CCStoreAssets *storeAssets) {
        CCSoomlaUtils::logDebug(TAG, "Setting store assets in SoomlaInfo");
        
        if (storeAssets == NULL){
            CCSoomlaUtils::logError(TAG, "The given store assets can't be null!");
            return false;
        }
        
        // support reflection call to initializeFromDB
        CCSoomlaEventDispatcher::getInstance()->registerEventHandler("Reflection::CCStoreInfo::initializeFromDB",
                                                                     this, (SEL_EventHandler)(&CCStoreInfo::handle__REFLECTION_INITIALIZE_FROM_DB));
        
        setStoreAssets(storeAssets);
        
        // At this point we have StoreInfo JSON saved at the local key-value storage. We can just
        // continue by initializing from DB.
        
        initializeFromDB();
        
        return true;
    }

    CCVirtualItem *CCStoreInfo::getItemByItemId(char const *itemId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
							   CCString::createWithFormat("Trying to fetch an item with itemId: %s", itemId)->getCString());

        CCVirtualItem *item = dynamic_cast<CCVirtualItem *>(getVirtualItems()->objectForKey(itemId));
        if (item != NULL) {
            return item;
        }
        
        CCError::tryFillError(error, CCString::createWithFormat("Virtual item was not found when searching with itemId=%s", itemId));
        return NULL;
    }

    CCPurchasableVirtualItem *CCStoreInfo::getPurchasableItemWithProductId(char const *productId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
                                CCString::createWithFormat("Trying to fetch a purchasable item with productId: %s", productId)->getCString());
        
        CCPurchasableVirtualItem *item = dynamic_cast<CCPurchasableVirtualItem *>(getPurchasableItems()->objectForKey(productId));
        if (item != NULL) {
            return item;
        }
        
        CCError::tryFillError(error, CCString::createWithFormat("Virtual item was not found when searching with productId=%s", productId));
        return NULL;
    }

    CCVirtualCategory *CCStoreInfo::getCategoryForVirtualGood(char const *goodItemId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
                                CCString::createWithFormat("Trying to fetch a category for a good with itemId: %s", goodItemId)->getCString());
        
        CCVirtualCategory *category = dynamic_cast<CCVirtualCategory *>(getGoodsCategories()->objectForKey(goodItemId));
        if (category != NULL) {
            return category;
        }
        
        CCError::tryFillError(error, CCString::createWithFormat("Virtual item was not found when searching with goodItemId of category=%s", goodItemId));
        return NULL;
    }

    CCUpgradeVG *CCStoreInfo::getFirstUpgradeForVirtualGood(char const *goodItemId) {
        CCSoomlaUtils::logDebug(TAG,
                                CCString::createWithFormat("Trying to fetch first upgrade of a good with itemId: %s", goodItemId)->getCString());
        
        CCArray *upgrades = dynamic_cast<CCArray *>(getGoodsUpgrades()->objectForKey(goodItemId));
        if (upgrades != NULL) {
            CCObject *obj;
            CCARRAY_FOREACH(upgrades, obj) {
                CCUpgradeVG *upgradeVG = dynamic_cast<CCUpgradeVG *>(obj);
                if (CCSoomlaUtils::isNullOrEmpty(upgradeVG->getPrevItemId())) {
                    return upgradeVG;
                }
            }
        }
        
        return NULL;
    }

    CCUpgradeVG *CCStoreInfo::getLastUpgradeForVirtualGood(char const *goodItemId) {
        CCSoomlaUtils::logDebug(TAG,
                                CCString::createWithFormat("Trying to fetch last upgrade of a good with itemId: %s", goodItemId)->getCString());
        
        CCArray *upgrades = dynamic_cast<CCArray *>(getGoodsUpgrades()->objectForKey(goodItemId));
        if (upgrades != NULL) {
            CCObject *obj;
            CCARRAY_FOREACH(upgrades, obj) {
                CCUpgradeVG *upgradeVG = dynamic_cast<CCUpgradeVG *>(obj);
                if (CCSoomlaUtils::isNullOrEmpty(upgradeVG->getNextItemId())) {
                    return upgradeVG;
                }
            }
        }
        
        return NULL;
    }

    CCArray *CCStoreInfo::getUpgradesForVirtualGood(char const *goodItemId) {
        CCSoomlaUtils::logDebug(TAG,
                                CCString::createWithFormat("Trying to fetch upgrades of a good with itemId: %s", goodItemId)->getCString());
        
        CCArray *upgrades = dynamic_cast<CCArray *>(getGoodsUpgrades()->objectForKey(goodItemId));
        return upgrades;
    }

    void CCStoreInfo::saveItem(CCVirtualItem *virtualItem, bool saveToDB) {
        replaceVirtualItem(virtualItem);
        
        if (saveToDB) {
            save();
        }
    }
    
    void CCStoreInfo::saveItems(cocos2d::CCArray *virtualItems, bool saveToDB) {
        if ((virtualItems == NULL) || (virtualItems->count() == 0)) {
            return;
        }
        
        CCObject *obj;
        CCARRAY_FOREACH(virtualItems, obj) {
            CCVirtualItem *virtualItem = dynamic_cast<CCVirtualItem *>(obj);
            CC_ASSERT(virtualItem);
            replaceVirtualItem(virtualItem);
        }
        
        if (saveToDB) {
            save();
        }
    }
    
    void CCStoreInfo::save() {
        CCDictionary *storeDict = toDictionary();
        const char *jsonString = json_dumps(CCJsonHelper::getJsonFromCCObject(storeDict), JSON_COMPACT | JSON_ENSURE_ASCII);
        CCSoomlaUtils::logDebug(TAG, CCString::createWithFormat("saving StoreInfo to DB. json is: %s", jsonString)->getCString());
        CCKeyValueStorage::getInstance()->setValue(KEY_META_STORE_INFO, jsonString);
    }

    CCDictionary *CCStoreInfo::storeAssetsToDictionary(CCStoreAssets *storeAssets) {
        CCArray *currenciesJSON = CCArray::create();
        {
            CCArray *currencies = storeAssets->getCurrencies();
            CCObject *obj;
            CCARRAY_FOREACH(currencies, obj) {
                currenciesJSON->addObject(((CCVirtualCurrency *)obj)->toDictionary());
            }
        }
        
        CCArray *packsJSON = CCArray::create();
        {
            CCArray *packs = storeAssets->getCurrencyPacks();
            CCObject *obj;
            CCARRAY_FOREACH(packs, obj) {
                packsJSON->addObject(((CCVirtualCurrencyPack *)obj)->toDictionary());
            }
        }
        
        CCArray *suGoods = CCArray::create();
        CCArray *ltGoods = CCArray::create();
        CCArray *eqGoods = CCArray::create();
        CCArray *upGoods = CCArray::create();
        CCArray *paGoods = CCArray::create();
        
        CCObject *obj;
        CCARRAY_FOREACH(storeAssets->getGoods(), obj) {
            if (dynamic_cast<CCSingleUseVG *>(obj)) {
                suGoods->addObject(((CCSingleUseVG *)obj)->toDictionary());
            } else if (dynamic_cast<CCEquippableVG *>(obj)) {
                eqGoods->addObject(((CCEquippableVG *)obj)->toDictionary());
            } else if (dynamic_cast<CCUpgradeVG *>(obj)) {
                upGoods->addObject(((CCUpgradeVG *)obj)->toDictionary());
            } else if (dynamic_cast<CCLifetimeVG *>(obj)) {
                ltGoods->addObject(((CCLifetimeVG *)obj)->toDictionary());
            } else if (dynamic_cast<CCSingleUsePackVG *>(obj)) {
                paGoods->addObject(((CCSingleUsePackVG *)obj)->toDictionary());
            }
        }
        
        CCDictionary *goodsJSON = CCDictionary::create();
        goodsJSON->setObject(suGoods, CCStoreConsts::JSON_STORE_GOODS_SU);
        goodsJSON->setObject(ltGoods, CCStoreConsts::JSON_STORE_GOODS_LT);
        goodsJSON->setObject(eqGoods, CCStoreConsts::JSON_STORE_GOODS_EQ);
        goodsJSON->setObject(upGoods, CCStoreConsts::JSON_STORE_GOODS_UP);
        goodsJSON->setObject(paGoods, CCStoreConsts::JSON_STORE_GOODS_PA);
        
        CCArray *categoriesJSON = CCArray::create();
        {
            CCArray *categories = storeAssets->getCategories();
            CCObject *obj;
            CCARRAY_FOREACH(categories, obj) {
                categoriesJSON->addObject(((CCVirtualCategory *)obj)->toDictionary());
            }
        }
        
        CCDictionary *storeAssetsObj = CCDictionary::create();
        storeAssetsObj->setObject(categoriesJSON, CCStoreConsts::JSON_STORE_CATEGORIES);
        storeAssetsObj->setObject(currenciesJSON, CCStoreConsts::JSON_STORE_CURRENCIES);
        storeAssetsObj->setObject(packsJSON, CCStoreConsts::JSON_STORE_CURRENCY_PACKS);
        storeAssetsObj->setObject(goodsJSON, CCStoreConsts::JSON_STORE_GOODS);
        
        return storeAssetsObj;
    }
    
    void CCStoreInfo::setStoreAssets(CCStoreAssets *storeAssets) {
        CCDictionary *storeDict = storeAssetsToDictionary(storeAssets);
        const char *jsonString = json_dumps(CCJsonHelper::getJsonFromCCObject(storeDict), JSON_COMPACT | JSON_ENSURE_ASCII);
        CCKeyValueStorage::getInstance()->setValue(KEY_META_STORE_INFO, jsonString);
    }
    
    void CCStoreInfo::initializeFromDB() {
        const char *val = CCKeyValueStorage::getInstance()->getValue(KEY_META_STORE_INFO);
        
        if (val == NULL){
            CCSoomlaUtils::logError(TAG, "store json is not in DB. Make sure you initialized SoomlaStore with your Store assets. The App will shut down now.");
            CC_ASSERT(false);
        }
        
        CCSoomlaUtils::logDebug(TAG, CCString::createWithFormat("the metadata-economy json (from DB) is %s", val)->getCString());
        
        json_error_t error;
        json_t* storeJson = json_loads(val, 0, &error);
        if (!storeJson) {
            CCSoomlaUtils::logError(TAG, "Unable to parse metadata-economy JSON");
            CC_ASSERT(false);
        }
        
        CCDictionary *storeDict = dynamic_cast<CCDictionary *>(CCJsonHelper::getCCObjectFromJson(storeJson));
        initWithDictionary(storeDict);
    }
    
    void CCStoreInfo::updateAggregatedLists() {
        // rewritten from android java code
        CCObject *obj;
        
        CCARRAY_FOREACH(getCurrencies(), obj) {
            CCVirtualCurrency *vi = dynamic_cast<CCVirtualCurrency *>(obj);
            CC_ASSERT(vi);
            getVirtualItems()->setObject(vi, vi->getItemId()->getCString());
        }
        
        CCARRAY_FOREACH(getCurrencyPacks(), obj) {
            CCVirtualCurrencyPack *vi = dynamic_cast<CCVirtualCurrencyPack *>(obj);
            CC_ASSERT(vi);
            getVirtualItems()->setObject(vi, vi->getItemId()->getCString());
            CCPurchaseType *purchaseType = vi->getPurchaseType();
            CCPurchaseWithMarket *purchaseWithMarket = dynamic_cast<CCPurchaseWithMarket *>(purchaseType);
            if (purchaseWithMarket != NULL) {
                getPurchasableItems()->setObject(vi, purchaseWithMarket->getMarketItem()->getProductId()->getCString());
            }
        }
        
        CCARRAY_FOREACH(getGoods(), obj) {
            CCVirtualGood *vi = dynamic_cast<CCVirtualGood *>(obj);
            CC_ASSERT(vi);
            getVirtualItems()->setObject(vi, vi->getItemId()->getCString());
            
            CCUpgradeVG *upgradeVG = dynamic_cast<CCUpgradeVG *>(vi);
            if (upgradeVG != NULL) {
                CCArray *upgrades = dynamic_cast<CCArray *>(getGoodsUpgrades()->objectForKey(upgradeVG->getGoodItemId()->getCString()));
                if (upgrades == NULL) {
                    upgrades = CCArray::create();
                    getGoodsUpgrades()->setObject(upgrades, upgradeVG->getGoodItemId()->getCString());
                }
                upgrades->addObject(upgradeVG);
            }
            CCPurchaseType *purchaseType = vi->getPurchaseType();
            CCPurchaseWithMarket *purchaseWithMarket = dynamic_cast<CCPurchaseWithMarket *>(purchaseType);
            if (purchaseWithMarket != NULL) {
                getPurchasableItems()->setObject(vi, purchaseWithMarket->getMarketItem()->getProductId()->getCString());
            }
        }
        
        CCARRAY_FOREACH(getCategories(), obj) {
            CCVirtualCategory *category = dynamic_cast<CCVirtualCategory *>(obj);
            CC_ASSERT(category);
            
            CCObject *itemIdObj;
            CCARRAY_FOREACH(category->getGoodItemIds(), itemIdObj) {
                CCString *goodItemId = dynamic_cast<CCString *>(itemIdObj);
                CC_ASSERT(goodItemId);
                getGoodsCategories()->setObject(category, goodItemId->getCString());
            }
        }
    }
    
    void CCStoreInfo::replaceVirtualItem(CCVirtualItem *virtualItem) {
        getVirtualItems()->setObject(virtualItem, virtualItem->getItemId()->getCString());
        
        CCVirtualCurrency *currency = dynamic_cast<CCVirtualCurrency *>(virtualItem);
        if (currency != NULL) {
            for (int i = 0; i < getCurrencies()->count(); ++i) {
                CCVirtualCurrency *currentCurrency = dynamic_cast<CCVirtualCurrency *>(getCurrencies()->objectAtIndex(i));
                CC_ASSERT(currentCurrency);
                if (currentCurrency->getItemId()->isEqual(currency->getItemId())) {
                    getCurrencies()->removeObjectAtIndex(i);
                    break;
                }
            }
            getCurrencies()->addObject(currency);
        }
        
        CCVirtualCurrencyPack *vcp = dynamic_cast<CCVirtualCurrencyPack *>(virtualItem);
        if (vcp != NULL) {
            CCPurchaseWithMarket *purchaseWithMarket = dynamic_cast<CCPurchaseWithMarket *>(vcp->getPurchaseType());
            if (purchaseWithMarket != NULL) {
                getPurchasableItems()->setObject(vcp, purchaseWithMarket->getMarketItem()->getProductId()->getCString());
            }
            
            for (int i = 0; i < getCurrencyPacks()->count(); ++i) {
                CCVirtualCurrencyPack *currentCurrencyPack = dynamic_cast<CCVirtualCurrencyPack *>(getCurrencyPacks()->objectAtIndex(i));
                CC_ASSERT(currentCurrencyPack);
                if (currentCurrencyPack->getItemId()->isEqual(vcp->getItemId())) {
                    getCurrencyPacks()->removeObjectAtIndex(i);
                    break;
                }
            }
            
            getCurrencyPacks()->addObject(vcp);
        }
        
        CCVirtualGood *vg = dynamic_cast<CCVirtualGood *>(virtualItem);
        if (vg != NULL) {
            CCUpgradeVG *upgradeVG = dynamic_cast<CCUpgradeVG *>(vg);
            if (upgradeVG != NULL) {
                CCArray *upgrades = dynamic_cast<CCArray *>(getGoodsUpgrades()->objectForKey(upgradeVG->getGoodItemId()->getCString()));
                if (upgrades == NULL) {
                    upgrades = CCArray::create();
                    getGoodsUpgrades()->setObject(upgrades, upgradeVG->getGoodItemId()->getCString());
                }
                upgrades->addObject(upgradeVG);
            }
            
            CCPurchaseWithMarket *purchaseWithMarket = dynamic_cast<CCPurchaseWithMarket *>(vg->getPurchaseType());
            if (purchaseWithMarket != NULL) {
                getPurchasableItems()->setObject(vg, purchaseWithMarket->getMarketItem()->getProductId()->getCString());
            }
            
            for (int i = 0; i < getGoods()->count(); ++i) {
                CCVirtualGood *currentVG = dynamic_cast<CCVirtualGood *>(getGoods()->objectAtIndex(i));
                CC_ASSERT(currentVG);
                if (currentVG->getItemId()->isEqual(vg->getItemId())) {
                    getGoods()->removeObjectAtIndex(i);
                    break;
                }
            }
            
            getGoods()->addObject(vg);
        }
    }
    
    bool CCStoreInfo::initWithDictionary(cocos2d::CCDictionary* dict) {
        if (dict == NULL) {
            CCSoomlaUtils::logError(TAG, "The given store dictionary can't be null!");
            return false;
        }
        
        setVirtualItems(CCDictionary::create());
        setPurchasableItems(CCDictionary::create());
        setGoodsCategories(CCDictionary::create());
        setGoodsUpgrades(CCDictionary::create());
        
        CCObject *ref;
        
        ref = dict->objectForKey(CCStoreConsts::JSON_STORE_CURRENCIES);
        if (ref) {
            CCArray *currenciesDictArray = dynamic_cast<CCArray *>(ref);
            CC_ASSERT(currenciesDictArray);
            CCArray *currenciesArray = CCDomainHelper::getInstance()->getDomainsFromDictArray(currenciesDictArray);
            setCurrencies(currenciesArray);
        }
        
        ref = dict->objectForKey(CCStoreConsts::JSON_STORE_CURRENCY_PACKS);
        if (ref) {
            CCArray *currencyPacksDictArray = dynamic_cast<CCArray *>(ref);
            CC_ASSERT(currencyPacksDictArray);
            CCArray *currencyPacksArray = CCDomainHelper::getInstance()->getDomainsFromDictArray(currencyPacksDictArray);
            setCurrencyPacks(currencyPacksArray);
        }
        
        setGoods(CCArray::create());
        ref = dict->objectForKey(CCStoreConsts::JSON_STORE_GOODS);
        if (ref) {
            CCDictionary *goodsDictArray = dynamic_cast<CCDictionary *>(ref);
            CC_ASSERT(goodsDictArray);
            
            ref = goodsDictArray->objectForKey(CCStoreConsts::JSON_STORE_GOODS_SU);
            if (ref) {
                CCArray *singleUseDictArray = dynamic_cast<CCArray *>(ref);
                CC_ASSERT(singleUseDictArray);
                
                CCArray *suGoods = CCDomainHelper::getInstance()->getDomainsFromDictArray(singleUseDictArray);
                CCObject *obj;
                CCARRAY_FOREACH(suGoods, obj) {
                    CCSingleUseVG *singleUseVG = dynamic_cast<CCSingleUseVG *>(obj);
                    CC_ASSERT(singleUseVG);
                    getGoods()->addObject(singleUseVG);
                }
            }
            
            ref = goodsDictArray->objectForKey(CCStoreConsts::JSON_STORE_GOODS_LT);
            if (ref) {
                CCArray *liftimeDictArray = dynamic_cast<CCArray *>(ref);
                CC_ASSERT(liftimeDictArray);
                
                CCArray *ltGoods = CCDomainHelper::getInstance()->getDomainsFromDictArray(liftimeDictArray);
                CCObject *obj;
                CCARRAY_FOREACH(ltGoods, obj) {
                    CCLifetimeVG *lifetimeVG = dynamic_cast<CCLifetimeVG *>(obj);
                    CC_ASSERT(lifetimeVG);
                    getGoods()->addObject(lifetimeVG);
                }
            }
            
            ref = goodsDictArray->objectForKey(CCStoreConsts::JSON_STORE_GOODS_EQ);
            if (ref) {
                CCArray *equippableDictArray = dynamic_cast<CCArray *>(ref);
                CC_ASSERT(equippableDictArray);
                
                CCArray *eqGoods = CCDomainHelper::getInstance()->getDomainsFromDictArray(equippableDictArray);
                CCObject *obj;
                CCARRAY_FOREACH(eqGoods, obj) {
                    CCEquippableVG *equippableVG = dynamic_cast<CCEquippableVG *>(obj);
                    CC_ASSERT(equippableVG);
                    getGoods()->addObject(equippableVG);
                }
            }
            
            ref = goodsDictArray->objectForKey(CCStoreConsts::JSON_STORE_GOODS_PA);
            if (ref) {
                CCArray *singleUsePackDictArray = dynamic_cast<CCArray *>(ref);
                CC_ASSERT(singleUsePackDictArray);
                
                CCArray *paGoods = CCDomainHelper::getInstance()->getDomainsFromDictArray(singleUsePackDictArray);
                CCObject *obj;
                CCARRAY_FOREACH(paGoods, obj) {
                    CCSingleUsePackVG *singleUsePackVG = dynamic_cast<CCSingleUsePackVG *>(obj);
                    CC_ASSERT(singleUsePackVG);
                    getGoods()->addObject(singleUsePackVG);
                }
            }
            
            ref = goodsDictArray->objectForKey(CCStoreConsts::JSON_STORE_GOODS_UP);
            if (ref) {
                CCArray *upgradeDictArray = dynamic_cast<CCArray *>(ref);
                CC_ASSERT(upgradeDictArray);
                
                CCArray *upGoods = CCDomainHelper::getInstance()->getDomainsFromDictArray(upgradeDictArray);
                CCObject *obj;
                CCARRAY_FOREACH(upGoods, obj) {
                    CCUpgradeVG *upgradeVG = dynamic_cast<CCUpgradeVG *>(obj);
                    CC_ASSERT(upgradeVG);
                    getGoods()->addObject(upgradeVG);
                }
            }
        }
        
        ref = dict->objectForKey(CCStoreConsts::JSON_STORE_CATEGORIES);
        if (ref) {
            CCArray *categoriesDictArray = dynamic_cast<CCArray *>(ref);
            CC_ASSERT(categoriesDictArray);
            CCArray *categoriesArray = CCDomainHelper::getInstance()->getDomainsFromDictArray(categoriesDictArray);
            setCategories(categoriesArray);
        }
        
        updateAggregatedLists();
        
        return true;
    }
    
    CCDictionary* CCStoreInfo::toDictionary() {
        
        CCArray *currenciesJSON = CCArray::create();
        {
            CCArray *currencies = getCurrencies();
            CCObject *obj;
            CCARRAY_FOREACH(currencies, obj) {
                currenciesJSON->addObject(((CCVirtualCurrency *)obj)->toDictionary());
            }
        }
        
        CCArray *packsJSON = CCArray::create();
        {
            CCArray *packs = getCurrencyPacks();
            CCObject *obj;
            CCARRAY_FOREACH(packs, obj) {
                packsJSON->addObject(((CCVirtualCurrencyPack *)obj)->toDictionary());
            }
        }
        
        CCArray *suGoods = CCArray::create();
        CCArray *ltGoods = CCArray::create();
        CCArray *eqGoods = CCArray::create();
        CCArray *upGoods = CCArray::create();
        CCArray *paGoods = CCArray::create();
        
        CCObject *obj;
        CCARRAY_FOREACH(getGoods(), obj) {
            if (dynamic_cast<CCSingleUseVG *>(obj)) {
                suGoods->addObject(((CCSingleUseVG *)obj)->toDictionary());
            } else if (dynamic_cast<CCEquippableVG *>(obj)) {
                eqGoods->addObject(((CCEquippableVG *)obj)->toDictionary());
            } else if (dynamic_cast<CCUpgradeVG *>(obj)) {
                upGoods->addObject(((CCUpgradeVG *)obj)->toDictionary());
            } else if (dynamic_cast<CCLifetimeVG *>(obj)) {
                ltGoods->addObject(((CCLifetimeVG *)obj)->toDictionary());
            } else if (dynamic_cast<CCSingleUsePackVG *>(obj)) {
                paGoods->addObject(((CCSingleUsePackVG *)obj)->toDictionary());
            }
        }
        
        CCDictionary *goodsJSON = CCDictionary::create();
        goodsJSON->setObject(suGoods, CCStoreConsts::JSON_STORE_GOODS_SU);
        goodsJSON->setObject(ltGoods, CCStoreConsts::JSON_STORE_GOODS_LT);
        goodsJSON->setObject(eqGoods, CCStoreConsts::JSON_STORE_GOODS_EQ);
        goodsJSON->setObject(upGoods, CCStoreConsts::JSON_STORE_GOODS_UP);
        goodsJSON->setObject(paGoods, CCStoreConsts::JSON_STORE_GOODS_PA);
        
        CCArray *categoriesJSON = CCArray::create();
        {
            CCArray *categories = getCategories();
            CCObject *obj;
            CCARRAY_FOREACH(categories, obj) {
                categoriesJSON->addObject(((CCVirtualCategory *)obj)->toDictionary());
            }
        }
        
        CCDictionary *storeAssetsObj = CCDictionary::create();
        storeAssetsObj->setObject(categoriesJSON, CCStoreConsts::JSON_STORE_CATEGORIES);
        storeAssetsObj->setObject(currenciesJSON, CCStoreConsts::JSON_STORE_CURRENCIES);
        storeAssetsObj->setObject(packsJSON, CCStoreConsts::JSON_STORE_CURRENCY_PACKS);
        storeAssetsObj->setObject(goodsJSON, CCStoreConsts::JSON_STORE_GOODS);
        
        return storeAssetsObj;
    }
    
    void CCStoreInfo::handle__REFLECTION_INITIALIZE_FROM_DB(cocos2d::CCDictionary *paramaters) {
        this->initializeFromDB();
    }
}
