#include "Default.h"
#include "AudioEngine.h"

//using namespace experimental;

Scene* Stage01::createScene()
{
	auto scene = Scene::create();
	auto layer = Stage01::create();
	scene->addChild(layer, 1);
	auto GamePad = GameUI::create();
	GamePad->setTag(100);
	scene->addChild(GamePad, 5);

	return scene;
}

bool Stage01::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//사운드 
	CSoundManager::getInstance()->stopBgm("Sound/Dog_and_Pony_Show.mp3");
	CSoundManager::getInstance()->playBgm("Sound/bgm_tutorial.mp3", true, false, 0.6f);
	CSoundManager::getInstance()->preloadAudioFile("Sound/Jump.mp3");
	CSoundManager::getInstance()->preloadAudioFile("Sound/Coin_Get.mp3");
	CSoundManager::getInstance()->preloadAudioFile("Sound/Box_Get.mp3");
	CSoundManager::getInstance()->preloadAudioFile("Sound/Box_Change.mp3");
	CSoundManager::getInstance()->preloadAudioFile("Sound/Box_Fit.mp3");
	CSoundManager::getInstance()->preloadAudioFile("Sound/Monster_Death1.mp3");
	CSoundManager::getInstance()->preloadAudioFile("Sound/Gate_Open.mp3");
	CSoundManager::getInstance()->preloadAudioFile("Sound/Game_Over.mp3");
	CSoundManager::getInstance()->preloadAudioFile("Sound/Game_Clear.mp3");
	CSoundManager::getInstance()->preloadAudioFile("Sound/cat_sound4.mp3");
	CSoundManager::getInstance()->preloadAudioFile("Sound/QuizClear.mp3");
	CSoundManager::getInstance()->preloadAudioFile("Sound/Save.mp3");

	m_winSize = Director::getInstance()->getWinSize();
	m_Life = UserDefault::getInstance()->getIntegerForKey("MathLife", 3);
	m_Scale = Block::Change_Right;
	curTime = 0;
	m_EnemyColide = false;
	OverCheck = false;
	duncon = false;
	timer1 = 0;
	CameraFix = false;
	stoneOnce = false;


	coinEvent = 0;
	con1 = false;
	con2 = false;
	con3 = false;
	con4 = false;
	con5 = false;
	con6 = false;
	con7 = false;

	event1 = false;
	event2 = false;
	event3 = false;
	event4 = false;
	event5 = false;

	for (int i = 0; i < 3; ++i)
		once[i] = false;

	Sprite* back = Sprite::create("BackGround/Sky2_1.png");
	back->setPosition(Vec2(m_winSize.width*0.5f, m_winSize.height*0.5f));
	this->addChild(back, -1);

	Sprite* bacK2 = Sprite::create("BackGround/back_night_2400.png");
	bacK2->setPosition(Vec2(m_winSize.width*0.5f + back->getContentSize().width, m_winSize.height*0.5f));
	this->addChild(bacK2, -1);

	CreateBack();

	//애니메이션///////////////////////////////////////////////
	char str[100] = { 0 };
	for (int i = 0; i < EffectNum; i++)
	{
		effect[i] = Animation::create();
		if (i == 4)
			effect[i]->setDelayPerUnit(0.3f);
		else
			effect[i]->setDelayPerUnit(0.1f);
	}

	for (int i = 0; i <= 7; i++)// 블록파괴
	{
		sprintf(str, "block/Ani/Destroy1_%d.png", i);
		effect[0]->addSpriteFrameWithFileName(str);
	}
	effect[0]->setRestoreOriginalFrame(true);//애니메이션 종료시 원래 스프라이트로 복귀

	for (int i = 1; i <= 9; i++)// 블록파괴
	{
		sprintf(str, "block/Ani/effect_block_%d.png", i);
		effect[1]->addSpriteFrameWithFileName(str);
	}
	effect[1]->setRestoreOriginalFrame(true);//애니메이션 종료시 원래 스프라이트로 복귀

	for (int i = 1; i <= 8; i++)// 블록파괴
	{
		sprintf(str, "block/Ani/effect_get_%d.png", i);
		effect[2]->addSpriteFrameWithFileName(str);
	}
	effect[2]->setRestoreOriginalFrame(true);//애니메이션 종료시 원래 스프라이트로 복귀

	//for (int i = 1; i <= 9; i++)// 블록파괴
	//{
	//	sprintf(str, "block/Ani/effect_extinction_%d.png", i);
	//	effect[3]->addSpriteFrameWithFileName(str);
	//}
	//effect[3]->setRestoreOriginalFrame(true);//애니메이션 종료시 원래 스프라이트로 복귀

	for (int i = 1; i <= 4; i++)// 말풍선
	{
		sprintf(str, "Hero/Speech%d.png", i);
		effect[4]->addSpriteFrameWithFileName(str);
	}
	for (int i = 1; i <= 9; i++)// 말풍선 힌트
	{
		sprintf(str, "Hero/Hint%d.png", i);
		effect[5]->addSpriteFrameWithFileName(str);
	}
	for (int i = 1; i <= 6; i++)// +350
	{
		sprintf(str, "block/Ani/plus_350_%d.png", i);
		effect[6]->addSpriteFrameWithFileName(str);
	}
	for (int i = 1; i <= 7; i++)// Save
	{
		sprintf(str, "block/Ani/Save%d.png", i);
		effect[7]->addSpriteFrameWithFileName(str);
	}
	for (int i = 1; i <= 6; i++)// 57
	{
		sprintf(str, "block/Ani/plus_57_%d.png", i);
		effect[8]->addSpriteFrameWithFileName(str);
	}
	for (int i = 1; i <= 6; i++)// 43
	{
		sprintf(str, "block/Ani/plus_43_%d.png", i);
		effect[9]->addSpriteFrameWithFileName(str);
	}
	for (int i = 1; i <= 6; i++)// 7
	{
		sprintf(str, "block/Ani/plus_7_%d.png", i);
		effect[10]->addSpriteFrameWithFileName(str);
	}
	for (int i = 0; i < EffectNum; i++)
		CC_SAFE_RETAIN(effect[i]);
	////////////////////////////////////////////////////////////

	m_Score = 0;
	m_Stage01Event1 = false;

	for (int i = 0; i < 10; ++i)
		m_condition[i] = false;

	m_Timer = 0;

	RoadTileMap();

	CameraAction2();

	this->scheduleUpdate();

	return true;
}

void Stage01::CameraAction2()
{
	auto cam1 = Sprite::create("block/BreakBlock.png");
	cam1->setName("Cam1");
	cam1->setPosition(Vec2(9840, 225));
	cam1->setVisible(false);
	this->addChild(cam1, 6);
	extern ObjectHero* Hero;

	cam1->runAction(Sequence::create(
		MoveTo::create(1.5f, Vec2(9280, 1040)),
		MoveTo::create(1.8f, Vec2(7117, 2245)),
		MoveTo::create(1.0f, Vec2(6500, 1700)),
		MoveTo::create(1.0f, Vec2(6000, 500)),
		MoveTo::create(1.0f, Vec2(5000, 900)),
		MoveTo::create(1.5f, Vec2(3900, 1600)),
		MoveTo::create(1.0f, Vec2(2500, 1600)),
		MoveTo::create(1.5f, Vec2(Hero->m_Spr->getPositionX(), Hero->m_Spr->getPositionY())),
		NULL));
}

void Stage01::CameraAction1()
{
	if (Camera::getDefaultCamera() != NULL)
	{
		Camera::getDefaultCamera()->runAction(Sequence::create(
			ScaleTo::create(0.4f, 0.5f, 0.5f),
			ScaleTo::create(0.3f, 1.0f, 1.0f),
			NULL));
	}
}

void Stage01::AnimationSwitch(Sprite* block)
{
	extern ObjectHero* Hero;
	float tagnum = block->getTag();
	if (block->getTag() == Block::Coin)
	{
		Hero->CoinNum += 1;
		UserDefault::getInstance()->setIntegerForKey("HeroCoin", Hero->CoinNum);
		CSoundManager::getInstance()->playEffect("Sound/Coin_Get.mp3", false, 1.0f);
		block->runAction(Sequence::create(
			Spawn::create(
				MoveTo::create(0.3f, Vec2(block->getPositionX() + 78, block->getPositionY() + 100)),
				RotateTo::create(0.3f, Vec3(0.0f, 180.0f, 0.0f)),
				NULL),
			RemoveSelf::create(),
			NULL));
	}
	else if (block->getTag() == Block::Break)
	{
		CSoundManager::getInstance()->playEffect("Sound/BoxColide.mp3", false, 1.0f);
		block->runAction(Sequence::create(
			MoveTo::create(0.0f, Vec2(block->getPositionX() - 110, block->getPositionY() + 80)),
			Animate::create(effect[0]),
			RemoveSelf::create(),
			NULL));
	}
	else if (block->getTag() >= 320 && block->getTag() <= 333)
	{
		CSoundManager::getInstance()->playEffect("Sound/Box_Get.mp3", false, 1.0f);
		block->runAction(Sequence::create(
			MoveTo::create(0.0f, Vec2(block->getPositionX() - 40, block->getPositionY() + 40)),
			Animate::create(effect[2]),
			RemoveSelf::create(),
			NULL));
	}
	else if (block->getTag() >= 300 && block->getTag() <= 319)
	{
		CSoundManager::getInstance()->playEffect("Sound/Box_Change.mp3", false, 1.0f);
		auto Spr = Sprite::create();
		Spr->setPosition(Vec2(block->getPositionX() + 40, block->getPositionY() - 80));
		Spr->runAction(Sequence::create(
			Animate::create(effect[1]),
			RemoveSelf::create(),
			NULL));
		this->addChild(Spr, 3);
	}
	else if (block->getTag() == 100)
	{
		UserDefault::getInstance()->setIntegerForKey("HeroCoin", Hero->CoinNum);
		extern ObjectHero* Hero;
		auto Spr = Sprite::create();
		Spr->setPosition(Vec2(Hero->m_Spr->getPositionX(), Hero->m_Spr->getPositionY() + 80));
		for (Sprite* box : m_ChangeBox)
		{
			if (box->getPositionX() == 5600)
			{
				if (box->getTag() == Block::Change_Mul)
				{
					coinEvent = 0;
				}
				else if(box->getTag() == Block::Change_Div)
				{
					coinEvent = 1;
				}
				else if (box->getTag() == Block::Change_Minus)
				{
					coinEvent = 3;
				}
				else if (box->getTag() == Block::Change_Plus)
				{
					coinEvent = 2;
				}
			}
		}
		if (coinEvent == 0)
		{
			Hero->CoinNum += 350;
			Spr->runAction(Sequence::create(
				ScaleTo::create(0.0f, 1.5f),
				Animate::create(effect[6]),
				RemoveSelf::create(),
				NULL));
		}
		else if (coinEvent == 1)
		{
			Hero->CoinNum += 7;
			Spr->runAction(Sequence::create(
				ScaleTo::create(0.0f, 1.5f),
				Animate::create(effect[10]),
				RemoveSelf::create(),
				NULL));
		}
		else if (coinEvent == 2)
		{
			Hero->CoinNum += 57;
			Spr->runAction(Sequence::create(
				ScaleTo::create(0.0f, 1.5f),
				Animate::create(effect[8]),
				RemoveSelf::create(),
				NULL));
		}
		else if (coinEvent == 3)
		{
			Hero->CoinNum += 43;
			Spr->runAction(Sequence::create(
				ScaleTo::create(0.0f, 1.5f),
				Animate::create(effect[9]),
				RemoveSelf::create(),
				NULL));
		}
		this->addChild(Spr, 5);
	}
}

