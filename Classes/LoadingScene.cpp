#include "Default.h"

#pragma once

Scene* LoadingScene::createScene()
{
	auto scene = Scene::create();

	auto layer = LoadingScene::create();

	scene->addChild(layer);

	return scene;
}


bool LoadingScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//ChangeScene();

	_numberOfLoadedSprites = 0;
	_numberOfSprites = 150;

	auto size = Director::getInstance()->getWinSize();

	auto Back = Sprite::create("UI/Loading.png");
	Back->setPosition(Vec2(640, 360));
	this->addChild(Back,0);

	_labelLoading = Label::createWithTTF("LOADING", "fonts/Abduction.ttf", 50);
	_labelPercent = Label::createWithTTF("  %0", "fonts/Abduction.ttf", 50);

	_labelLoading->setPosition(Vec2(size.width / 2 - 40, size.height / 2 + 100));
	_labelPercent->setPosition(Vec2(size.width / 2 + 125, size.height / 2 + 100));

	this->addChild(_labelLoading,1);
	this->addChild(_labelPercent,1);

	char str[100] = { 0 };

	// load textrues
	for (int i = 0; i <= 7; ++i)// 8
	{
		sprintf(str, "block/Ani/Destroy1_%d.png", i);
		Director::getInstance()->getTextureCache()->addImageAsync(str, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	}
	for (int i = 1; i <= 9; ++i)// 9
	{
		sprintf(str, "block/Ani/effect_block_%d.png", i);
		Director::getInstance()->getTextureCache()->addImageAsync(str, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	}
	for (int i = 1; i <= 8; ++i)// 8
	{
		sprintf(str, "block/Ani/effect_get_%d.png", i);
		Director::getInstance()->getTextureCache()->addImageAsync(str, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	}
	for (int i = 1; i <= 6; ++i)// 6
	{
		sprintf(str, "block/Ani/plus_350_%d.png", i);
		Director::getInstance()->getTextureCache()->addImageAsync(str, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	}
	for (int i = 1; i <= 7; ++i)// 7
	{
		sprintf(str, "block/Ani/Save%d.png", i);
		Director::getInstance()->getTextureCache()->addImageAsync(str, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	}
	for (int i = 1; i <= 9; ++i)// 9
	{
		sprintf(str, "Hero/Hint%d.png", i);
		Director::getInstance()->getTextureCache()->addImageAsync(str, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	}
	for (int i = 0; i < 10; ++i) //10
	{
		sprintf(str, "block/Change_%d.png", i);
		Director::getInstance()->getTextureCache()->addImageAsync(str, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	}
	// 12
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_Basic.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_Div.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_fx.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_Left.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_Minus.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_Mul.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_Plus.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_Right.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_Same.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_X.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_Y.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Change_Z.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	// 14
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_0.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_2.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_3.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_5.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_I.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_LARGE1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_Multi.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_N.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_PLUS.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_S.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_SAME.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_Square.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/FIX_Y.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	// 15
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Basic.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Block_2.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Block_3.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Block_5.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Block_6.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Block_7.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Block_Div.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Block_LARGE1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Block_Minus.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Block_Mul.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Block_Plus.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Block_Square.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/Get_Coin.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/InteractBlock.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("block/BreakBlock.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	// 28
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/back_falls.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/back_layer_2.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/back_middle_rock1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/back_middle_rock1-2.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/back_middle_tree1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/back_night_2400.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/back_smalltree_ver1_1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/back1_temple1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/back1_temple2.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/back1_temple3.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/cloud_1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/cloud_2.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/middle_bush_back.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/middle_bush_front.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/middle_plant1_1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/middle_plant1_2.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/middle_plant1_3.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/middle_stone1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/middle_stone2.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/middle_tree1_1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/middle_tree1_2.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/middle_tree1_3.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/middle_tree1_4.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/sky_3.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/Sky2_1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/star_down.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/star_up.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("BackGround/temp1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	//11
	Director::getInstance()->getTextureCache()->addImageAsync("Object/ClearPoint.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("Object/Coin.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("Object/Black.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("Object/gate.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("Object/Scale1.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("Object/ScaleX.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("Object/ScaleY.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("Object/sin.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("Object/trap2.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("Object/Stone.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("Object/Stop.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	//13
	Director::getInstance()->getTextureCache()->addImageAsync("TileMap/MetaSet.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("TileMap/MetaSet2.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("TileMap/MetaSet3.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("TileMap/Interact.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("TileMap/Interact.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("UI/left.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("UI/right.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("UI/TempLife.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("UI/Life_point.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("UI/Stage_clear.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("UI/Game_over.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("UI/inventory.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("UI/jump.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));

	//터치리스너
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(LoadingScene::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void LoadingScene::loadingCallBack(Texture2D *texture)
{
	++_numberOfLoadedSprites;
	char tmp[10];
	sprintf(tmp, "%%%d", (int)(((float)_numberOfLoadedSprites / _numberOfSprites) * 100));
	_labelPercent->setString(tmp);

	if (_numberOfLoadedSprites == _numberOfSprites)
	{
		Sprite* spr = Sprite::create("BackGround/Touch_Screen.png");
		spr->setScale(2.0f);
		spr->setPosition(640, 300);
		this->addChild(spr, 2);
	}
}

void LoadingScene::onTouchBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
	//방어코드 추가 필요 bool 형 추가해서 한번만 실행하게
	////다음메뉴로넘어가기
	if (_numberOfLoadedSprites == _numberOfSprites)
	{
		this->removeChild(_labelLoading, true);
		this->removeChild(_labelPercent, true);
		Scene* s = TransitionFade::create(1.0f, Stage01::createScene());
		Director::getInstance()->replaceScene(s);
	}
}

void LoadingScene::ChangeScene()
{
	static bool once;
	if (once == false)
	{
		once = true;
		Scene* s = TransitionFade::create(1.0f, Stage01::createScene());
		Director::getInstance()->replaceScene(s);
	}
}
