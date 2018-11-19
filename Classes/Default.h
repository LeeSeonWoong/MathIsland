#pragma once

#include "cocos2d.h"

USING_NS_CC;
#pragma execution_character_set("utf-8")

// DelayTime + Sequence 매크로함수
#define RUN_WITH_DELAY(delay, func) Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(delay), CallFunc::create(func), nullptr))


// 충돌박스 On/Off
#define ColiideRect false

//오디오 엔진
#include "singletonBase.h"
#include "soundManager.h"

//타이틀
#include "TitleScene.h"

//인트로 영상
#include "IntroScene.h"

//스테이지 선택
#include "StageSelect.h"

//로딩씬
#include "LoadingScene.h"

//스테이지01
#include <spine/spine-cocos2dx.h>
#include "ObjectEnemy.h"
#include "ObjectHero.h"
#include "GameUI.h"
#include "Stage01.h"

