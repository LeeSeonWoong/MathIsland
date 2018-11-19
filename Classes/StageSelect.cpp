#include "Default.h"

#pragma once

Scene* StageSelect::createScene()
{
	auto scene = Scene::create();

	auto layer = StageSelect::create();

	scene->addChild(layer);

	return scene;
}

bool StageSelect::init()
{
	if (!Layer::init())
	{
		return false;
	}
	CSoundManager::getInstance()->playBgm("Sound/Dog_and_Pony_Show.mp3", true, false, 0.3f);

	Size winSize = Director::getInstance()->getWinSize();//화면출력 크기사이즈값을 얻어옴
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//위와비슷한기능

	UserDefault::getInstance()->setIntegerForKey("Save5", 0);

	auto shop = Sprite::create("UI/shop.png");
	shop->setName("SHOP");
	shop->setPosition(Vec2(winSize.width*0.5f + 540, winSize.height*0.5f + 200));
	this->addChild(shop, 3);

	auto option = Sprite::create("UI/Option.png");
	option->setName("OPTION");
	option->setPosition(Vec2(winSize.width*0.5f + 540, winSize.height*0.5f + 50));
	this->addChild(option, 3);

	auto select = Sprite::create("UI/StageSelect.png");
	select->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	this->addChild(select, 1);

	auto stage = Sprite::create("UI/Stage_BT.png");
	stage->setName("STAGE1");
	stage->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	this->addChild(stage, 2);

	auto chapter1 = Sprite::create("UI/chapter_1.png");
	chapter1->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f - 230));
	this->addChild(chapter1, 2);

	auto Coin = Sprite::create("UI/CoinUI.png");
	Coin->setPosition(Vec2(winSize.width*0.5f -450, winSize.height*0.5f +270));
	this->addChild(Coin, 2);

	auto Star = Sprite::create("UI/StarUI.png");
	Star->setPosition(Vec2(winSize.width*0.5f - 200, winSize.height*0.5f + 270));
	this->addChild(Star, 2);

	auto coinlabel = Label::createWithTTF("", "fonts/Abduction.ttf", 30.0f);
	coinlabel->setPosition(Vec2(winSize.width*0.5f - 420, winSize.height*0.5f + 270));
	coinlabel->setTextColor(Color4B(255, 255, 255, 255));
	coinlabel->setName("GetCoin");
	this->addChild(coinlabel, 3);

	int cnum = UserDefault::getInstance()->getIntegerForKey("HeroCoin", 0);
	coinlabel->setString(StringUtils::format("%d", cnum));

	//터치리스너
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create(); // 싱글터치용
	listener->onTouchBegan = CC_CALLBACK_2(StageSelect::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool StageSelect::onTouchBegan(Touch* touch, Event* unused_event)
{
	//방어코드 추가 필요 bool 형 추가해서 한번만 실행하게
	////다음메뉴로넘어가기
	auto stage = dynamic_cast<Sprite*>(this->getChildByName("STAGE1"));
	auto shop = dynamic_cast<Sprite*>(this->getChildByName("SHOP"));
	auto option = dynamic_cast<Sprite*>(this->getChildByName("OPTION"));


	if (stage->getBoundingBox().containsPoint(touch->getLocation()))
	{
		Scene* s = TransitionFade::create(1.0f, LoadingScene::createScene());
		Director::getInstance()->replaceScene(s);
	}
	else if (shop->getBoundingBox().containsPoint(touch->getLocation()))
	{
		
	}
	else if (option->getBoundingBox().containsPoint(touch->getLocation()))
	{

	}

	return true;
}