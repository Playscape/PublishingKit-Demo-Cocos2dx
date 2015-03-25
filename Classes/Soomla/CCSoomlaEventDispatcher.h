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

#ifndef __CCEventDispatcher_H_
#define __CCEventDispatcher_H_

#include "cocos2d.h"
#include "CCSoomlaMacros.h"

namespace soomla {

    typedef void (cocos2d::CCObject::*SEL_EventHandler)(cocos2d::CCDictionary*);

    struct StructEventHandler {
        StructEventHandler(cocos2d::CCObject *target, SEL_EventHandler selector)
                : target(target), selector(selector) {
        }

        cocos2d::CCObject *target;
        SEL_EventHandler selector;
    };

    class CCSoomlaEventDispatcher : public cocos2d::CCObject {
    private:
        std::map<std::string, StructEventHandler *> mEventHandlers;
    public:
        static CCSoomlaEventDispatcher *getInstance();

        void ndkCallback(cocos2d::CCDictionary *parameters);

        void registerEventHandler(const char *key, cocos2d::CCObject *target, SEL_EventHandler selector);

        void unregisterEventHandler(const char *key);

        virtual ~CCSoomlaEventDispatcher();
    };
};

#endif //__CCEventDispatcher_H_