//인벤에서 선택한 박스를 배치와 동시에 해당위치에 생성
void Stage01::CreateBoxFromInven(Sprite* spr, int tagNum)
{
	if (tagNum >= 320 && tagNum < 330)
	{
		char str[100] = {};
		sprintf(str, "block/Get_Block_%d.png", tagNum - 320);
		auto box = Sprite::create(str);
		box->setTag(tagNum);
		box->setAnchorPoint(Vec2(0.0f, 1.0f));
		box->setPosition(spr->getPosition());
		this->addChild(box, 5);
		m_ToSave = box;
		box->runAction(Sequence::create(
			DelayTime::create(0.4f),
			CallFunc::create(CC_CALLBACK_0(Stage01::DelayCreate, this)), NULL));
	}
	else if (tagNum == 330)
	{
		auto box = Sprite::create("block/Get_Block_Square.png");
		box->setTag(Block::GetSquare);
		box->setAnchorPoint(Vec2(0.0f, 1.0f));
		box->setPosition(spr->getPosition());
		this->addChild(box, 5);
		m_ToSave = box;
		box->runAction(Sequence::create(
			DelayTime::create(0.4f),
			CallFunc::create(CC_CALLBACK_0(Stage01::DelayCreate, this)), NULL));
	}
	else if (tagNum == 331)
	{
		auto box = Sprite::create("block/Get_Block_Minus.png");
		box->setTag(Block::GetMinus);
		box->setAnchorPoint(Vec2(0.0f, 1.0f));
		box->setPosition(spr->getPosition());
		this->addChild(box, 5);
		m_ToSave = box;
		box->runAction(Sequence::create(
			DelayTime::create(0.4f),
			CallFunc::create(CC_CALLBACK_0(Stage01::DelayCreate, this)), NULL));
	}
	else if (tagNum == 332)
	{
		auto box = Sprite::create("block/Get_Block_Plus.png");
		box->setTag(Block::GetPlus);
		box->setAnchorPoint(Vec2(0.0f, 1.0f));
		box->setPosition(spr->getPosition());
		this->addChild(box, 5);
		m_ToSave = box;
		box->runAction(Sequence::create(
			DelayTime::create(0.4f),
			CallFunc::create(CC_CALLBACK_0(Stage01::DelayCreate, this)), NULL));
	}
	else if (tagNum == 333)
	{
		auto box = Sprite::create("block/Get_Block_Div.png");
		box->setTag(Block::GetDiv);
		box->setAnchorPoint(Vec2(0.0f, 1.0f));
		box->setPosition(spr->getPosition());
		this->addChild(box, 5);
		m_ToSave = box;
		box->runAction(Sequence::create(
			DelayTime::create(0.4f),
			CallFunc::create(CC_CALLBACK_0(Stage01::DelayCreate, this)), NULL));
	}
}

void Stage01::CraeteEnemy()
{
	for (int i = 0; i < 3; ++i)
	{
		m_enemy[i] = new ObjectEnemy();
		m_enemy[i]->GetLayer(this);
		if (i == 0 || i == 2)
			m_enemy[i]->CreateObject(4650 + i * 100, 950 - i * 100, true);
		else if (i == 1)
			m_enemy[i]->CreateObject(4650 + i * 100, 950 - i * 100, false);
	}

	for (int i = 0; i < 3; ++i)
	{
		m_Fly[i] = new ObjectEnemy();
		m_Fly[i]->GetLayer(this);
		if (i == 0)
			m_Fly[i]->CreateObject2(8540, 1620, true);
		else if (i == 1)
			m_Fly[i]->CreateObject2(7940, 1060, true);
		else
			m_Fly[i]->CreateObject2(8840, 1060, true);
	}

	for (int i = 0; i < 3; ++i)
	{
		m_Ground[i] = new ObjectEnemy();
		m_Ground[i]->GetLayer(this);
		if (i == 0)
			m_Ground[i]->CreateObject3(9500, 1115, true);
		else if (i == 1)
			m_Ground[i]->CreateObject3(9650, 1435, false);
		else
			m_Ground[i]->CreateObject3(9500, 1755, true);
	}
}

void Stage01::DelayCreate()
{
	m_box.pushBack(m_ToSave);
	m_ToSave = NULL;
}

void Stage01::EventCheck(Sprite* block)
{
	// 부서지는 블록 -> 파괴 -> 세팅블록 생성
	if (block->getTag() == Block::Break && (block->getPositionX() == 3800 || block->getPositionX() == 5920 || block->getPositionX() == 5680))
	{
		For_SetBlock = block->getPosition();
		RUN_WITH_DELAY(0.5f, CC_CALLBACK_0(Stage01::CreateSetting1, this));
	}
	else if (block->getTag() == Block::Get2 && block->getPositionX() == 3800)
	{
		For_SetBlock = block->getPosition();
		RUN_WITH_DELAY(0.5f, CC_CALLBACK_0(Stage01::CreateSetting1, this));
	}
	else if (block->getTag() == Block::GetPlus && block->getPositionX() == 4000)
	{
		For_SetBlock = block->getPosition();
		RUN_WITH_DELAY(0.5f, CC_CALLBACK_0(Stage01::CreateSetting1, this));
	}
	// 저울 아래 동전 지형
	else if (block->getTag() == Block::Get7 && block->getPositionX() == 5920)
	{
		For_SetBlock = block->getPosition();
		RUN_WITH_DELAY(0.5f, CC_CALLBACK_0(Stage01::CreateSetting1, this));
	}
	else if (block->getTag() == Block::GetMinus && (block->getPositionX() == 5920 || block->getPositionX() == 5680))
	{
		For_SetBlock = block->getPosition();
		RUN_WITH_DELAY(0.5f, CC_CALLBACK_0(Stage01::CreateSetting1, this));
	}
	else if (block->getTag() == Block::Get7 && block->getPositionX() == 6960)
	{
		For_SetBlock = block->getPosition();
		RUN_WITH_DELAY(0.5f, CC_CALLBACK_0(Stage01::CreateSetting1, this));
	}
}

void Stage01::CreateSetting1()
{
	auto blockAnswer_1 = Sprite::create("block/InteractBlock.png");
	blockAnswer_1->setAnchorPoint(Vec2(0.0f, 1.0f));
	blockAnswer_1->setPosition(For_SetBlock);
	//blockAnswer_1->setVisible(ColiideRect);
	blockAnswer_1->setVisible(true);
	blockAnswer_1->setTag(Block::Setting);
	this->addChild(blockAnswer_1, 3);
	m_box.pushBack(blockAnswer_1);
}

// 박스와 주인공이 충돌했는지 체크하고 화면상에 없에줌
void Stage01::CheckBoxObject(float dt)
{
	extern ObjectHero* Hero;

	if (con1 && con2 && m_condition[3] == false)
	{
		CSoundManager::getInstance()->playEffect("Sound/QuizClear.mp3", false, 1.0f);
		m_condition[3] = true;
	}
	else if (con3)//추후에 con1&&con2를 첨가해야함
	{
		con3 = false;
		m_condition[5] = true;
	}
	else if (con4)
	{
		//CSoundManager::getInstance()->playEffect("Sound/QuizClear.mp3", false, 0.8f);
		m_condition[7] = true;
	}
	else if (con6)
	{
		con6 = false;
		CSoundManager::getInstance()->playEffect("Sound/QuizClear.mp3", false, 1.0f);
		m_condition[8] = true;
	}
	if (con7)
	{
		CSoundManager::getInstance()->playEffect("Sound/QuizClear.mp3", false, 1.0f);
		con7 = false;
		m_condition[9] = true;
	}
	for (Sprite* box : m_box)
	{
		if (Hero->m_Spr->getBoundingBox().intersectsRect(box->getBoundingBox()))
		{
			EventCheck(box);
			//코인
			if (box->getTag() == Block::Coin)
			{
				m_Score += 10;
			}
			// 획득가능박스는 저장
			if (box->getTag() >= 320 && box->getTag() <= 340)
			{
				// 현재 박스 텍스쳐가 사라지기전까지는 저장된 박스가 메모리 오류가 발생함
				// 일단 임시로 sprite를 복사해 새로 만든후 저장하는 방식 기존 것은 애니메이션및 삭제 정상적으로 진행
				Sprite* spr = Sprite::createWithSpriteFrame(box->getSpriteFrame());
				spr->setTag(box->getTag());

				m_Inven.pushBack(spr);
			}
			// 끼워넣기용 블럭
			if (box->getTag() == Block::Setting)
			{

				if (Hero->m_Have)
				{
					if (Hero->m_box->getTag() == Block::Get6)
					{
						CSoundManager::getInstance()->playEffect("Sound/Box_Fit.mp3", false, 1.0f);
						m_condition[1] = true;
						box->setTexture(Hero->m_box->getTexture());
						Hero->InitSelectBox();
						box->setVisible(true);
						m_box.eraseObject(box);
						DeleteInvenBox(Hero->m_box->getTag());
						return;
					}
					else if (box->getPositionX() == 4000)//나주엔 condition[0]을 포함시켜서 조건을 추가해야함
					{
						if (Hero->m_box->getTag() == Block::Get2 && con1 == false)
						{
							CSoundManager::getInstance()->playEffect("Sound/Box_Fit.mp3", false, 1.0f);
							con1 = true;
							box->setTexture(Hero->m_box->getTexture());
							Hero->InitSelectBox();
							box->setVisible(true);
							m_box.eraseObject(box);
							DeleteInvenBox(Hero->m_box->getTag());
							return;
						}
						else
							con1 = false;
					}
					else if (box->getPositionX() == 3800 && con2 == false)
					{
						if (Hero->m_box->getTag() == Block::GetPlus)
						{
							CSoundManager::getInstance()->playEffect("Sound/Box_Fit.mp3", false, 1.0f);
							con2 = true;
							box->setTexture(Hero->m_box->getTexture());
							Hero->InitSelectBox();
							box->setVisible(true);
							m_box.eraseObject(box);
							DeleteInvenBox(Hero->m_box->getTag());
							return;
						}
						else
							con2 = false;
					}
					else if (box->getPositionX() == 5920 && con3 == false)
					{
						if (Hero->m_box->getTag() == Block::Get5)
						{
							CSoundManager::getInstance()->playEffect("Sound/Box_Fit.mp3", false, 1.0f);
							con3 = true;
							box->setTexture(Hero->m_box->getTexture());
							Hero->InitSelectBox();
							box->setVisible(true);
							m_box.eraseObject(box);
							DeleteInvenBox(Hero->m_box->getTag());
							return;
						}
						else
							con3 = false;
					}
					else if (box->getPositionX() == 5680 && con4 == false)
					{
						if (Hero->m_box->getTag() == Block::Get7)
						{
							CSoundManager::getInstance()->playEffect("Sound/Box_Fit.mp3", false, 1.0f);
							con4 = true;
							box->setTag(100);
							AnimationSwitch(box);
							box->setTexture(Hero->m_box->getTexture());
							Hero->InitSelectBox();
							box->setVisible(true);
							m_box.eraseObject(box);
							DeleteInvenBox(Hero->m_box->getTag());
							return;
						}
						else
							con4 = false;
					}
					else if (box->getPositionX() == 5680 && con4 == false)
					{
						if (Hero->m_box->getTag() == Block::Get7)
						{
							CSoundManager::getInstance()->playEffect("Sound/Box_Fit.mp3", false, 1.0f);
							con4 = true;
							box->setTag(100);
							AnimationSwitch(box);
							box->setTexture(Hero->m_box->getTexture());
							Hero->InitSelectBox();
							box->setVisible(true);
							m_box.eraseObject(box);
							DeleteInvenBox(Hero->m_box->getTag());
							return;
						}
						else
							con4 = false;
					}
					else if (box->getPositionX() == 6960 && con6 == false)
					{
						if (Hero->m_box->getTag() == Block::GetMinus)
						{
							CSoundManager::getInstance()->playEffect("Sound/Box_Fit.mp3", false, 1.0f);
							auto Sin = dynamic_cast<Sprite*>(this->getChildByName("SIN"));
							Sin->setVisible(true);
							Hero->Sin_Event = true;
							con6 = true;
							AnimationSwitch(box);
							box->setTexture(Hero->m_box->getTexture());
							Hero->InitSelectBox();
							box->setVisible(true);
							m_box.eraseObject(box);
							DeleteInvenBox(Hero->m_box->getTag());
							return;
						}
						else
							con6 = false;
					}
					else if (box->getPositionX() == 9200 && con7 == false)
					{
						if (Hero->m_box->getTag() == Block::Get3)
						{
							CSoundManager::getInstance()->playEffect("Sound/Box_Fit.mp3", false, 1.0f);
							con7 = true;
							AnimationSwitch(box);
							box->setTexture(Hero->m_box->getTexture());
							Hero->InitSelectBox();
							box->setVisible(true);
							m_box.eraseObject(box);
							DeleteInvenBox(Hero->m_box->getTag());
							return;
						}
						else
							con7 = false;
					}
					// 재배치한 박스를 다시 재획득 가능하게 하는 소스
					CreateBoxFromInven(box, Hero->m_box->getTag());
					box->setTexture(Hero->m_box->getTexture());
					Hero->InitSelectBox();
					box->setVisible(false);
					m_box.eraseObject(box);
					DeleteInvenBox(Hero->m_box->getTag());
					return;
				}
				else
				{
					// setting블록 출동해도 현재 상태변환 없음
					//m_box.eraseObject(box);
					//this->removeChild(box);
					return;
				}
			}
			AnimationSwitch(box);//->애니메이션에서 자체삭제해주고 있음
			m_box.eraseObject(box);

			return;
		}
	}
	Vec2 pos;
	for (Sprite* box : m_ChangeBox)
	{
		if (Hero->m_Spr->getBoundingBox().intersectsRect(box->getBoundingBox()) && m_Timer - curTime >= 0.4 &&
			Hero->m_Spr->getPositionY() < box->getPositionY())
		{
			curTime = m_Timer;
			int num = box->getTag();
			pos = box->getPosition();
			switch (num)
			{
			case Block::Change0:
				m_condition[0] = false;
				box->initWithFile("block/Change_1.png");
				break;
			case Block::Change1:
				m_condition[0] = false;
				box->initWithFile("block/Change_2.png");
				break;
			case Block::Change2:
				m_condition[0] = false;
				box->initWithFile("block/Change_3.png");
				break;
			case Block::Change3:
				m_condition[0] = true;
				box->initWithFile("block/Change_4.png");
				break;
			case Block::Change4:
				m_condition[0] = false;
				box->initWithFile("block/Change_5.png");
				break;
			case Block::Change5:
				m_condition[0] = false;
				box->initWithFile("block/Change_6.png");
				break;
			case Block::Change6:
				m_condition[0] = false;
				box->initWithFile("block/Change_7.png");
				break;
			case Block::Change7:
				m_condition[0] = false;
				box->initWithFile("block/Change_8.png");
				break;
			case Block::Change8:
				m_condition[0] = false;
				box->initWithFile("block/Change_9.png");
				break;
			case Block::Change9:
				m_condition[0] = false;
				box->initWithFile("block/Change_0.png");
				break;
				/////////////////////////////////////////////
			case Block::ChangeX:
				m_condition[2] = false;
				box->initWithFile("block/Change_Y.png");
				break;
			case Block::ChangeY:
				m_condition[2] = false;
				box->initWithFile("block/Change_Z.png");
				break;
			case Block::ChangeZ:
				m_condition[2] = true;
				box->initWithFile("block/Change_X.png");
				break;
			case Block::Change_Mul:
				m_condition[4] = false;
				box->initWithFile("block/Change_Div.png");
				break;
			case Block::Change_Div:
				m_condition[4] = false;
				box->initWithFile("block/Change_Plus.png");
				break;
			case Block::Change_Plus:
				m_condition[4] = false;
				box->initWithFile("block/Change_Minus.png");
				break;
			case Block::Change_Minus:
				m_condition[4] = true;
				box->initWithFile("block/Change_Mul.png");
				break;
			case Block::Change_Left:
				m_Scale = Block::Change_Right;
				box->initWithFile("block/Change_Right.png");
				break;
			case Block::Change_Right:
				m_Scale = Block::Change_Same;
				box->initWithFile("block/Change_Same.png");
				break;
			case Block::Change_Same:
				m_Scale = Block::Change_Left;
				box->initWithFile("block/Change_Left.png");
				break;
			}

			if (num == Block::Change9)
				box->setTag(Block::Change0);
			else if (num == Block::ChangeZ)
				box->setTag(Block::ChangeX);
			else if (num == Block::Change_Minus)
				box->setTag(Block::Change_Mul);
			else if (num == Block::Change_Same)
				box->setTag(Block::Change_Left);
			else
				box->setTag(num + 1);
			box->setAnchorPoint(Vec2(0.0f, 1.0f));
			box->setPosition(pos);
			AnimationSwitch(box);

			// 바뀐조건체크 나머지 체크도 여기서 옮길필요가 있음
			// 던전 블록체인지 체크
			if (box->getPositionX() == 3200 && box->getTag() == Block::Change5)
			{
				m_condition[6] = true;
			}
		}
	}
}


