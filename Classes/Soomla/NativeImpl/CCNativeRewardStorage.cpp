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

#include "CCNativeRewardStorage.h"
#include "CCError.h"
#include "CCNdkBridge.h"
#include "CCSoomlaUtils.h"

namespace soomla {
    
    #define TAG "SOOMLA NativeRewardStorage"
    
    USING_NS_CC;

    void CCNativeRewardStorage::setRewardStatus(CCReward *reward, bool give, bool notify) {
        CCError *error = NULL;
        
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCNativeRewardStorage::setRewardStatus"), "method");
        params->setObject(reward->getId(), "reward");
        params->setObject(CCBool::create(give), "give");
        params->setObject(CCBool::create(notify), "notify");
        CCNdkBridge::callNative (params, &error);
        
        if (error) {
            CCSoomlaUtils::logException(TAG, error);
            CC_ASSERT(false);
        }
    }

    int CCNativeRewardStorage::getTimesGiven(CCReward *reward) {
        CCError *error = NULL;
        
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCNativeRewardStorage::getTimesGiven"), "method");
        params->setObject(reward->getId(), "reward");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, &error);
        
        if (error) {
            CCSoomlaUtils::logException(TAG, error);
            CC_ASSERT(false);
            return -1;
        }
        
        if (retParams == NULL) {
            return -1;
        }
        
        CCInteger *retValue = (CCInteger *) retParams->objectForKey("return");
        if (!retValue) {
            return -1;
        }
        
        return retValue->getValue();
    }

    int CCNativeRewardStorage::getLastSeqIdxGiven(CCSequenceReward *sequenceReward) {
        CCError *error = NULL;
        
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCNativeRewardStorage::getLastSeqIdxGiven"), "method");
        params->setObject(sequenceReward->getId(), "reward");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, &error);
        
        if (error) {
            CCSoomlaUtils::logException(TAG, error);
            CC_ASSERT(false);
            return -1;
        }
        
        if (retParams == NULL) {
            return -1;
        }
        
        CCInteger *retValue = (CCInteger *) retParams->objectForKey("return");
        if (!retValue) {
            return -1;
        }
        
        return retValue->getValue();
    }

    void CCNativeRewardStorage::setLastSeqIdxGiven(CCSequenceReward *sequenceReward, unsigned int idx) {
        CCError *error = NULL;
        
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCNativeRewardStorage::setLastSeqIdxGiven"), "method");
        params->setObject(sequenceReward->getId(), "reward");
        params->setObject(CCInteger::create(idx), "idx");
        CCNdkBridge::callNative (params, &error);
        
        if (error) {
            CCSoomlaUtils::logException(TAG, error);
            CC_ASSERT(false);
        }
    }
}
