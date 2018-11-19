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


	Size winSize = Director::getInstance()->getWinSize();//ȭ����� ũ�������� ����
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//���ͺ���ѱ��

	//����ȭ��
	auto sprite = Sprite::create("BackGround/title3.png");//��潺������Ʈ ����
	sprite->setPosition(Vec2(winSize.width *0.5 + origin.x, winSize.height * 0.5 + origin.y));
	this->addChild(sprite, 0);

	//����ȭ��
	auto sprite2 = Sprite::create("BackGround/Title.png");//��潺������Ʈ ����
	sprite2->setPosition(Vec2(winSize.width *0.5 + origin.x, winSize.height * 0.5 + origin.y));
	this->addChild(sprite2, 1);

	//����ȭ��
	auto sprite3 = Sprite::create("BackGround/Touch_Screen.png");//��潺������Ʈ ����
	sprite3->setPosition(Vec2(winSize.width *0.5 + origin.x, winSize.height * 0.5 + origin.y - 300));
	sprite3->runAction(RepeatForever::create(
		Sequence::create(
			FadeOut::create(1.0f),
			FadeIn::create(1.0f), NULL
		)
	));
	this->addChild(sprite3, 2);

	//��ġ������
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(TitleScene::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void TitleScene::onTouchBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
	//����ڵ� �߰� �ʿ� bool �� �߰��ؼ� �ѹ��� �����ϰ�
	////�����޴��γѾ��
	Scene* s = TransitionFade::create(1.0f, IntroScene::createScene());
	Director::getInstance()->replaceScene(s);
}