void Stage01::DeleteInvenBox(int tagNum)
{
	for (Sprite* box : m_Inven)
	{
		if (box->getTag() == tagNum)
		{
			m_Inven.eraseObject(box);
			return;
		}
	}
}

// 주변환경 스프라이트 배치
void Stage01::CreateBack()
{
	auto tree1 = Sprite::create("BackGround/back_layer_2.png");
	tree1->setPosition(Vec2(440, 1280));
	this->addChild(tree1, -1);

	auto temp1 = Sprite::create("BackGround/temp1.png");
	temp1->setPosition(Vec2(1640, 1120));
	this->addChild(temp1, 3);

	auto noon1 = Sprite::create("BackGround/back1_temple1.png");
	noon1->setPosition(Vec2(1640, 1280));
	this->addChild(noon1, -1);

	auto noon2 = Sprite::create("BackGround/back1_temple2.png");
	noon2->setPosition(Vec2(1740, 1280));
	this->addChild(noon2, -1);

	auto plant1 = Sprite::create("BackGround/middle_plant1_1.png");
	plant1->setPosition(Vec2(2300, 1270));
	this->addChild(plant1, 3);

	auto plant2 = Sprite::create("BackGround/middle_tree1_3.png");
	plant2->setPosition(Vec2(2460, 1270));
	this->addChild(plant2, -1);

	auto plant3 = Sprite::create("BackGround/middle_plant1_2.png");
	plant3->setPosition(Vec2(2700, 1350));
	this->addChild(plant3, -1);

	auto temp2 = Sprite::create("BackGround/back1_temple3.png");
	temp2->setPosition(Vec2(2060, 1370));
	this->addChild(temp2, -1);

	auto plant4 = Sprite::create("BackGround/middle_tree1_1.png");
	plant4->setPosition(Vec2(3380, 1150));
	this->addChild(plant4, -1);

	auto stone1 = Sprite::create("BackGround/middle_stone1.png");
	stone1->setPosition(Vec2(3380, 960));
	this->addChild(stone1, -1);

	auto plant5 = Sprite::create("BackGround/back_middle_rock1-2.png");
	plant5->setPosition(Vec2(3880, 1350));
	this->addChild(plant5, -1);

	auto fall1 = Sprite::create("BackGround/back_falls.png");
	fall1->setPosition(Vec2(4760, 850));
	this->addChild(fall1, -1);

	auto plant6 = Sprite::create("BackGround/middle_tree1_3.png");
	plant6->setPosition(Vec2(5780, 710));
	this->addChild(plant6, -1);

	auto plant7 = Sprite::create("BackGround/middle_tree1_2.png");
	plant7->setPosition(Vec2(5980, 710));
	this->addChild(plant7, -1);

	auto plant8 = Sprite::create("BackGround/middle_bush_back.png");
	plant8->setPosition(Vec2(5980, 470));
	this->addChild(plant8, -1);

	auto stone2 = Sprite::create("BackGround/middle_stone2.png");
	stone2->setPosition(Vec2(6180, 470));
	this->addChild(stone2, -1);

	auto plant9 = Sprite::create("BackGround/middle_tree1_1.png");
	plant9->setPosition(Vec2(5980, 470));
	this->addChild(plant9, -1);

	auto plant10 = Sprite::create("BackGround/back_middle_tree1.png");
	plant10->setPosition(Vec2(6680, 790));
	this->addChild(plant10, -1);

	auto star = Sprite::create("BackGround/star_up.png");
	star->setPosition(Vec2(6680, 1890));
	this->addChild(star, -1);

	auto sky = Sprite::create("BackGround/sky_3.png");
	sky->setPosition(Vec2(6380, 2190));
	this->addChild(sky, -1);

	auto cloud1 = Sprite::create("BackGround/cloud_1.png");
	cloud1->setPosition(Vec2(7880, 2190));
	this->addChild(cloud1, -1);

	auto star2 = Sprite::create("BackGround/star_down.png");
	star2->setPosition(Vec2(8580, 1890));
	this->addChild(star2, -1);

	auto plant11 = Sprite::create("BackGround/back_smalltree_ver1_1.png");
	plant11->setPosition(Vec2(9155, 930));
	this->addChild(plant11, -1);
}

// 맵로딩
void Stage01::RoadTileMap()
{
	m_tmap = TMXTiledMap::create("TileMap/MathStage01.tmx");
	m_background = m_tmap->getLayer("Background");

	m_metainfo = m_tmap->getLayer("MetaInfo");
	m_metainfo->setVisible(false);

	m_metainfo2 = m_tmap->getLayer("MetaInfo2");
	m_metainfo2->setVisible(false);


	m_metainfo3 = m_tmap->getLayer("MetaInfo3");
	m_metainfo3->setVisible(false);

	interact = m_tmap->getLayer("Interact1");
	interact->setVisible(false);

	this->addChild(m_tmap, 0, 11);

	TMXObjectGroup* Spawn_Pos = m_tmap->getObjectGroup("Spawn");
	ValueMap spawnPoint = Spawn_Pos->getObject("SpawnPoint");

	float x = spawnPoint["x"].asFloat();
	float y = spawnPoint["y"].asFloat();

	spawnPoint = Spawn_Pos->getObject("MoveBox");
	DunPos.x = spawnPoint["x"].asFloat();
	DunPos.y = spawnPoint["y"].asFloat();

	Vec2 tileCoord;
	int tileGid;
	auto metabox = Sprite::create("MetaSet.png");
	this->addChild(metabox);
	auto metabox2 = Sprite::create("TileMap/MetaSet2.png");
	this->addChild(metabox2);

	auto metabox3 = Sprite::create("TileMap/MetaSet3.png");
	this->addChild(metabox3);

	//충돌박스 Vector에 삽입
	for (int i = 0; i <= 28; ++i)
	{
		for (int j = 0; j <= 132; ++j)
		{
			tileCoord.x = j;
			tileCoord.y = i;
			tileGid = m_metainfo->getTileGIDAt(tileCoord);
			if (tileGid)
			{
				Value property2 = m_tmap->getPropertiesForGID(tileGid);
				property2 = m_metainfo->getProperties();
				if (!property2.isNull())
				{
					std::string wall = property2.asValueMap()["Wall"].asString();
					if (wall == "YES")
					{
						int x = tileCoord.x * m_tmap->getTileSize().width;
						int y = 2320 - tileCoord.y * m_tmap->getTileSize().height;

						auto spr = Sprite::createWithSpriteFrame(metabox->getSpriteFrame());
						spr->setVisible(ColiideRect);
						spr->setAnchorPoint(Vec2(0.0f, 1.0f));
						spr->setPosition(x, y);
						this->addChild(spr, 5);
						m_metabox.pushBack(spr);
					}
				}
			}
			tileGid = m_metainfo2->getTileGIDAt(tileCoord);
			if (tileGid)
			{
				Value property2 = m_tmap->getPropertiesForGID(tileGid);
				property2 = m_metainfo2->getProperties();
				if (!property2.isNull())
				{
					std::string wall = property2.asValueMap()["Wall"].asString();
					if (wall == "YES")
					{
						int x = tileCoord.x * m_tmap->getTileSize().width;
						int y = 2320 - tileCoord.y * m_tmap->getTileSize().height;
						auto spr = Sprite::createWithSpriteFrame(metabox2->getSpriteFrame());
						spr->setTag(55);
						spr->setVisible(ColiideRect);
						spr->setAnchorPoint(Vec2(0.0f, 1.0f));
						spr->setPosition(x, y);
						this->addChild(spr, 5);
						m_metabox.pushBack(spr);
					}
				}
			}

			tileGid = m_metainfo3->getTileGIDAt(tileCoord);
			if (tileGid)
			{
				Value property2 = m_tmap->getPropertiesForGID(tileGid);
				property2 = m_metainfo3->getProperties();
				if (!property2.isNull())
				{
					std::string wall = property2.asValueMap()["Wall"].asString();
					if (wall == "YES")
					{
						int x = tileCoord.x * m_tmap->getTileSize().width;
						int y = 2320 - tileCoord.y * m_tmap->getTileSize().height;

						auto spr = Sprite::createWithSpriteFrame(metabox3->getSpriteFrame());
						spr->setVisible(true);
						spr->setAnchorPoint(Vec2(0.0f, 1.0f));
						spr->setPosition(x, y);
						this->addChild(spr, 2);
					}
				}
			}
		}
	}
	metabox->removeFromParent();

	//// 주인공 생성및 초기화
	extern ObjectHero* Hero;
	Hero->GetLayer(this);
	m_SaveNum = UserDefault::getInstance()->getIntegerForKey("Save5", 0);

	if(m_SaveNum == 0)
		Hero->CreateObject(318, 1024);
	else if(m_SaveNum == 1)
		Hero->CreateObject(x + 3500, y - 10);

	setViewPointCenter(Hero->m_Spr->getPosition());

	CreateDungeon();
	CreateBlock();
	CraeteEnemy();
}

