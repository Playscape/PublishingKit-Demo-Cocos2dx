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

#ifndef __CCDomain_H_
#define __CCDomain_H_

#include "cocos2d.h"

namespace soomla {
    class CCDomain: public cocos2d::CCObject {
    public:
        virtual bool initWithDictionary(cocos2d::CCDictionary *dict) = 0;
        virtual cocos2d::CCDictionary *toDictionary() = 0;
        virtual ~CCDomain() {};
    protected:
        cocos2d::CCArray *getDomainsFromDictArray(cocos2d::CCArray *dictArray);
        cocos2d::CCArray *getDictArrayFromDomains(cocos2d::CCArray *domainArray);
        cocos2d::CCDictionary *putTypeData(cocos2d::CCDictionary *dict, const char* type);
    };
}

#endif //__CCDomain_H_
