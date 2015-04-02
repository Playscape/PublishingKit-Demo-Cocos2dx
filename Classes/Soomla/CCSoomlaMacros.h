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

#ifndef __CCSoomlaMacros_h
#define __CCSoomlaMacros_h

#include "cocos2d.h"

#define SL_SYNTHESIZE_RETAIN_WITH_DICT(varType, varName, funName, jsonKey)    \
CC_SYNTHESIZE_RETAIN(varType, varName, funName); \
protected: inline void fill##funName##FromDict(cocos2d::CCDictionary* dict) \
{ \
    cocos2d::CCObject* obj = dict->objectForKey(jsonKey); \
    CCAssert(obj == NULL || dynamic_cast<varType>(obj), "invalid object type in dictionary"); \
    if (varName != obj) \
    { \
        set##funName((varType)obj); \
    } \
} \
protected: inline void put##funName##ToDict(cocos2d::CCDictionary* dict) { \
  if (varName) { \
    dict->setObject(varName, jsonKey); \
  } \
}

#define SL_SYNTHESIZE_DOUBLE_RETAIN_WITH_DICT(varName, funName, jsonKey) \
CC_SYNTHESIZE_RETAIN(cocos2d::CCDouble *, varName, funName); \
protected: inline void fill##funName##FromDict(cocos2d::CCDictionary* dict) \
{ \
    cocos2d::CCDouble *val = NULL; \
    cocos2d::CCObject* obj = dict->objectForKey(jsonKey); \
    if (obj != NULL) { \
        val = dynamic_cast<cocos2d::CCDouble *>(obj); \
        if (val == NULL) { \
            CCAssert(dynamic_cast<cocos2d::CCInteger *>(obj), "invalid object type in dictionary"); \
            val = cocos2d::CCDouble::create(((cocos2d::CCInteger *)obj)->getValue()); \
        }\
    } \
    if (varName != obj) \
    { \
        set##funName((cocos2d::CCDouble *)obj); \
    } \
} \
protected: inline void put##funName##ToDict(cocos2d::CCDictionary* dict) { \
  if (varName) { \
    dict->setObject(varName, jsonKey); \
  } \
}

#define SL_SYNTHESIZE_RETAIN_WITH_DICT_DCL(varType, varName, funName)    \
CC_SYNTHESIZE_RETAIN(varType, varName, funName); \
protected: inline void fill##funName##FromDict(cocos2d::CCDictionary* dict); \
protected: inline void put##funName##ToDict(cocos2d::CCDictionary* dict); \

#define SL_CREATE_WITH_DICTIONARY(__class) \
static __class *createWithDictionary(cocos2d::CCDictionary *dict) { \
    __class *ret = new __class(); \
    if (ret->initWithDictionary(dict)) { \
        ret->autorelease(); \
    } \
    else { \
        CC_SAFE_DELETE(ret); \
    } \
    return ret; \
} \

#define SL_SAFE_CREATE_FROM_RETURN(__T__, __ret__, __retParams__)			\
    SL_SAFE_CREATE(__T__, __ret__, (__retParams__->objectForKey("return")))

#define SL_SAFE_CREATE(__T__, __ret__, __ref__)			\
    __T__ __ret__ = NULL;\
    {\
        CCDictionary *dict = dynamic_cast<CCDictionary *>(__ref__); \
        CC_ASSERT(dict); \
        soomla::CCDomain *domain = CCDomainFactory::getInstance()->createWithDictionary(dict); \
	    __ret__ = dynamic_cast<__T__>(domain);			\
	    CC_ASSERT(__ret__); \
    }\

#define SL_EXTRACT_FROM_RETURN(__T__, __ret__, __retParams__) \
__T__ *__ret__ = NULL; \
{ \
  CCObject *retRef = __retParams__->objectForKey("return"); \
  CC_ASSERT(retRef); \
  __ret__ = dynamic_cast<__T__ *>(retRef); \
  CC_ASSERT(__ret__); \
} \

#define SL_EXTRACT_DOUBLE_FROM_RETURN(__ret__, __retParams__) \
CCDouble *__ret__ = NULL; \
{ \
  CCObject *retRef = __retParams__->objectForKey("return"); \
  CC_ASSERT(retRef); \
  __ret__ = dynamic_cast<CCDouble *>(retRef); \
  if (__ret__ == NULL) { \
    CCInteger *intRef = dynamic_cast<CCInteger *>(retRef); \
    if (intRef != NULL) { \
        __ret__ = CCDouble::create(intRef->getValue());\
    }\
  } \
  CC_ASSERT(__ret__); \
} \

#define SL_CREATE_PARAMS_FOR_METHOD(__ret__, __methodName__) \
CCDictionary *__ret__ = CCDictionary::create(); \
params->setObject(CCString::create(__methodName__), "method");


#endif // __CCSoomlaMacros_h