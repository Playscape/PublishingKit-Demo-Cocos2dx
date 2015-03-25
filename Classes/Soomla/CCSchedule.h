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

#ifndef __CCSchedule_H_
#define __CCSchedule_H_

#include "CCDomain.h"
#include "CCSoomlaMacros.h"
#include "CCCoreConsts.h"

namespace soomla {
    class CCSchedule : public CCDomain {
    public:
        enum Recurrence {
            EVERY_MONTH,
            EVERY_WEEK,
            EVERY_DAY,
            EVERY_HOUR,
            NONE
        };
        SL_SYNTHESIZE_RETAIN_WITH_DICT(cocos2d::CCInteger*, mRequiredRecurrence, RequiredRecurrence, CCCoreConsts::JSON_SCHE_REC);
        SL_SYNTHESIZE_RETAIN_WITH_DICT(cocos2d::CCArray*, mTimeRanges, TimeRanges, CCCoreConsts::JSON_SCHE_RANGES);
        SL_SYNTHESIZE_RETAIN_WITH_DICT(cocos2d::CCInteger*, mActivationLimit, ActivationLimit, CCCoreConsts::JSON_SCHE_APPROVALS);
    public:
        class CCDateTimeRange: public CCDomain {
        public:
            timeval mStart;
            timeval mEnd;

            static CCDateTimeRange *create(timeval start, timeval end);
            SL_CREATE_WITH_DICTIONARY(CCDateTimeRange);
            virtual bool init(timeval start, timeval end);
            virtual bool initWithDictionary(cocos2d::CCDictionary* dict);
            virtual ~CCDateTimeRange();
            virtual cocos2d::CCDictionary* toDictionary();
        };

        CCSchedule(): CCDomain(), mRequiredRecurrence(NULL), mTimeRanges(NULL), mActivationLimit(NULL) {}

        static CCSchedule *create(cocos2d::CCInteger *requiredRecurrence, cocos2d::CCArray *timeRanges, cocos2d::CCInteger *activationLimit);
        static CCSchedule *create(cocos2d::CCInteger *activationLimit);
        static CCSchedule *create(cocos2d::CCInteger *requiredRecurrence, timeval start, timeval end, cocos2d::CCInteger *activationLimit);

        static CCSchedule *createAnyTimeOnce();
        static CCSchedule *createAnyTimeLimited(int activationLimit);
        static CCSchedule *createAnyTimeUnLimited();

        SL_CREATE_WITH_DICTIONARY(CCSchedule);

        virtual bool init(cocos2d::CCInteger *requiredRecurrence, cocos2d::CCArray *timeRanges, cocos2d::CCInteger *activationLimit);
        virtual bool initWithDictionary(cocos2d::CCDictionary* dict);

        virtual ~CCSchedule();

        virtual cocos2d::CCDictionary* toDictionary();

        bool approve(int activationTimes);
    };
}

#endif //__CCSchedule_H_
