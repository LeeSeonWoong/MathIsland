#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "Default.h"

class LoadingScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LoadingScene);

	void loadingCallBack(Texture2D *texture);
	Label *_labelLoading;
	Label *_labelPercent;
	int _numberOfSprites;
	int _numberOfLoadedSprites;

	void ChangeScene();

	void onTouchBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

};

#endif 