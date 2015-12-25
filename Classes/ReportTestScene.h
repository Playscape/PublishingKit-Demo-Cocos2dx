#ifndef __ReportTestScene_h__
#define __ReportTestScene_h__

#include <iostream>
#include <string>
#include "cocos2d.h"

using namespace cocos2d;
using std::string;

class ReportTestScene : public CCLayerColor
{
protected:
		void showInGameMenuLayer();

		// Ingame menu Callbacks
		void menuButtonCallback(CCObject* sender);
		void reportEventClicked(CCObject* sender);
		void reportAttrEventClicked(CCObject* sender);

		CCMenu* mMainMenu;
	
public:
		// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
		virtual bool init();

		// there's no 'id' in cpp, so we recommend to return the class instance pointer
		static cocos2d::CCScene* scene();

		// preprocessor macro for "static create()" constructor ( node() deprecated )
		CREATE_FUNC(ReportTestScene);
};


#endif /* defined(__ReportTestScene_h__) */