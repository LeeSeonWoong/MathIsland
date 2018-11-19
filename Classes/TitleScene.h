#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "Default.h"

class TitleScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(TitleScene);

	void onTouchBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

};


#endif 
