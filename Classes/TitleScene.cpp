#include "Default.h"

#pragma once

Scene* TitleScene::createScene()
{
	auto scene = Scene::create();

	auto layer = TitleScene::create();

	scene->addChild(layer);

	return scene;
}


bool TitleScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	CSoundManager::getInstance()->playBgm("Sound/Dog_and_Pony_Show.mp3", true, false, 0.3f);


	Size winSize = Director::getInstance()->getWinSize();//화면출력 크기사이즈값을 얻어옴
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//위와비슷한기능

	//시작화면
	auto sprite = Sprite::create("BackGround/title3.png");//배경스프라이트 저장
	sprite->setPosition(Vec2(winSize.width *0.5 + origin.x, winSize.height * 0.5 + origin.y));
	this->addChild(sprite, 0);

	//시작화면
	auto sprite2 = Sprite::create("BackGround/Title.png");//배경스프라이트 저장
	sprite2->setPosition(Vec2(winSize.width *0.5 + origin.x, winSize.height * 0.5 + origin.y));
	this->addChild(sprite2, 1);

	//시작화면
	auto sprite3 = Sprite::create("BackGround/Touch_Screen.png");//배경스프라이트 저장
	sprite3->setPosition(Vec2(winSize.width *0.5 + origin.x, winSize.height * 0.5 + origin.y - 300));
	sprite3->runAction(RepeatForever::create(
		Sequence::create(
			FadeOut::create(1.0f),
			FadeIn::create(1.0f), NULL
		)
	));
	this->addChild(sprite3, 2);

	//터치리스너
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(TitleScene::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void TitleScene::onTouchBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
	//방어코드 추가 필요 bool 형 추가해서 한번만 실행하게
	////다음메뉴로넘어가기
	Scene* s = TransitionFade::create(1.0f, IntroScene::createScene());
	Director::getInstance()->replaceScene(s);
}