void Stage01::CreateDungeon()
{
	////////////// 던전 오브젝트/////////////////////////////////////////////////
	Sprite* moveBox = Sprite::create("Object/MoveBox.png");
	moveBox->setName("MoveBox");
	moveBox->setPosition(Vec2(DunPos.x - 200, DunPos.y + 28));//2080/240
	this->addChild(moveBox, 3);
	moveBox->runAction(RepeatForever::create(Sequence::create(
		MoveTo::create(2.0f, Vec2(moveBox->getPositionX() + 480, moveBox->getPositionY())),
		MoveTo::create(2.0f, Vec2(moveBox->getPositionX(), moveBox->getPositionY())),
		NULL)));
	m_metabox.pushBack(moveBox);

	Sprite* trap1 = Sprite::create("Object/trap2.png");
	trap1->setAnchorPoint(Vec2(0.5f, 0.5f));
	trap1->setPosition(Vec2(960 + 40, 120));
	trap1->setTag(44);
	this->addChild(trap1, 3);
	m_metabox.pushBack(trap1);

	Sprite* rotTrap1 = Sprite::create("Object/Stop.png");
	rotTrap1->setAnchorPoint(Vec2(0.5f, 0.5f));
	rotTrap1->setPosition(Vec2(4285, 1310));
	rotTrap1->setName("STOP");
	this->addChild(rotTrap1, 3);

	for (int i = 1; i < 4; ++i)
	{
		Sprite* trap = Sprite::createWithSpriteFrame(trap1->getSpriteFrame());
		trap->setAnchorPoint(Vec2(0.5f, 0.5f));
		trap->setTag(44);
		trap->setPosition(Vec2(960 + i * 80 + 40, 120));
		this->addChild(trap, 3);
		m_metabox.pushBack(trap);
	}

	for (int i = 0; i < 4; ++i)
	{
		Sprite* trap = Sprite::createWithSpriteFrame(trap1->getSpriteFrame());
		trap->setAnchorPoint(Vec2(0.5f, 0.5f));
		trap->setTag(44);
		trap->setPosition(Vec2(5200 + i * 80 + 40, 120));
		this->addChild(trap, 3);
		m_metabox.pushBack(trap);
	}

	for (int i = 0; i < 9; ++i)
	{
		Sprite* trap = Sprite::createWithSpriteFrame(trap1->getSpriteFrame());
		trap->setAnchorPoint(Vec2(0.5f, 0.5f));
		trap->setTag(44);
		trap->setPosition(Vec2(1760 + i * 80 + 40, 120));
		this->addChild(trap, 3);
		m_metabox.pushBack(trap);
	}

	/*Sprite* star = Sprite::create("Object/Stage_star.png");
	star->setPosition(Vec2(1600, 480));
	star->setTag(Block::Coin);
	this->addChild(star, 3);
	m_box.pushBack(star);*/

	auto NPC2 = Sprite::create("Hero/NPC2.png");
	NPC2->setPosition(Vec2(1600, 495));
	NPC2->setFlipX(true);
	NPC2->setName("NPC2");
	this->addChild(NPC2, 2);

	//auto NPCspeak = Sprite::create("Hero/Speech1.png");
	//NPCspeak->setPosition(Vec2(10290, 545));
	//NPCspeak->runAction(RepeatForever::create(
	//	Animate::create(effect[4])));
	//this->addChild(NPCspeak, 2);

	Sprite* coin1 = Sprite::create("Object/Coin.png");
	coin1->setPosition(Vec2(1920, 580));
	coin1->setTag(Block::Coin);
	this->addChild(coin1, 3);
	m_box.pushBack(coin1);

	Sprite* coin2 = Sprite::createWithSpriteFrame(coin1->getSpriteFrame());
	coin2->setPosition(Vec2(2320, 580));
	coin2->setTag(Block::Coin);
	this->addChild(coin2, 3);
	m_box.pushBack(coin2);
}

void Stage01::setViewPointCenter(Vec2 pos)
{
	float x = MAX(pos.x, m_winSize.width * 0.5f);
	float y = MAX(pos.y, m_winSize.height * 0.5f);

	x = MIN(x, (m_tmap->getMapSize().width * m_tmap->getTileSize().width) - m_winSize.width * 0.5f);
	y = MIN(y, (m_tmap->getMapSize().height * m_tmap->getTileSize().height) - m_winSize.height * 0.5f);

	Vec2 actualPosition = Vec2(x, y);
	Vec2 centerOfView = Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f);
	Vec2 viewPoint = centerOfView - actualPosition;

	this->setPosition(viewPoint);
}

void Stage01::update(float dt)
{
	static bool hint1;

	////////////// 스테이지 시작 카메라 이벤트
	static bool CamEvent;
	auto cam = dynamic_cast<Sprite*>(this->getChildByName("Cam1"));
	if (m_SaveNum == 1)
	{
		CamEvent = true;
	}
	if (cam->getPositionX() > 500 && CamEvent == false)
	{
		setViewPointCenter(cam->getPosition());
		return;
	}
	else
		CamEvent = true;

	auto NPC2 = dynamic_cast<Sprite*>(this->getChildByName("NPC2"));
	GameUI* pad = (GameUI*)this->getParent()->getChildByTag(100);
	pad->GetInvenInfo(m_Inven);
	pad->GetTimer(m_Timer);
	pad->GetLife(m_Life);
	extern ObjectHero* Hero;
	static bool saveCheck;
	auto save = dynamic_cast<Sprite*>(this->getChildByName("SavePoint"));

	if (saveCheck == false)
	{
		if (Hero->m_Spr->getBoundingBox().intersectsRect(save->getBoundingBox()))
		{
			CSoundManager::getInstance()->playEffect("Sound/Save.mp3", false, 1.0f);
			saveCheck = true;
			save->runAction(Sequence::create(
				Animate::create(effect[7]),
				RemoveSelf::create(),
				NULL
			));
			UserDefault::getInstance()->setIntegerForKey("Save5", 1);
		}
	}
	for (Sprite* trap : m_metabox)
	{
		if (Hero->m_Spr->getBoundingBox().intersectsRect(trap->getBoundingBox()) && trap->getTag() == 44)
		{
			Hero->AnimationSwitch(HeroMotion::Damage);
			this->unscheduleUpdate();
			CameraAction1();
			RUN_WITH_DELAY(3.0f, CC_CALLBACK_0(Stage01::GameOver, this));
			return;
		}
	}

	if (hint1 == false && NPC2->getBoundingBox().intersectsRect(Hero->m_Spr->getBoundingBox()))
	{
		hint1 = true;
		CSoundManager::getInstance()->playEffect("Sound/QuizClear.mp3");
		auto NPCspeak = Sprite::create("Hero/Hint1.png");
		NPCspeak->setPosition(Vec2(NPC2->getPositionX() + 210, NPC2->getPositionY() + 150));
		NPCspeak->runAction(RepeatForever::create(
			Animate::create(effect[5])));
		this->addChild(NPCspeak, 2);
	}

	auto clear = dynamic_cast<Sprite*>(this->getChildByName("ClearPoint"));
	if (Hero->m_Spr->getBoundingBox().intersectsRect(clear->getBoundingBox()) && Hero->m_Spr->getPositionY() + 70 - clear->getPositionY() < 3)
	{
		clear->runAction(MoveTo::create(0.3f, Vec2(clear->getPositionX(), clear->getPositionY() - 320)));
		this->unscheduleUpdate();
		CSoundManager::getInstance()->playEffect("Sound/Game_Clear.mp3");
		Hero->m_spineAni->runAction(Spawn::create(
			MoveTo::create(1.0f, Vec2(clear->getPositionX() - 10, clear->getPositionY() - 130)),
			FadeOut::create(0.6f),
			NULL));
		pad->GameClear();
		RUN_WITH_DELAY(1.1f, CC_CALLBACK_0(Stage01::GameClear, this));
		return;
	}
	//if (Hero->m_Control && Hero->m_Death == false)
	{
		Hero->MoveCharacter(&(pad->m_dir), m_metabox);
		Hero->HeroUpdate();
		ConditionCheck();
		if (pad->m_Inven && Hero->m_Have == false)
			pad->ShowInvenBox();

		m_Timer += dt;
		CheckBoxObject(dt);

		// 지상적 업뎃
		for (int i = 0; i < 3; ++i)
		{
			if (m_Ground[i]->m_death == false)
			{
				m_Ground[i]->EnemyUpdate2();
				if (Hero->m_Spr->getBoundingBox().intersectsRect(m_Ground[i]->m_Spr->getBoundingBox()))
				{
					if (Hero->m_Spr->getPositionY() < m_Ground[i]->m_Spr->getPositionY()+20)
					{
						m_Ground[i]->m_death = true;
						Hero->AnimationSwitch(HeroMotion::Damage);
						this->unscheduleUpdate();
						CameraAction1();
						RUN_WITH_DELAY(3.0f, CC_CALLBACK_0(Stage01::GameOver, this));
					}
					else
					{
						Hero->SmallCallJump();
						m_Ground[i]->m_death = true;
						m_Ground[i]->AnimationSwitch(EnemyMotion::EnemyDeath);
					}
				}
			}
		}
		// 공중적 업뎃
		for (int i = 0; i < 3; ++i)
		{
			if (m_Fly[i]->m_death == false)
			{
				m_Fly[i]->EnemyUpdate2();
				if (Hero->m_Spr->getBoundingBox().intersectsRect(m_Fly[i]->m_Spr->getBoundingBox()))
				{
					if (Hero->m_Spr->getPositionY() < m_Fly[i]->m_Spr->getPositionY())
					{
						m_Fly[i]->m_death = true;
						Hero->AnimationSwitch(HeroMotion::Damage);
						this->unscheduleUpdate();
						CameraAction1();
						RUN_WITH_DELAY(3.0f, CC_CALLBACK_0(Stage01::GameOver, this));
					}
					else
					{
						/////////현재 모바일에서 이부분에서 터짐
						Hero->CallJump();
						m_Fly[i]->m_death = true;
						m_Fly[i]->AnimationSwitch(EnemyMotion::FLYDeath);
					}
				}
			}
		}
		if (m_condition[6] && duncon == false)
		{
			CSoundManager::getInstance()->playEffect("Sound/QuizClear.mp3");
			CSoundManager::getInstance()->playEffect("Sound/Gate_Open.mp3");
			duncon = true;

			auto gate2 = dynamic_cast<Sprite*>(this->getChildByName("Gate2"));
			gate2->runAction(MoveTo::create(1.0f, Vec2(gate2->getPositionX(), gate2->getPositionY() - 241)));
			auto meta4 = dynamic_cast<Sprite*>(this->getChildByName("GateMeta4"));
			meta4->runAction(MoveTo::create(1.0f, Vec2(gate2->getPositionX(), gate2->getPositionY() - 241)));
			auto meta5 = dynamic_cast<Sprite*>(this->getChildByName("GateMeta5"));
			meta5->runAction(MoveTo::create(1.0f, Vec2(gate2->getPositionX(), gate2->getPositionY() - 241)));
			auto meta6 = dynamic_cast<Sprite*>(this->getChildByName("GateMeta6"));
			meta6->runAction(MoveTo::create(1.0f, Vec2(gate2->getPositionX(), gate2->getPositionY() - 241)));
		}

		if (m_EnemyColide == false)
		{
			for (int i = 0; i < 3; ++i)
			{
				m_enemy[i]->EnemyUpdate();
				auto Stone = dynamic_cast<Sprite*>(this->getChildByName("Stone"));
				if (m_enemy[i]->m_death == false)
				{
					if (Stone->getBoundingBox().intersectsRect(m_enemy[i]->m_Spr->getBoundingBox()))
					{
						m_enemy[i]->AnimationSwitch(EnemyMotion::EnemyDeath);
						m_enemy[i]->m_death = true;
					}
					else if (Hero->m_Spr->getBoundingBox().intersectsRect(m_enemy[i]->m_Spr->getBoundingBox()))
					{
						for (int i = 0; i < 3; ++i)
							m_enemy[i]->m_spineAni->stopAllActions();
						m_EnemyColide = true;
						Hero->AnimationSwitch(HeroMotion::Damage);
						this->unscheduleUpdate();
						i = 2;
						CameraAction1();
						RUN_WITH_DELAY(3.0f, CC_CALLBACK_0(Stage01::GameOver, this));
					}
				}
			}
		}
		// 적과의 돌과의 충돌
		// 첫번째 퍼즐(관문열기)////////////////////////////////////////////////
		if (m_condition[0] && m_condition[1])
		{
			CameraFix = true;
			timer1 = m_Timer;
			auto Mgate1 = dynamic_cast<Sprite*>(this->getChildByName("ForCamer1"));
			Mgate1->setPosition(Hero->m_spineAni->getPosition());
		}

		if (CameraFix == false && m_Stage01Event1 == false)
			setViewPointCenter(Hero->m_Spr->getPosition());
		else if (CameraFix)
		{
			if (m_Timer - timer1 < 2.0f)
			{
				//static bool con1;
				auto gate1 = dynamic_cast<Sprite*>(this->getChildByName("Gate1"));
				auto Mgate1 = dynamic_cast<Sprite*>(this->getChildByName("ForCamer1"));
				if (Mgate1->getPositionX() < gate1->getPositionX() && con5 == false)
					Mgate1->setPosition(Vec2(Mgate1->getPositionX() + 20, Mgate1->getPositionY() - 10));
				else
				{
					con5 = true;
					if (m_Timer - timer1 < 1.6f)
					{

					}
					else
					{
						if (Mgate1->getPositionY() < Hero->m_Spr->getPositionY())
							Mgate1->setPositionY(Mgate1->getPositionY() + 8);
						if (Mgate1->getPositionX() > Hero->m_Spr->getPositionX())
							Mgate1->setPositionX(Mgate1->getPositionX() - 23);
					}
				}
				setViewPointCenter(Mgate1->getPosition());
			}
			else
			{
				auto gate1 = dynamic_cast<Sprite*>(this->getChildByName("ForCamer1"));
				CameraFix = false;
			}
		}
		// 미끄럼틀 이벤트
		else if (m_Stage01Event1)
		{
			auto gate1 = dynamic_cast<Sprite*>(this->getChildByName("ForCamer1"));
			if (event2 == false)
			{
				gate1->setPosition(Hero->m_Spr->getPosition());
				event2 = true;
			}
			setViewPointCenter(gate1->getPosition());
			if (gate1->getPositionX() <= 5760)
			{
				gate1->setPositionX(gate1->getPositionX() + 10);
				gate1->setPositionY(gate1->getPositionY() - 1);

			}
			else if (Hero->m_Spr->getPositionX() <= 5760)
			{
				setViewPointCenter(Vec2(5760, Hero->m_Spr->getPositionY()));
			}
			else
			{
				m_Stage01Event1 = false;
			}
		}
		///////////////////////////////////////////////////////////////////////
		// 동전+블록생성
		if (event3)
		{
			auto gate1 = dynamic_cast<Sprite*>(this->getChildByName("ForCamer1"));
			if (event4 == false)
			{
				Vec2 pos = Hero->m_Spr->getPosition();
				event4 = true;
				gate1->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(pos.x, pos.y + 300)),
					NULL));
				timer1 = m_Timer;
			}
			if (m_Timer - timer1 < 1.0f)
				setViewPointCenter(gate1->getPosition());
			else
				event3 = false;
		}
		////////////////////////////////////////////////////////////////
	}
	if (OverCheck == false && Hero->m_Death)
	{
		OverCheck = true;
		CameraAction1();
		RUN_WITH_DELAY(3.0f, CC_CALLBACK_0(Stage01::GameOver, this));
	}
}

