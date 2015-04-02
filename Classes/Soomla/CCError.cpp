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

#include "CCError.h"
#include "CCSoomlaUtils.h"

USING_NS_CC;

namespace soomla {
    #define CANNOT_EXTRACT "Cannot extract error data"
    #define JSON_ERROR_INFO "errorInfo"

    #define TAG "CCError"

    CCError *CCError::createWithObject(cocos2d::CCObject *obj) {
        if (obj == NULL) {
            return NULL;
        }
        
        CCString *errorInfoStr = dynamic_cast<CCString *>(obj);
        if (errorInfoStr == NULL) {
            CCDictionary *dict = static_cast<CCDictionary *>(obj);
            if (dict->objectForKey(JSON_ERROR_INFO) == NULL) {
                return NULL;
            }
            
            errorInfoStr = dynamic_cast<CCString *>(dict->objectForKey(JSON_ERROR_INFO));
        }
        
        std::string errorInfo;
        if (errorInfoStr) {
            errorInfo = errorInfoStr->getCString();
        } else {
            errorInfo = CANNOT_EXTRACT;
        }

        CCError *ret = new CCError();
        if (ret->init(errorInfo)) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    void CCError::tryFillError(CCError **error, cocos2d::CCObject *obj, const char *tag) {
        if (error != NULL) {
            CCError *resultError = CCError::createWithObject(obj);
            if (resultError != NULL) {
                *error = resultError;
            }
        }
        else {
            CCString *errorStr = dynamic_cast<CCString *>(obj);
            if (errorStr != NULL) {
                if (tag == NULL) {
                    tag = TAG;
                }
                CCSoomlaUtils::logError(tag, errorStr->getCString());
            }
        }
    }

    bool CCError::init(std::string &errorInfo) {
        mInfo = errorInfo;

        return true;
    }
}