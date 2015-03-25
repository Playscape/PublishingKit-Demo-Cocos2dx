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

#include "CCSoomlaEventDispatcher.h"

#ifdef COCOS2D_JAVASCRIPT
#include "JSBinding.h"
#endif

namespace soomla {

    USING_NS_CC;

    static CCSoomlaEventDispatcher *s_SharedInstance = NULL;

    CCSoomlaEventDispatcher *CCSoomlaEventDispatcher::getInstance() {
        if (!s_SharedInstance) {
            s_SharedInstance = new CCSoomlaEventDispatcher();
        }

        return s_SharedInstance;
    }

	void CCSoomlaEventDispatcher::registerEventHandler(const char *key, cocos2d::CCObject *target, SEL_EventHandler selector) {
		mEventHandlers[key] = new StructEventHandler(target, selector);
	}

    void CCSoomlaEventDispatcher::unregisterEventHandler(const char *key) {
        if (mEventHandlers.find(key) != mEventHandlers.end()) {
            StructEventHandler *handler = mEventHandlers[key];
            mEventHandlers.erase(key);
            delete handler;
        }
    }

    void CCSoomlaEventDispatcher::ndkCallback(CCDictionary *parameters) {
#ifdef COCOS2D_JAVASCRIPT
        Soomla::JSBinding::ndkCallback(parameters);
#else
        CCString *eventName = dynamic_cast<CCString *>(parameters->objectForKey("method"));
        if (eventName == NULL) {
            // Suppress any events without callbacks (push event probably)
            return;
        }
        
        if (mEventHandlers.find(eventName->getCString()) != mEventHandlers.end()) {
            StructEventHandler *handler = mEventHandlers[eventName->getCString()];
            ((handler->target)->*(handler->selector))(parameters);
        }
        else {
            CCLog("Unregistered event happened: %s", eventName->getCString());
        }
#endif
    }

    CCSoomlaEventDispatcher::~CCSoomlaEventDispatcher() {
        std::map<std::string, StructEventHandler *>::iterator iter;
        for (iter = mEventHandlers.begin(); iter != mEventHandlers.end(); ++iter) {
            this->unregisterEventHandler(iter->first.c_str());
        }
    }
}