// 클리어 부가 조건 체크
void Stage01::ConditionCheck()
{
	extern ObjectHero* Hero;
	float tag = 0;
	if (m_Scale == Block::Change_Right)
	{
		if (scaleBlock->getTag() >= Block::Change6)
		{
			if (once[0] == false)
			{
				auto scaX = dynamic_cast<Sprite*>(this->getChildByName("ScaleX"));
				scaX->runAction(Sequence::create(
					RotateTo::create(1.0f, Vec3(0.0f, 0.0f, 15.0f)),
					NULL));
				auto scaL = dynamic_cast<Sprite*>(this->getChildByName("ScaleLeft"));
				scaL->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(6195, 1885)),
					NULL));
				auto meta1 = dynamic_cast<Sprite*>(this->getChildByName("ScaleMeta1"));
				meta1->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(6075, 1805)),
					NULL));
				auto scaR = dynamic_cast<Sprite*>(this->getChildByName("ScaleRight"));
				scaR->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(7100, 1645)),
					NULL));
				auto meta2 = dynamic_cast<Sprite*>(this->getChildByName("ScaleMeta2"));
				meta2->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(6980, 1565)),
					NULL));
				once[0] = true;
				once[1] = false;
				once[2] = false;
			}
		}
	}
	else if (m_Scale == Block::Change_Left)
	{
		if (scaleBlock->getTag() <= Block::Change4)
		{
			if (once[1] == false)
			{
				auto scaX = dynamic_cast<Sprite*>(this->getChildByName("ScaleX"));
				scaX->runAction(Sequence::create(
					RotateTo::create(1.0f, Vec3(0.0f, 0.0f, -15.0f)),
					NULL));
				auto scaL = dynamic_cast<Sprite*>(this->getChildByName("ScaleLeft"));
				scaL->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(6195, 1645)),
					NULL));
				auto meta1 = dynamic_cast<Sprite*>(this->getChildByName("ScaleMeta1"));
				meta1->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(6075, 1565)),
					NULL));
				auto scaR = dynamic_cast<Sprite*>(this->getChildByName("ScaleRight"));
				scaR->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(7100, 1885)),
					NULL));
				auto meta2 = dynamic_cast<Sprite*>(this->getChildByName("ScaleMeta2"));
				meta2->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(6980, 1805)),
					NULL));
				once[1] = true;
				once[0] = false;
				once[2] = false;
			}
		}
	}
	else if (m_Scale == Block::Change_Same)
	{
		if (scaleBlock->getTag() == Block::Change5)
		{
			if (once[2] == false)
			{
				auto scaX = dynamic_cast<Sprite*>(this->getChildByName("ScaleX"));
				scaX->runAction(Sequence::create(
					RotateTo::create(1.0f, Vec3(0.0f, 0.0f, 0.0f)),
					NULL));
				auto scaL = dynamic_cast<Sprite*>(this->getChildByName("ScaleLeft"));
				scaL->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(6180, 1763)),
					NULL));
				auto meta1 = dynamic_cast<Sprite*>(this->getChildByName("ScaleMeta1"));
				meta1->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(6060, 1683)),
					NULL));
				auto scaR = dynamic_cast<Sprite*>(this->getChildByName("ScaleRight"));
				scaR->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(7118, 1763)),
					NULL));
				auto meta2 = dynamic_cast<Sprite*>(this->getChildByName("ScaleMeta2"));
				meta2->runAction(Sequence::create(
					MoveTo::create(1.0f, Vec2(6998, 1683)),
					NULL));
				once[2] = true;
				once[0] = false;
				once[1] = false;
			}
		}
	}
	// Gate관문
	if (m_condition[0] && m_condition[1])
	{
		CSoundManager::getInstance()->playEffect("Sound/QuizClear.mp3", false, 1.0f);
		CSoundManager::getInstance()->playEffect("Sound/Gate_Open.mp3");

		auto gate1 = dynamic_cast<Sprite*>(this->getChildByName("Gate1"));
		gate1->runAction(MoveTo::create(1.0f, Vec2(2960.0f, 1760.0f)));
		m_condition[0] = false;
		m_condition[1] = false;

		auto Mgate1 = dynamic_cast<Sprite*>(this->getChildByName("GateMeta1"));
		Mgate1->setPositionY(Mgate1->getPositionY() + 80);
		auto Mgate2 = dynamic_cast<Sprite*>(this->getChildByName("GateMeta2"));
		Mgate2->setPositionY(Mgate2->getPositionY() + 160);
		auto Mgate3 = dynamic_cast<Sprite*>(this->getChildByName("GateMeta3"));
		Mgate3->setPositionY(Mgate3->getPositionY() + 240);
	}
	// 마지막관문
	if (m_condition[9])
	{
		m_condition[9] = false;
		auto fgate = dynamic_cast<Sprite*>(this->getChildByName("Final_Gate"));
		fgate->runAction(MoveTo::create(1.0f, Vec2(fgate->getPositionX(), fgate->getPositionY() + 240.0f)));
		CSoundManager::getInstance()->playEffect("Sound/Gate_Open.mp3");
	}
	// 돌멩이 드롭
	if (m_condition[2] && m_condition[3] && stoneOnce == false)
	{
		stoneOnce = true;
		auto stop = dynamic_cast<Sprite*>(this->getChildByName("STOP"));
		stop->setVisible(false);
		m_condition[2] = false;
		m_condition[3] = false;
		auto Stone = dynamic_cast<Sprite*>(this->getChildByName("Stone"));
		Stone->setVisible(true);
		Stone->runAction(Sequence::create(
			MoveTo::create(0.5f, Vec2(Stone->getPositionX(), Stone->getPositionY() - 230)),
			Spawn::create(
				MoveTo::create(2.0f, Vec2(Stone->getPositionX() + 880, Stone->getPositionY() - 1100)),
				RotateBy::create(2.0f, Vec3(0, 0, 390)), NULL),
			MoveTo::create(0.3f, Vec2(Stone->getPositionX() + 880, Stone->getPositionY() - 1140)),
			MoveTo::create(0.3f, Vec2(Stone->getPositionX() + 880, Stone->getPositionY() - 1340)),
			RemoveSelf::create(),
			NULL));
	}
	//미끄럼틀
	if (Hero->m_Spr->getPositionX() >= 4330 && Hero->m_Spr->getPositionX() < 5160 && event1 == false)
	{

		float sy = Hero->m_Spr->getPositionX() - 4330;
		if (Hero->m_Spr->getPositionY() < 1347 - sy)
		{
			Vec2 pos = Hero->m_Spr->getPosition();
			event1 = true;
			float x = 4520 + 640; //5160
			float y = 1585 - 1060; //2645
			Hero->m_Control = false;
			Hero->AnimationSwitch(HeroMotion::Jump);
			float acTime = 2.0f;
			if (sy < 240 && sy >= 0)
				acTime -= 0.1f;
			else if (sy >= 240 && sy < 480)
				acTime -= 0.6f;
			else if (sy >= 480 && sy < 720)
				acTime -= 1.2f;
			else
				acTime -= 1.6f;
			Hero->m_spineAni->setRotationY(0);
			Hero->m_Spr->runAction(Sequence::create(
				MoveTo::create(acTime, Vec2(x, y)),
				CallFunc::create(CC_CALLBACK_0(Stage01::Stage01Event1, this)),
				NULL));
		}
	}

	// 동전 + 벽생성
	if (m_condition[4] && m_condition[5] && event3 == false)
	{
		CSoundManager::getInstance()->playEffect("Sound/QuizClear.mp3");

		m_condition[4] = false;
		m_condition[5] = false;
		event3 = true;
		CreateEventBlock();
		auto gate1 = dynamic_cast<Sprite*>(this->getChildByName("ForCamer1"));
		gate1->setPosition(Vec2(Hero->m_Spr->getPositionX(), Hero->m_Spr->getPositionY()));
	}

	// 저울위 코인생성 이벤트
	if (m_condition[7])
	{
		m_condition[7] = false;
	}

	if (m_condition[8])
	{
		m_condition[8] = false;
	}
}

