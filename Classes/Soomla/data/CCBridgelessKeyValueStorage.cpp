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

#include "support/user_default/CCUserDefault.h"

#include "CCBridgelessKeyValueStorage.h"

namespace soomla {
    
    USING_NS_CC;
    
    #define KEY_VALUE_STORAGE_KEY "soomla.kvs.keys"
    
    CCBridgelessKeyValueStorage::CCBridgelessKeyValueStorage() {
        mStoredKeys = CCSet::create();
        mStoredKeys->retain();
        
        loadStoredKeys();
    }
    
    const char *CCBridgelessKeyValueStorage::getValue(const char *key) const {
        std::string defaultValue = "";
        std::string result = CCUserDefault::sharedUserDefault()->getStringForKey(key, defaultValue);
        if (result == defaultValue) {
            return NULL;
        }
        
        // Using this for pooling facilities, so c_str won't go away when returned
        CCString *returnedValue = CCString::create(result);
        return returnedValue->getCString();
    }
    
    void CCBridgelessKeyValueStorage::setValue(const char *key, const char *val) {
        CCUserDefault::sharedUserDefault()->setStringForKey(key, val);
        CCUserDefault::sharedUserDefault()->flush();
        
        addStoredKeys(key);
        saveStoredKeys();
    }
    
    void CCBridgelessKeyValueStorage::deleteKeyValue(const char *key) {
        CCUserDefault::sharedUserDefault()->setStringForKey(key, "");
        CCUserDefault::sharedUserDefault()->flush();
        
        removeStoredKeys(key);
        saveStoredKeys();
    }
    
    void CCBridgelessKeyValueStorage::purge() {
        for(CCSetIterator i = mStoredKeys->begin(); i != mStoredKeys->end(); i++) {
            if (!(*i)) {
                break;
            }
            
            CCString *key = dynamic_cast<CCString *>(*i);
            deleteKeyValue(key->getCString());
        }
        
        mStoredKeys->removeAllObjects();
        
        CCUserDefault::sharedUserDefault()->setStringForKey(KEY_VALUE_STORAGE_KEY, "");
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    void CCBridgelessKeyValueStorage::addStoredKeys(const char *key) {
        CCString *wrapKey = CCString::create(key);
        
        if (mStoredKeys->containsObject(wrapKey)) {
            return;
        }
        
        mStoredKeys->addObject(wrapKey);
    }
    
    void CCBridgelessKeyValueStorage::removeStoredKeys(const char *key) {
        CCString *wrapKey = CCString::create(key);
        
        mStoredKeys->removeObject(wrapKey);
    }
    
    void CCBridgelessKeyValueStorage::saveStoredKeys() {
        std::string joinedKeys = "";
        for(CCSetIterator i = mStoredKeys->begin(); i != mStoredKeys->end(); i++) {
            if (!(*i)) {
                break;
            }
            
            CCString *key = dynamic_cast<CCString *>(*i);
            joinedKeys.append("#").append(key->getCString());
        }
        
        CCUserDefault::sharedUserDefault()->setStringForKey(KEY_VALUE_STORAGE_KEY, joinedKeys);
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    void CCBridgelessKeyValueStorage::loadStoredKeys() {
        std::string joinedKeys = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_VALUE_STORAGE_KEY, "");
        
        std::stringstream ss(joinedKeys);
        std::string item;
        while(std::getline(ss, item, '#')) {
            addStoredKeys(item.c_str());
        }
    }
}
