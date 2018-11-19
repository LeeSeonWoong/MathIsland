#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "Default.h"
#include "ui/UIVideoPlayer.h"
#include "StageSelect.h"

class IntroScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(IntroScene);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_TIZEN) && !defined(CC_PLATFORM_OS_TVOS)
	Label* _videoStateLabel;
	experimental::ui::VideoPlayer* _videoPlayer;

	void menuSkipCallback(Ref* sender)
	{
		Scene* s = TransitionFade::create(1.0f, StageSelect::createScene());
		Director::getInstance()->replaceScene(s);
	}

	void menuStopCallback(Ref* sender)
	{
		_videoPlayer->play();
	}

	void ChangeScene()
	{
		Scene* s = TransitionFade::create(1.0f, StageSelect::createScene());
		Director::getInstance()->replaceScene(s);
	}

	void videoEventCallback(cocos2d::Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType)
	{
		switch (eventType) 
		{
		case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
			break;
		case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
			_videoPlayer->stop();
			//_videoStateLabel->setString("PAUSED");
			ChangeScene();
			break;
		case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
			break;
		case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
			ChangeScene();
			break;
		default:
			break;
		}
	}
#endif

	void onTouchBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

};


#endif 