void Stage01::CreateEventBlock()
{
	Sprite* basic[5] = { 0 };
	Sprite* meta[5] = { 0 };
	Vec2 pos;
	for (int j = 0; j < 2; ++j)
	{
		if (j == 0)
			pos = Vec2(6160.0f, 1060.0f);
		else
			pos = Vec2(6240.0f, 1380.0f);
		for (int i = 0; i < 5; ++i)
		{
			if (i == 0)
			{
				basic[0] = Sprite::create("block/Change_Basic.png");
				meta[0] = Sprite::create("MetaSet.png");
			}
			else
			{
				basic[i] = Sprite::createWithSpriteFrame(basic[0]->getSpriteFrame());
				meta[i] = Sprite::createWithSpriteFrame(meta[0]->getSpriteFrame());
			}
			basic[i]->setAnchorPoint(Vec2(0.0f, 1.0f));
			basic[i]->setPosition(Vec2(pos.x - (80.0f*i), pos.y));
			this->addChild(basic[i], 3);
			meta[i]->setAnchorPoint(Vec2(0.0f, 1.0f));
			meta[i]->setPosition(Vec2(pos.x - (80.0f*i), pos.y - 20.0f));
			meta[i]->setVisible(ColiideRect);
			this->addChild(meta[i], 5);
			m_metabox.pushBack(meta[i]);
		}
	}
	for (int i = 0; i < 2; ++i)
	{
		if (i == 0)
			pos = Vec2(6160.0f, 1060.0f);
		else
			pos = Vec2(6240.0f, 1380.0f);
		auto Coin1 = Sprite::create("Object/Coin.png");
		Coin1->setTag(Block::Coin);
		Coin1->setAnchorPoint(Vec2(0.0f, 1.0f));
		Coin1->setPosition(Vec2(pos.x, pos.y + 150));
		this->addChild(Coin1, 3);
		m_box.pushBack(Coin1);
		auto Coin2 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
		Coin2->setTag(Block::Coin);
		Coin2->setAnchorPoint(Vec2(0.0f, 1.0f));
		Coin2->setPosition(Vec2(pos.x - 100, pos.y + 150));
		this->addChild(Coin2, 3);
		m_box.pushBack(Coin2);
		auto Coin3 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
		Coin3->setTag(Block::Coin);
		Coin3->setAnchorPoint(Vec2(0.0f, 1.0f));
		Coin3->setPosition(Vec2(pos.x - 200, pos.y + 150));
		this->addChild(Coin3, 3);
		m_box.pushBack(Coin3);
		auto Coin4 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
		Coin4->setTag(Block::Coin);
		Coin4->setAnchorPoint(Vec2(0.0f, 1.0f));
		Coin4->setPosition(Vec2(pos.x - 300, pos.y + 150));
		this->addChild(Coin4, 3);
		m_box.pushBack(Coin4);
	}
}

void Stage01::Stage01Event1()
{
	extern ObjectHero* Hero;
	Hero->m_Death = false;
	Hero->m_Control = true;
	m_Stage01Event1 = true;
}

// 게임오버
void Stage01::GameOver()
{
	m_Life -= 1;
	UserDefault::getInstance()->setIntegerForKey("MathLife", m_Life);
	if (m_Life != 0)
	{
		GameUI* pad = (GameUI*)this->getParent()->getChildByTag(100);
		pad->GetLife(m_Life);
		this->unscheduleUpdate();
		this->stopAllActions();
		Scene* s = TransitionFade::create(3.0f, Stage01::createScene());
		Director::getInstance()->replaceScene(s);
	}
	else
	{
		CSoundManager::getInstance()->playEffect("Sound/Game_Over.mp3");
		GameUI* pad = (GameUI*)this->getParent()->getChildByTag(100);
		pad->GetLife(m_Life);
		pad->GameOver();
		UserDefault::getInstance()->setIntegerForKey("MathLife", 3);
		RUN_WITH_DELAY(1.0f, CC_CALLBACK_0(Stage01::GameClear, this));
	}
}

void Stage01::GameClear()
{
	extern ObjectHero* Hero;
	Hero->StageClear = true;
	CSoundManager::getInstance()->stopBgm("Sound/bgm_tutorial.mp3");
	this->unscheduleUpdate();
	this->stopAllActions();
	Scene* s = TransitionFade::create(3.0f, IntroScene::createScene());
	Director::getInstance()->replaceScene(s);
}

