#include "Default.h"

#pragma once

Scene* IntroScene::createScene()
{
	auto scene = Scene::create();

	auto layer = IntroScene::create();

	scene->addChild(layer);

	return scene;
}


bool IntroScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//터치리스너
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(IntroScene::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_TIZEN) && !defined(CC_PLATFORM_OS_TVOS)

	CSoundManager::getInstance()->stopAllBgms();

	_videoPlayer = experimental::ui::VideoPlayer::create();
	_videoPlayer->setPosition(Vec2(640, 360));
	_videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_videoPlayer->setContentSize(Size(1280, 720));
	this->addChild(_videoPlayer, 3);

	extern ObjectHero* Hero;

	_videoPlayer->addEventListener(CC_CALLBACK_2(IntroScene::videoEventCallback, this));
	if (Hero->StageClear != true)
	{
		_videoPlayer->setFileName("OP.mp4");
		_videoPlayer->play();
	}
	else
	{
		_videoPlayer->setFileName("ED.mp4");
		_videoPlayer->play();
	}

	/*auto menu = Menu::create(SkipItem, nullptr);
	menu->setPosition(Vec2(640,200));
	this->addChild(menu,3);*/

#endif



	return true;
}

void IntroScene::onTouchBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
	//방어코드 추가 필요 bool 형 추가해서 한번만 실행하게
	////다음메뉴로넘어가기
	Scene* s = TransitionFade::create(1.0f, StageSelect::createScene());
	Director::getInstance()->replaceScene(s);
}