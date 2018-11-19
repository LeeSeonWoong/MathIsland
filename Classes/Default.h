#pragma once

#include "cocos2d.h"

USING_NS_CC;
#pragma execution_character_set("utf-8")

// DelayTime + Sequence ��ũ���Լ�
#define RUN_WITH_DELAY(delay, func) Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(delay), CallFunc::create(func), nullptr))


// �浹�ڽ� On/Off
#define ColiideRect false

//����� ����
#include "singletonBase.h"
#include "soundManager.h"

//Ÿ��Ʋ
#include "TitleScene.h"

//��Ʈ�� ����
#include "IntroScene.h"

//�������� ����
#include "StageSelect.h"

//�ε���
#include "LoadingScene.h"

//��������01
#include <spine/spine-cocos2dx.h>
#include "ObjectEnemy.h"
#include "ObjectHero.h"
#include "GameUI.h"
#include "Stage01.h"