void Stage01::CreateBlock()
{
	//코인1
	auto Coin1 = Sprite::create("Object/Coin.png");
	Coin1->setTag(Block::Coin);
	Coin1->setAnchorPoint(Vec2(0.0f, 1.0f));
	Coin1->setPosition(Vec2(910.0f, 1280.0f));
	this->addChild(Coin1, 3);
	m_box.pushBack(Coin1);

	//코인2
	auto Coin2 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	Coin2->setTag(Block::Coin);
	Coin2->setAnchorPoint(Vec2(0.0f, 1.0f));
	Coin2->setPosition(Vec2(1030.0f, 1400.0f));
	this->addChild(Coin2, 3);
	m_box.pushBack(Coin2);
	
	//코인3
	auto Coin3 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	Coin3->setTag(Block::Coin);
	Coin3->setAnchorPoint(Vec2(0.0f, 1.0f));
	Coin3->setPosition(Vec2(1130.0f, 1400.0f));
	this->addChild(Coin3, 3);
	m_box.pushBack(Coin3);

	//코인4
	auto Coin4 = Sprite::create("Object/Coin.png");
	Coin4->setTag(Block::Coin);
	Coin4->setAnchorPoint(Vec2(0.0f, 1.0f));
	Coin4->setPosition(Vec2(1210.0f, 1280.0f));
	this->addChild(Coin4, 3);
	m_box.pushBack(Coin4);

	/*void Camera3DTestDemo::scaleCameraCallback(Ref* sender, float value)
	{
		if (_camera&& _cameraType != CameraType::FirstPerson)
		{
			Vec3 cameraPos = _camera->getPosition3D();
			cameraPos += cameraPos.getNormalized()*value;
			_camera->setPosition3D(cameraPos);
		}
	}*/

	auto block = Sprite::create("block/BreakBlock.png");
	block->setTag(Block::Break);
	block->setAnchorPoint(Vec2(0.0f, 1.0f));
	block->setPosition(Vec2(480.0f, 1360.0f));
	this->addChild(block, 3);
	m_box.pushBack(block);
	m_metabox.pushBack(block);

	auto get6 = Sprite::create("block/Get_Block_6.png");
	get6->setTag(Block::Get6);
	get6->setAnchorPoint(Vec2(0.0f, 1.0f));
	get6->setPosition(Vec2(560.0f, 1360.0f));
	this->addChild(get6, 3);
	m_box.pushBack(get6);

	auto block2 = Sprite::createWithSpriteFrame(block->getSpriteFrame());
	block2->setTag(Block::Break);
	block2->setAnchorPoint(Vec2(0.0f, 1.0f));
	block2->setPosition(Vec2(640.0f, 1360.0f));
	this->addChild(block2, 3);
	m_box.pushBack(block2);
	m_metabox.pushBack(block2);

	//////////////////////////////////////////////////////////////////////////
	auto blockNum1 = Sprite::create("block/FIX_1.png");
	blockNum1->setAnchorPoint(Vec2(0.0f, 1.0f));
	blockNum1->setPosition(Vec2(1920.0f, 1925.0f));
	this->addChild(blockNum1, 3);

	auto get2 = Sprite::create("block/FIX_2.png");
	get2->setTag(Block::Get2);
	get2->setAnchorPoint(Vec2(0.0f, 1.0f));
	get2->setPosition(Vec2(2000.0f, 1925.0f));
	this->addChild(get2, 3);

	auto Plusblock = Sprite::create("block/FIX_PLUS.png");
	Plusblock->setAnchorPoint(Vec2(0.0f, 1.0f));
	Plusblock->setPosition(Vec2(2080.0f, 1925.0f));
	this->addChild(Plusblock, 3);

	auto Tblock = Sprite::create("block/Change_0.png");
	Tblock->setTag(Block::Change0);
	Tblock->setAnchorPoint(Vec2(0.0f, 1.0f));
	Tblock->setPosition(Vec2(2163.0f, 1941.0f));
	this->addChild(Tblock, 3);
	m_ChangeBox.pushBack(Tblock);

	auto Sameblock = Sprite::create("block/FIX_SAME.png");
	Sameblock->setAnchorPoint(Vec2(0.0f, 1.0f));
	Sameblock->setPosition(Vec2(2237.0f, 1925.0f));
	this->addChild(Sameblock, 3);

	auto blockNum1_2 = Sprite::create("block/FIX_1.png");
	blockNum1_2->setAnchorPoint(Vec2(0.0f, 1.0f));
	blockNum1_2->setPosition(Vec2(2317.0f, 1925.0f));
	this->addChild(blockNum1_2, 3);

	auto blockAnswer = Sprite::create("block/InteractBlock.png");
	blockAnswer->setAnchorPoint(Vec2(0.0f, 1.0f));
	blockAnswer->setPosition(Vec2(2400.0f, 1920.0f));
	blockAnswer->setVisible(true);
	blockAnswer->setTag(Block::Setting);
	m_box.pushBack(blockAnswer);
	this->addChild(blockAnswer, 3);

	auto gate1 = Sprite::create("Object/gate.png");
	gate1->setAnchorPoint(Vec2(0.0f, 1.0f));
	gate1->setPosition(Vec2(2960.0f, 1520.0f));
	gate1->setName("Gate1");
	this->addChild(gate1, -1);

	auto meta1 = Sprite::create("MetaSet.png");
	meta1->setAnchorPoint(Vec2(0.0f, 1.0f));
	meta1->setPosition(Vec2(2960.0f, 1520.0f));
	meta1->setName("GateMeta1");
	meta1->setVisible(ColiideRect);
	this->addChild(meta1, 5);
	m_metabox.pushBack(meta1);

	auto ForCamer1 = Sprite::createWithSpriteFrame(meta1->getSpriteFrame());
	ForCamer1->setAnchorPoint(Vec2(0.0f, 1.0f));
	ForCamer1->setPosition(Vec2(2960.0f, 1520.0f));
	ForCamer1->setName("ForCamer1");
	ForCamer1->setVisible(false);
	this->addChild(ForCamer1, 5);

	auto meta2 = Sprite::createWithSpriteFrame(meta1->getSpriteFrame());
	meta2->setAnchorPoint(Vec2(0.0f, 1.0f));
	meta2->setPosition(Vec2(2960.0f, 1440.0f));
	meta2->setName("GateMeta2");
	meta2->setVisible(ColiideRect);
	this->addChild(meta2, 5);
	m_metabox.pushBack(meta2);

	auto meta3 = Sprite::createWithSpriteFrame(meta1->getSpriteFrame());
	meta3->setAnchorPoint(Vec2(0.0f, 1.0f));
	meta3->setPosition(Vec2(2960.0f, 1360.0f));
	meta3->setName("GateMeta3");
	meta3->setVisible(ColiideRect);
	this->addChild(meta3, 5);
	m_metabox.pushBack(meta3);

	auto TblockZ = Sprite::create("block/Change_Z.png");
	TblockZ->setTag(Block::ChangeZ);
	TblockZ->setAnchorPoint(Vec2(0.0f, 1.0f));
	TblockZ->setPosition(Vec2(3600.0f, 1620.0f));
	this->addChild(TblockZ, 3);
	m_ChangeBox.pushBack(TblockZ);

	auto blockAnswer3 = Sprite::createWithSpriteFrame(block->getSpriteFrame());
	blockAnswer3->setAnchorPoint(Vec2(0.0f, 1.0f));
	blockAnswer3->setPosition(Vec2(3800.0f, 1600.0f));
	blockAnswer3->setVisible(true);
	blockAnswer3->setTag(Block::Break);
	m_box.pushBack(blockAnswer3);
	this->addChild(blockAnswer3, 3);

	auto FIX2 = Sprite::create("block/FIX_2.png");
	FIX2->setAnchorPoint(Vec2(0.0f, 1.0f));
	FIX2->setPosition(Vec2(3677.0f, 1685.0f));
	this->addChild(FIX2, 3);

	/*auto Plusbloc2 = Sprite::create("block/FIX_PLUS.png");
	Plusbloc2->setAnchorPoint(Vec2(0.0f, 1.0f));
	Plusbloc2->setPosition(Vec2(3837.0f, 1605.0f));
	this->addChild(Plusbloc2, 3);*/

	auto FIXY = Sprite::create("block/FIX_Y.png");
	FIXY->setAnchorPoint(Vec2(0.0f, 1.0f));
	FIXY->setPosition(Vec2(3917.0f, 1605.0f));
	this->addChild(FIXY, 3);

	/*auto Square1 = Sprite::create("block/FIX_Square.png");
	Square1->setAnchorPoint(Vec2(0.0f, 1.0f));
	Square1->setPosition(Vec2(3995.0f, 1605.0f));
	this->addChild(Square1, 3);*/

	auto blockAnswer2 = Sprite::createWithSpriteFrame(blockAnswer->getSpriteFrame());
	blockAnswer2->setAnchorPoint(Vec2(0.0f, 1.0f));
	blockAnswer2->setPosition(Vec2(4000.0f, 1680.0f));
	blockAnswer2->setVisible(true);
	blockAnswer2->setTag(Block::Setting);
	m_box.pushBack(blockAnswer2);
	this->addChild(blockAnswer2, 3);

	auto meta_a = Sprite::createWithSpriteFrame(meta1->getSpriteFrame());
	meta_a->setAnchorPoint(Vec2(0.0f, 1.0f));
	meta_a->setPosition(Vec2(3800.0f, 1600.0f));
	meta_a->setVisible(ColiideRect);
	this->addChild(meta_a, 5);
	m_metabox.pushBack(meta_a);


	auto Same1 = Sprite::create("block/FIX_SAME.png");
	Same1->setAnchorPoint(Vec2(0.0f, 1.0f));
	Same1->setPosition(Vec2(4155.0f, 1605.0f));
	this->addChild(Same1, 3);

	auto get2_2 = Sprite::create("block/Get_Block_2.png");
	get2_2->setTag(Block::Get2);
	get2_2->setAnchorPoint(Vec2(0.0f, 1.0f));
	get2_2->setPosition(Vec2(3830.0f, 1240.0f));
	this->addChild(get2_2, 3);
	m_box.pushBack(get2_2);

	auto GetSqu = Sprite::create("block/Get_Block_Plus.png");
	GetSqu->setTag(Block::GetPlus);
	GetSqu->setAnchorPoint(Vec2(0.0f, 1.0f));
	GetSqu->setPosition(Vec2(3910.0f, 1240.0f));
	this->addChild(GetSqu, 3);
	m_box.pushBack(GetSqu);

	auto xline = Sprite::create("Xline2.png");
	xline->setAnchorPoint(Vec2(0.0f, 1.0f));
	xline->setPosition(Vec2(4240.0f, 1595.0f));
	this->addChild(xline, 3);

	auto Yline = Sprite::create("Yline2.png");
	Yline->setAnchorPoint(Vec2(0.0f, 1.0f));
	Yline->setPosition(Vec2(4490.0f, 1865.0f));
	this->addChild(Yline, 3);

	auto Stone = Sprite::create("Object/Stone.png");
	Stone->setAnchorPoint(Vec2(0.5f, 0.5f));
	Stone->setPosition(Vec2(4520.0f, 1585.0f));
	Stone->setName("Stone");
	Stone->setVisible(false);
	this->addChild(Stone, 3);

	//코인5
	auto Coin5 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	Coin5->setTag(Block::Coin);
	Coin5->setAnchorPoint(Vec2(0.0f, 1.0f));
	Coin5->setPosition(Vec2(3780.0f, 1840.0f));
	this->addChild(Coin5, 3);
	m_box.pushBack(Coin5);

	//코인6
	auto Coin6 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	Coin6->setTag(Block::Coin);
	Coin6->setAnchorPoint(Vec2(0.0f, 1.0f));
	Coin6->setPosition(Vec2(3900.0f, 1840.0f));
	this->addChild(Coin6, 3);
	m_box.pushBack(Coin6);

	//코인7
	auto Coin7 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	Coin7->setTag(Block::Coin);
	Coin7->setAnchorPoint(Vec2(0.0f, 1.0f));
	Coin7->setPosition(Vec2(4010.0f, 1840.0f));
	this->addChild(Coin7, 3);
	m_box.pushBack(Coin7);

	//코인8
	auto Coin8 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	Coin8->setTag(Block::Coin);
	Coin8->setAnchorPoint(Vec2(0.0f, 1.0f));
	Coin8->setPosition(Vec2(4120.0f, 1840.0f));
	this->addChild(Coin8, 3);
	m_box.pushBack(Coin8);

	//지한던전블록//////////////////////////////////////////////////////////////
	auto Dun_FIX2 = Sprite::create("block/FIX_2.png");
	Dun_FIX2->setAnchorPoint(Vec2(0.0f, 1.0f));
	Dun_FIX2->setPosition(Vec2(2880.0f, 1210.0f));
	this->addChild(Dun_FIX2, 3);

	auto Dun_FIXP = Sprite::create("block/FIX_PLUS.png");
	Dun_FIXP->setAnchorPoint(Vec2(0.0f, 1.0f));
	Dun_FIXP->setPosition(Vec2(2960.0f, 1210.0f));
	this->addChild(Dun_FIXP, 3);

	auto Dun_FIX3 = Sprite::create("block/FIX_3.png");
	Dun_FIX3->setAnchorPoint(Vec2(0.0f, 1.0f));
	Dun_FIX3->setPosition(Vec2(3040.0f, 1210.0f));
	this->addChild(Dun_FIX3, 3);

	auto Dun_FIXS = Sprite::create("block/FIX_SAME.png");
	Dun_FIXS->setAnchorPoint(Vec2(0.0f, 1.0f));
	Dun_FIXS->setPosition(Vec2(3120.0f, 1210.0f));
	this->addChild(Dun_FIXS, 3);

	auto Dun_C0 = Sprite::create("block/Change_0.png");
	Dun_C0->setTag(Block::Change0);
	Dun_C0->setAnchorPoint(Vec2(0.0f, 1.0f));
	Dun_C0->setPosition(Vec2(3200.0f, 1223.0f));//3200
	this->addChild(Dun_C0, 3);
	m_ChangeBox.pushBack(Dun_C0);

	auto gate2 = Sprite::createWithSpriteFrame(gate1->getSpriteFrame());
	gate2->setAnchorPoint(Vec2(0.0f, 1.0f));
	gate2->setPosition(Vec2(3120.0f, 1120.0f));
	gate2->setName("Gate2");
	this->addChild(gate2, -1);

	auto meta4 = Sprite::createWithSpriteFrame(meta1->getSpriteFrame());
	meta4->setAnchorPoint(Vec2(0.0f, 1.0f));
	meta4->setPosition(Vec2(3120.0f, 1120.0f));
	meta4->setName("GateMeta4");
	meta4->setVisible(ColiideRect);
	this->addChild(meta4, 5);
	m_metabox.pushBack(meta4);

	auto meta5 = Sprite::createWithSpriteFrame(meta1->getSpriteFrame());
	meta5->setAnchorPoint(Vec2(0.0f, 1.0f));
	meta5->setPosition(Vec2(3120.0f, 1040.0f));
	meta5->setName("GateMeta5");
	meta5->setVisible(ColiideRect);
	this->addChild(meta5, 5);
	m_metabox.pushBack(meta5);

	auto meta6 = Sprite::createWithSpriteFrame(meta1->getSpriteFrame());
	meta6->setAnchorPoint(Vec2(0.0f, 1.0f));
	meta6->setPosition(Vec2(3120.0f, 960.0f));
	meta6->setName("GateMeta6");
	meta6->setVisible(ColiideRect);
	this->addChild(meta6, 5);
	m_metabox.pushBack(meta6);

	auto Coin9 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	Coin9->setTag(Block::Coin);
	Coin9->setAnchorPoint(Vec2(0.0f, 1.0f));
	Coin9->setPosition(Vec2(2660.0f, 340.0f));
	this->addChild(Coin9, 3);
	m_box.pushBack(Coin9);

	auto Coin10 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	Coin10->setTag(Block::Coin);
	Coin10->setAnchorPoint(Vec2(0.0f, 1.0f));
	Coin10->setPosition(Vec2(2760.0f, 340.0f));
	this->addChild(Coin10, 3);
	m_box.pushBack(Coin10);

	/////////////////////////////////////////////////////////////////////////////
	auto FIX2_2 = Sprite::create("block/FIX_2.png");
	FIX2_2->setAnchorPoint(Vec2(0.0f, 1.0f));
	FIX2_2->setPosition(Vec2(5757.0f, 725.0f));
	this->addChild(FIX2_2, 3);

	auto Chnage_Mul = Sprite::create("block/Change_Mul.png");
	Chnage_Mul->setTag(Block::Change_Mul);
	Chnage_Mul->setAnchorPoint(Vec2(0.0f, 1.0f));
	Chnage_Mul->setPosition(Vec2(5840.0f, 740.0f));
	this->addChild(Chnage_Mul, 3);
	m_ChangeBox.pushBack(Chnage_Mul);
	m_condition[4] = true;

	auto blockAnswer4 = Sprite::createWithSpriteFrame(block->getSpriteFrame());
	blockAnswer4->setAnchorPoint(Vec2(0.0f, 1.0f));
	blockAnswer4->setPosition(Vec2(5920.0f, 720.0f));
	blockAnswer4->setVisible(true);
	blockAnswer4->setTag(Block::Break);
	m_box.pushBack(blockAnswer4);
	this->addChild(blockAnswer4, 3);

	auto FIX_SAME = Sprite::create("block/FIX_SAME.png");
	FIX_SAME->setAnchorPoint(Vec2(0.0f, 1.0f));
	FIX_SAME->setPosition(Vec2(5997.0f, 725.0f));
	this->addChild(FIX_SAME, 3);

	auto FIX1_1 = Sprite::create("block/FIX_1.png");
	FIX1_1->setAnchorPoint(Vec2(0.0f, 1.0f));
	FIX1_1->setPosition(Vec2(6077.0f, 725.0f));
	this->addChild(FIX1_1, 3);

	auto FIX0 = Sprite::create("block/FIX_0.png");
	FIX0->setAnchorPoint(Vec2(0.0f, 1.0f));
	FIX0->setPosition(Vec2(6157.0f, 725.0f));
	this->addChild(FIX0, 3);

	auto get_7 = Sprite::create("block/Get_Block_7.png");
	get_7->setTag(Block::Get7);
	get_7->setAnchorPoint(Vec2(0.0f, 1.0f));
	get_7->setPosition(Vec2(6480, 1100.0f));
	this->addChild(get_7, 3);
	m_box.pushBack(get_7);

	auto get_5 = Sprite::create("block/Get_Block_5.png");
	get_5->setTag(Block::Get5);
	get_5->setAnchorPoint(Vec2(0.0f, 1.0f));
	get_5->setPosition(Vec2(6600, 1020.0f));
	this->addChild(get_5, 3);
	m_box.pushBack(get_5);

	auto get_m = Sprite::create("block/Get_Block_Minus.png");
	get_m->setTag(Block::GetMinus);
	get_m->setAnchorPoint(Vec2(0.0f, 1.0f));
	get_m->setPosition(Vec2(6760, 920.0f));
	this->addChild(get_m, 3);
	m_box.pushBack(get_m);

	auto FIX5 = Sprite::create("block/FIX_5.png");
	FIX5->setAnchorPoint(Vec2(0.0f, 1.0f));
	FIX5->setPosition(Vec2(6558.0f, 1685.0f));
	this->addChild(FIX5, 3);

	auto ChnageBS = Sprite::create("block/Change_Right.png");
	ChnageBS->setTag(Block::Change_Right);
	ChnageBS->setAnchorPoint(Vec2(0.0f, 1.0f));
	ChnageBS->setPosition(Vec2(6641.0f, 1700.0f));
	this->addChild(ChnageBS, 3);
	m_ChangeBox.pushBack(ChnageBS);

	scaleBlock = Sprite::create("block/Change_7.png");
	scaleBlock->setTag(Block::Change7);
	scaleBlock->setAnchorPoint(Vec2(0.0f, 1.0f));
	scaleBlock->setPosition(Vec2(6720.0f, 1700.0f));
	this->addChild(scaleBlock, 3);
	m_ChangeBox.pushBack(scaleBlock);

	auto ScaleM = Sprite::create("Object/ScaleY.png");
	ScaleM->setAnchorPoint(Vec2(0.0f, 1.0f));
	ScaleM->setPosition(Vec2(6648.0f, 1950.0f));
	this->addChild(ScaleM, 2);

	auto ScaleX = Sprite::create("Object/ScaleX.png");
	ScaleX->setAnchorPoint(Vec2(0.5f, 0.5f));
	ScaleX->setName("ScaleX");
	ScaleX->runAction(
		RotateTo::create(0.0f, Vec3(0.0f, 0.0f, 15.0f)));
	ScaleX->setPosition(Vec2(6648.0f, 1925.0f));
	this->addChild(ScaleX, 1);

	auto ScaleL = Sprite::create("Object/Scale1.png");
	ScaleL->setName("ScaleLeft");
	ScaleL->setPosition(Vec2(6195.0f, 1885.0f));//6195 1885
	this->addChild(ScaleL, 3);

	auto meta7 = Sprite::create("TileMap/MetaSet_2.png");
	meta7->setAnchorPoint(Vec2(0.0f, 1.0f));
	meta7->setPosition(Vec2(6075.0f, 1805.0f));
	meta7->setName("ScaleMeta1");
	meta7->setVisible(ColiideRect);
	this->addChild(meta7, 5);
	m_metabox.pushBack(meta7);

	auto ScaleR = Sprite::createWithSpriteFrame(ScaleL->getSpriteFrame());
	ScaleR->setName("ScaleRight");
	ScaleR->setPosition(Vec2(7100.0f, 1645.0f));//7100 1645
	this->addChild(ScaleR, 3);

	auto meta8 = Sprite::createWithSpriteFrame(meta7->getSpriteFrame());
	meta8->setAnchorPoint(Vec2(0.0f, 1.0f));
	meta8->setPosition(Vec2(6980, 1565));
	meta8->setName("ScaleMeta2");
	meta8->setVisible(ColiideRect);
	this->addChild(meta8, 5);
	m_metabox.pushBack(meta8);

	Sprite* trap = Sprite::create("Object/trap2.png");
	trap->setAnchorPoint(Vec2(0.5f, 0.5f));
	trap->setTag(44);
	trap->setPosition(Vec2(6920, 120));
	this->addChild(trap, 3);
	m_metabox.pushBack(trap);

	for (int i = 1; i < 27; ++i)
	{
		Sprite* trap2 = Sprite::createWithSpriteFrame(trap->getSpriteFrame());
		trap2->setAnchorPoint(Vec2(0.5f, 0.5f));
		trap2->setTag(44);
		trap2->setPosition(Vec2(6880 + i * 80 + 40, 120));
		this->addChild(trap2, 3);
		m_metabox.pushBack(trap2);
	}

	//////////////////////////////////////////////////////////////////////////
	// 저울위 블록
	auto fix_top1 = Sprite::create("block/FIX_5.png");
	fix_top1->setAnchorPoint(Vec2(0.0f, 1.0f));
	fix_top1->setPosition(Vec2(5436, 2165));
	this->addChild(fix_top1, 3);

	auto fix_top0 = Sprite::create("block/FIX_0.png");
	fix_top0->setAnchorPoint(Vec2(0.0f, 1.0f));
	fix_top0->setPosition(Vec2(5516, 2165));
	this->addChild(fix_top0, 3);

	auto Chnage_Mul2 = Sprite::create("block/Change_Mul.png");
	Chnage_Mul2->setTag(Block::Change_Mul);
	Chnage_Mul2->setAnchorPoint(Vec2(0.0f, 1.0f));
	Chnage_Mul2->setPosition(Vec2(5600, 2180));
	this->addChild(Chnage_Mul2, 3);
	m_ChangeBox.pushBack(Chnage_Mul2);

	auto fix_top3 = Sprite::create("block/FIX_SAME.png");
	fix_top3->setAnchorPoint(Vec2(0.0f, 1.0f));
	fix_top3->setPosition(Vec2(5756, 2165));
	this->addChild(fix_top3, 3);

	auto fix_top4 = Sprite::create("block/Get_Coin.png");
	fix_top4->setAnchorPoint(Vec2(0.0f, 1.0f));
	fix_top4->setPosition(Vec2(5840, 2160));
	this->addChild(fix_top4, 3);

	auto blockAnswer5 = Sprite::createWithSpriteFrame(block->getSpriteFrame());
	blockAnswer5->setAnchorPoint(Vec2(0.0f, 1.0f));
	blockAnswer5->setPosition(Vec2(5680, 2160));
	blockAnswer5->setVisible(true);
	blockAnswer5->setTag(Block::Break);
	m_box.pushBack(blockAnswer5);
	this->addChild(blockAnswer5, 3);

	// Y = - SINX
	auto fix_top5 = Sprite::create("block/FIX_Y.png");
	fix_top5->setAnchorPoint(Vec2(0.0f, 1.0f));
	fix_top5->setPosition(Vec2(6796, 2245));
	this->addChild(fix_top5, 3);

	auto fix_top6 = Sprite::create("block/FIX_SAME.png");
	fix_top6->setAnchorPoint(Vec2(0.0f, 1.0f));
	fix_top6->setPosition(Vec2(6876, 2245));
	this->addChild(fix_top6, 3);

	auto top_answer = Sprite::createWithSpriteFrame(blockAnswer->getSpriteFrame());
	top_answer->setAnchorPoint(Vec2(0.0f, 1.0f));
	top_answer->setPosition(Vec2(6960, 2240));
	top_answer->setVisible(true);
	top_answer->setTag(Block::Setting);
	m_box.pushBack(top_answer);
	this->addChild(top_answer, 3);

	auto fix_top7 = Sprite::create("block/FIX_S.png");
	fix_top7->setAnchorPoint(Vec2(0.0f, 1.0f));
	fix_top7->setPosition(Vec2(7037, 2245));
	this->addChild(fix_top7, 3);

	auto fix_topI = Sprite::create("block/FIX_I.png");
	fix_topI->setAnchorPoint(Vec2(0.0f, 1.0f));
	fix_topI->setPosition(Vec2(7117, 2245));
	this->addChild(fix_topI, 3);

	auto fix_topN = Sprite::create("block/FIX_N.png");
	fix_topN->setAnchorPoint(Vec2(0.0f, 1.0f));
	fix_topN->setPosition(Vec2(7197, 2245));
	this->addChild(fix_topN, 3);

	auto fix_topx = Sprite::create("block/FIX_Multi.png");
	fix_topx->setAnchorPoint(Vec2(0.0f, 1.0f));
	fix_topx->setPosition(Vec2(7277, 2245));
	this->addChild(fix_topx, 3);

	auto fx = Sprite::create("block/Change_fx.png");
	fx->setAnchorPoint(Vec2(0.0f, 1.0f));
	fx->setPosition(Vec2(7280, 1320));
	fx->setName("FX");
	this->addChild(fx, 3);

	auto fxmeta = Sprite::createWithSpriteFrame(meta1->getSpriteFrame());
	fxmeta->setAnchorPoint(Vec2(0.0f, 1.0f));
	fxmeta->setPosition(Vec2(7280, 1300));
	fxmeta->setName("FXmeta");
	fxmeta->setVisible(ColiideRect);
	this->addChild(fxmeta, 4);
	m_metabox.pushBack(fxmeta);

	auto fx_xline = Sprite::create("Object/LineX2.png");
	fx_xline->setAnchorPoint(Vec2(0.0f, 1.0f));
	fx_xline->setPosition(Vec2(7280, 1300));
	this->addChild(fx_xline, 2);

	auto fx_yline = Sprite::create("Object/LineY3.png");
	fx_yline->setAnchorPoint(Vec2(0.0f, 1.0f));
	fx_yline->setPosition(Vec2(7280, 1730));
	this->addChild(fx_yline, 2);

	auto fx_SIN = Sprite::create("Object/sin.png");
	fx_SIN->setAnchorPoint(Vec2(0.0f, 0.5f));
	fx_SIN->setPosition(Vec2(7280, 1300));
	fx_SIN->setVisible(false);
	fx_SIN->setName("SIN");
	this->addChild(fx_SIN, 1);

	Sprite* fx_coin1 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fx_coin1->setPosition(Vec2(7480, 1260));
	fx_coin1->setTag(Block::Coin);
	this->addChild(fx_coin1, 3);
	m_box.pushBack(fx_coin1);

	Sprite* fx_coin2 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fx_coin2->setPosition(Vec2(7580, 1260));
	fx_coin2->setTag(Block::Coin);
	this->addChild(fx_coin2, 3);
	m_box.pushBack(fx_coin2);

	Sprite* fx_coin3 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fx_coin3->setPosition(Vec2(8600, 1260));
	fx_coin3->setTag(Block::Coin);
	this->addChild(fx_coin3, 3);
	m_box.pushBack(fx_coin3);

	Sprite* fx_coin4 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fx_coin4->setPosition(Vec2(8700, 1260));
	fx_coin4->setTag(Block::Coin);
	this->addChild(fx_coin4, 3);
	m_box.pushBack(fx_coin4);

	Sprite* fx_coin5 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fx_coin5->setPosition(Vec2(8040, 1820));
	fx_coin5->setTag(Block::Coin);
	this->addChild(fx_coin5, 3);
	m_box.pushBack(fx_coin5);

	Sprite* fx_coin6 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fx_coin6->setPosition(Vec2(8140, 1820));
	fx_coin6->setTag(Block::Coin);
	this->addChild(fx_coin6, 3);
	m_box.pushBack(fx_coin6);

	//// 성 입구 마지막 퍼즐
	auto final_gate = Sprite::createWithSpriteFrame(gate1->getSpriteFrame());
	final_gate->setAnchorPoint(Vec2(0.0f, 1.0f));
	final_gate->setPosition(Vec2(9280, 1040));
	final_gate->setName("Final_Gate");
	this->addChild(final_gate, -1);
	m_metabox.pushBack(final_gate);

	auto get_f3 = Sprite::create("block/Get_Block_3.png");
	get_f3->setTag(Block::Get3);
	get_f3->setAnchorPoint(Vec2(0.0f, 1.0f));
	get_f3->setPosition(Vec2(8600, 1520));
	this->addChild(get_f3, 3);
	m_box.pushBack(get_f3);

	auto Final_A = Sprite::createWithSpriteFrame(blockAnswer->getSpriteFrame());
	Final_A->setAnchorPoint(Vec2(0.0f, 1.0f));
	Final_A->setPosition(Vec2(9200, 1120));
	Final_A->setVisible(true);
	Final_A->setTag(Block::Setting);
	this->addChild(Final_A, 3);
	m_box.pushBack(Final_A);

	auto Final_Fix1 = Sprite::create("block/FIX_SAME.png");
	Final_Fix1->setAnchorPoint(Vec2(0.0f, 1.0f));
	Final_Fix1->setPosition(Vec2(9275, 1128));
	this->addChild(Final_Fix1, 3);

	auto Final_Fix2 = Sprite::create("block/FIX_3.png");
	Final_Fix2->setAnchorPoint(Vec2(0.0f, 1.0f));
	Final_Fix2->setPosition(Vec2(9355, 1128));
	this->addChild(Final_Fix2, 3);

	auto Clear = Sprite::create("Object/FinalDoor.png");
	Clear->setAnchorPoint(Vec2(0.0f, 1.0f));
	Clear->setPosition(Vec2(9360, 400));
	this->addChild(Clear, 1);

	auto black = Sprite::create("Object/Black.png");
	black->setAnchorPoint(Vec2(0.0f, 1.0f));
	black->setPosition(Vec2(9440, 320));
	this->addChild(black, 0);


	auto ClearB = Sprite::create("Object/FinalGate.png");
	ClearB->setAnchorPoint(Vec2(0.0f, 1.0f));
	ClearB->setPosition(Vec2(9440, 320));
	ClearB->setName("ClearPoint");
	this->addChild(ClearB, 2);



	auto fcoin1 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fcoin1->setTag(Block::Coin);
	fcoin1->setAnchorPoint(Vec2(0.0f, 1.0f));
	fcoin1->setPosition(Vec2(10050, 720));
	this->addChild(fcoin1, 3);
	m_box.pushBack(fcoin1);

	auto fcoin3 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fcoin3->setTag(Block::Coin);
	fcoin3->setAnchorPoint(Vec2(0.0f, 1.0f));
	fcoin3->setPosition(Vec2(10050, 880));
	this->addChild(fcoin3, 3);
	m_box.pushBack(fcoin3);

	auto fcoin2 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fcoin2->setTag(Block::Coin);
	fcoin2->setAnchorPoint(Vec2(0.0f, 1.0f));
	fcoin2->setPosition(Vec2(10200, 720));
	this->addChild(fcoin2, 3);
	m_box.pushBack(fcoin2);

	auto fcoin4 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fcoin4->setTag(Block::Coin);
	fcoin4->setAnchorPoint(Vec2(0.0f, 1.0f));
	fcoin4->setPosition(Vec2(10200, 880));
	this->addChild(fcoin4, 3);
	m_box.pushBack(fcoin4);

	auto fcoin5 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fcoin5->setTag(Block::Coin);
	fcoin5->setAnchorPoint(Vec2(0.0f, 1.0f));
	fcoin5->setPosition(Vec2(9810, 2120));
	this->addChild(fcoin5, 3);
	m_box.pushBack(fcoin5);

	auto fcoin6 = Sprite::createWithSpriteFrame(Coin1->getSpriteFrame());
	fcoin6->setTag(Block::Coin);
	fcoin6->setAnchorPoint(Vec2(0.0f, 1.0f));
	fcoin6->setPosition(Vec2(9940, 2120));
	this->addChild(fcoin6, 3);
	m_box.pushBack(fcoin6);

	
	auto SavePoint = Sprite::create("Object/SavePoint.png");
	SavePoint->setScale(0.6f);
	SavePoint->setPosition(Vec2(3800, 930));
	if (m_SaveNum == 1)
		SavePoint->setVisible(false);
	else
		SavePoint->setVisible(true);
	SavePoint->setName("SavePoint");
	this->addChild(SavePoint, 5);

	//클리어치트
	//CreateEventBlock();
}
