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

#include "CCDomainHelper.h"

#include "CCCoreConsts.h"
#include "CCDomainFactory.h"

using namespace soomla;
using namespace cocos2d;

static CCDomainHelper *s_SharedDomainFactory = NULL;

CCDomainHelper *CCDomainHelper::getInstance() {
    if (!s_SharedDomainFactory) {
        s_SharedDomainFactory = new CCDomainHelper();
    }

    return s_SharedDomainFactory;
}

cocos2d::CCArray *soomla::CCDomainHelper::getDomainsFromDictArray(cocos2d::CCArray *dictArray, const char *type) {
    if (dictArray == NULL) {
        return NULL;
    }

    cocos2d::CCArray *domainArray = cocos2d::CCArray::createWithCapacity(dictArray->count());
    cocos2d::CCObject *obj;
    cocos2d::CCDictionary *dict;
    CCARRAY_FOREACH(dictArray, obj) {
            dict = dynamic_cast<cocos2d::CCDictionary *>(obj);
            CC_ASSERT(obj);
            CCDomain *domain;
            if (type == NULL){
                domain = soomla::CCDomainFactory::getInstance()->createWithDictionary(dict);
            }
            else {
                domain = soomla::CCDomainFactory::getInstance()->createWithDictionaryAndType(dict, type);
            }
            domainArray->addObject(domain);
        }
    return domainArray;
}

cocos2d::CCArray *soomla::CCDomainHelper::getDictArrayFromDomains(cocos2d::CCArray *domainArray) {
    if (domainArray == NULL) {
        return NULL;
    }

    cocos2d::CCArray *dictArray = cocos2d::CCArray::createWithCapacity(domainArray->count());
    cocos2d::CCObject *obj;
    soomla::CCDomain *domain;
    CCARRAY_FOREACH(domainArray, obj) {
            domain = dynamic_cast<soomla::CCDomain *>(obj);
            CC_ASSERT(obj);
            dictArray->addObject(domain->toDictionary());
        }
    return dictArray;
}
