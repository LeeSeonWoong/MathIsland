#include "Default.h"

bool GameUI::init()
{
	if (!Layer::init())
		return false;

	curLife = 3;
	m_HeroBox = NULL;
	m_HeroBoxExist = false;
	m_Inven = false;
	m_nonTouch = false;
	DrawDir();
	m_dir.clear();
	m_boxNum = -1;
	m_ShowOn = false;
	m_TouchCheck = NULL;
	//auto listener = EventListenerTouchOneByOne::create(); // 싱글터치용
	auto listener = EventListenerTouchAllAtOnce::create();// 멀티터치용 - touch->touches로 비긴,무브,앤드 함수 수정해야함
	listener->onTouchesBegan = CC_CALLBACK_2(GameUI::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(GameUI::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(GameUI::onTouchesEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	MakeKeyListener();

	LeftTag = -1;
	RightTag = -1;
	JumpTag = -1;

	return true;
}

void GameUI::DrawDir()
{
	// <-왼쪽
	Sprite* LeftB = Sprite::create("UI/left.png");
	LeftB->setAnchorPoint(Vec2(0.0f, 0.0f));
	LeftB->setPosition(Vec2(80, 90));
	LeftB->setScale(1.2f);
	LeftB->setName("LeftB");
	this->addChild(LeftB, 5);

	// ->오른쪽
	Sprite* RightB = Sprite::create("UI/right.png");
	RightB->setAnchorPoint(Vec2(0.0f, 0.0f));
	RightB->setPosition(Vec2(257, 90));
	RightB->setScale(1.2f);
	RightB->setName("RightB");
	this->addChild(RightB, 5);

	// 점프
	Sprite* JumpB = Sprite::create("UI/jump.png");
	JumpB->setAnchorPoint(Vec2(0.0f, 0.0f));
	JumpB->setPosition(Vec2(1070, 90));
	JumpB->setName("JUMP");
	JumpB->setScale(1.4f);
	this->addChild(JumpB, 5);

	// 인벤토리
	Sprite* Inventory = Sprite::create("UI/inventory.png");
	Inventory->setAnchorPoint(Vec2(0.0f, 0.0f));
	Inventory->setPosition(Vec2(950, 90));
	Inventory->setName("INVEN");
	this->addChild(Inventory, 5);

	// 라이프
	Sprite* Life = Sprite::create("UI/Life_point.png");
	Life->setAnchorPoint(Vec2(0.0f, 0.0f));
	Life->setPosition(Vec2(150, 590));
	Life->setName("LIFE");
	this->addChild(Life, 5);

	//cur라이프
	for (int i = 0; i < 3; ++i)
	{
		Sprite* Life = Sprite::create("UI/TempLife.png");
		Life->setAnchorPoint(Vec2(0.0f, 0.0f));
		Life->setPosition(Vec2(156 + i * 80 + i, 597));
		if (i == 0)
			Life->setName("Life1");
		else if (i == 1)
			Life->setName("Life2");
		else if (i == 2)
			Life->setName("Life3");
		this->addChild(Life, 5);
	}
}

void GameUI::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	std::vector<Touch*>::const_iterator iter = touches.begin();
	Touch* touch;
	for (iter = touches.begin(); iter != touches.end(); iter++)
	{
		m_TouchCheck = true;
		touch = (Touch*)(*iter);
		Vec2 pos = touch->getLocation();
		m_TouchPos = touch->getLocation();

		auto Lbutton = dynamic_cast<Sprite*>(this->getChildByName("LeftB"));
		auto Rbutton = dynamic_cast<Sprite*>(this->getChildByName("RightB"));
		auto Jbutton = dynamic_cast<Sprite*>(this->getChildByName("JUMP"));
		auto Ibutton = dynamic_cast<Sprite*>(this->getChildByName("INVEN"));

		if (Lbutton->getBoundingBox().containsPoint(pos) && LeftTag == -1)
		{
			LeftTag = touch->getID();
			m_dir.push_back(PadDir::LeftMove);
			return;
		}
		if (Rbutton->getBoundingBox().containsPoint(pos) && RightTag == -1)
		{
			RightTag = touch->getID();
			m_dir.push_back(PadDir::RightMove);
			return;
		}
		if (Jbutton->getBoundingBox().containsPoint(pos) && JumpTag == -1)
		{
			extern ObjectHero* Hero;
			if (Hero->CanJump == false)
			{
				JumpTag = touch->getID();
				jumpTimer = timer;
				m_dir.push_back(PadDir::BJUMP);
				return;
			}
		}
		if (Ibutton->getBoundingBox().containsPoint(m_TouchPos))
		{
			extern ObjectHero* Hero;
			if (Hero->m_Have == false)
			{
				if (Hero->m_CurMotionNum != HeroMotion::Jump)
				{
					if (m_ShowOn == false && m_Inven == false)
						m_Inven = true;
					else if (m_ShowOn)
						BoxHideMotion();
				}
			}
		}

		for (Sprite* box : m_Inventory)
		{
			if (box->getBoundingBox().containsPoint(m_TouchPos))
			{
				extern ObjectHero* Hero;
				if (Hero->m_Have == false)
				{
					m_ShowOn = false;
					m_Inven = false;
					Hero->m_Have = true;
					Hero->ShowSelectBox(box);
					m_Inventory.eraseObject(box);
					this->removeChild(box);
					BoxHideMotion();
					return;
				}
			}
		}
	}
}

void GameUI::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{

	std::vector<Touch*>::const_iterator iter = touches.begin();
	Touch* touch = NULL;

	for (iter = touches.begin(); iter != touches.end(); iter++)
	{
		m_TouchCheck = true;
		touch = (Touch*)(*iter);
		Vec2 pos = touch->getLocation();
		if (touch->getID() == LeftTag)
		{
			auto button = dynamic_cast<Sprite*>(this->getChildByName("LeftB"));
			if (!button->getBoundingBox().containsPoint(pos))
			{
				std::vector<int>::const_iterator iter2 = m_dir.begin();
				for (iter2; iter2 != m_dir.end(); ++iter2)
				{
					if ((*iter2) == PadDir::LeftMove)
					{
						LeftTag = -1;
						m_dir.erase(iter2);
						return;
					}
				}
			}
		}
	}
	for (iter = touches.begin(); iter != touches.end(); iter++)
	{
		m_TouchCheck = true;
		touch = (Touch*)(*iter);
		Vec2 pos = touch->getLocation();
		if (touch->getID() == RightTag)
		{
			auto button = dynamic_cast<Sprite*>(this->getChildByName("RightB"));
			if (!button->getBoundingBox().containsPoint(pos))
			{
				std::vector<int>::const_iterator iter2 = m_dir.begin();
				for (iter2; iter2 != m_dir.end(); ++iter2)
				{
					if ((*iter2) == PadDir::RightMove)
					{
						RightTag = -1;
						m_dir.erase(iter2);
						return;
					}
				}
			}
		}
	}
	for (iter = touches.begin(); iter != touches.end(); iter++)
	{
		m_TouchCheck = true;
		touch = (Touch*)(*iter);
		Vec2 pos = touch->getLocation();
		if (touch->getID() == JumpTag)
		{
			auto button = dynamic_cast<Sprite*>(this->getChildByName("JUMP"));
			if (!button->getBoundingBox().containsPoint(pos))
			{
				JumpTag = -1;
			}
		}
	}
}

void GameUI::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	std::vector<Touch*>::const_iterator iter = touches.begin();
	Touch* touch;
	auto Lbutton = dynamic_cast<Sprite*>(this->getChildByName("LeftB"));
	auto Rbutton = dynamic_cast<Sprite*>(this->getChildByName("RightB"));
	auto Jbutton = dynamic_cast<Sprite*>(this->getChildByName("JUMP"));
	std::vector<int>::const_iterator iter2 = m_dir.begin();

	for (iter = touches.begin(); iter != touches.end(); iter++)
	{
		touch = (Touch*)(*iter);
		Vec2 pos = touch->getLocation();
		if (touch->getID() == LeftTag)
		{
			for (iter2 = m_dir.begin(); iter2 != m_dir.end(); ++iter2)
			{
				if ((*iter2) == PadDir::LeftMove)
				{
					m_dir.erase(iter2);
					LeftTag = -1;
					return;
				}
			}
		}
	}
	for (iter = touches.begin(); iter != touches.end(); iter++)
	{
		touch = (Touch*)(*iter);
		Vec2 pos = touch->getLocation();
		if (touch->getID() == RightTag)
		{
			for (iter2 = m_dir.begin(); iter2 != m_dir.end(); ++iter2)
			{
				if ((*iter2) == PadDir::RightMove)
				{
					m_dir.erase(iter2);
					RightTag = -1;
					return;
				}
			}
		}
	}
	for (iter = touches.begin(); iter != touches.end(); iter++)
	{
		touch = (Touch*)(*iter);
		Vec2 pos = touch->getLocation();
		if (touch->getID() == JumpTag)
		{
			JumpTag = -1;
			return;
		}
	}
	/*iter = touches.begin();
	for (iter = touches.begin(); iter != touches.end(); iter++)
	{
	touch = (Touch*)(*iter);
	Vec2 pos = touch->getLocation();
	m_TouchPos = touch->getLocation();
	for (iter2; iter2 != m_dir.end(); ++iter2)
	{
	if (Rbutton->getBoundingBox().containsPoint(m_TouchPos))
	{
	m_dir.erase(iter2);
	return;
	}
	}
	}
	iter = touches.begin();
	for (iter = touches.begin(); iter != touches.end(); iter++)
	{
	touch = (Touch*)(*iter);
	Vec2 pos = touch->getLocation();
	m_TouchPos = touch->getLocation();
	if (Jbutton->getBoundingBox().containsPoint(m_TouchPos))
	{
	if (timer - jumpTimer < 0.3f)
	m_dir.push_back(PadDir::JUMP);
	else if (timer - jumpTimer > 0.3f)
	m_dir.push_back(PadDir::BJUMP);
	return;
	}
	}*/
}

void GameUI::GetInvenInfo(Vector<Sprite*> box)
{
	m_boxNum = box.size();
	m_Inventory = box;
}

void GameUI::BoxHideMotion()
{
	m_ShowOn = false;
	m_Inven = false;

	int num = 0;
	for (Sprite* box : m_Inventory)
	{
		num = box->getTag();
		auto DynamicBox = dynamic_cast<Sprite*>(this->getChildByTag(num));
		if (DynamicBox == NULL)
		{

		}
		else
		{
			this->removeChild(DynamicBox);
			//DynamicBox->removeFromParent();
		}
	}
}

bool GameUI::ShowInvenBox()
{
	m_ShowOn = true;
	m_Inven = false;
	m_boxMotionNum = 0;
	int num = 0;
	for (Sprite* box : m_Inventory)
	{
		num = box->getTag();

		switch (num)
		{
		case Block::Get1:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::Get1);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::Get2:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::Get2);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::Get3:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::Get3);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::Get4:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::Get4);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::Get5:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::Get5);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::Get6:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::Get6);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::Get7:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::Get7);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::Get8:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::Get8);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::Get9:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::Get9);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::GetSquare:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::GetSquare);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::GetMinus:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::GetMinus);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::GetPlus:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::GetPlus);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		case Block::GetDiv:
			box->setAnchorPoint(Vec2(0.0f, 0.0f));
			box->setPosition(Vec2(950, 90));
			box->setTag(Block::GetDiv);
			box->setVisible(true);
			this->addChild(box, 10);
			break;
		default:
			break;
		}
		BoxShowMotion(box);

	}
	return true;
}

void GameUI::DeleteInvenBox(int tagNum)
{
	for (Sprite* box : m_Inventory)
	{
		if (box->getTag() == tagNum)
		{
			if (m_boxNum != 0)
				m_boxNum -= 1;
			m_Inventory.eraseObject(box);
			return;
		}
	}
}

void GameUI::BoxShowMotion(Sprite* spr)
{
	Vec2 pos;

	if (m_boxMotionNum != m_boxNum)
	{
		spr->setVisible(true);
		if (m_boxMotionNum == 0)
		{
			pos.x = 850;
			pos.y = 90;
		}
		else if (m_boxMotionNum == 1)
		{
			pos.x = 880;
			pos.y = 190;
		}
		else if (m_boxMotionNum == 2)
		{
			pos.x = 980;
			pos.y = 200;
		}
		else if (m_boxMotionNum == 3)
		{
			pos.x = 1070;
			pos.y = 200;
		}
		m_boxMotionNum += 1;
	}

	spr->runAction(Sequence::create(
		MoveTo::create(0.5f, pos),
		NULL));
}

void GameUI::GameOver()
{
	auto spr = Sprite::create("UI/Game_over.png");
	spr->setPosition(Vec2(0, 360));
	spr->runAction(
		Sequence::create(
			MoveTo::create(0.3f, Vec2(640, 360)),
			FadeIn::create(0.5f),
			NULL));
	this->addChild(spr, 5);
}

void GameUI::GameClear()
{
	UserDefault::getInstance()->setIntegerForKey("Save4", 0);
	auto spr = Sprite::create("UI/Stage_clear.png");
	spr->setPosition(Vec2(0, 360));
	spr->runAction(
		Sequence::create(
			MoveTo::create(0.3f, Vec2(640, 360)),
			FadeIn::create(0.5f),
			NULL));
	this->addChild(spr, 5);
}

void GameUI::MakeKeyListener()
{
	//키보드 리스너
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(GameUI::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(GameUI::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
}

void GameUI::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		m_dir.push_back(PadDir::LeftMove);
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		m_dir.push_back(PadDir::RightMove);
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_ALT)
	{

		m_dir.push_back(PadDir::BJUMP);

	}
}

void GameUI::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	std::vector<int>::const_iterator iter = m_dir.begin();

	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		for (iter; iter != m_dir.end(); ++iter)
		{
			if (*iter == PadDir::LeftMove)
			{
				m_dir.erase(iter);
				return;
			}
		}
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		for (iter; iter != m_dir.end(); ++iter)
		{
			if (*iter == PadDir::RightMove)
			{
				m_dir.erase(iter);
				return;
			}
		}
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_ALT)
	{
		//keychange = false;
		for (iter; iter != m_dir.end(); ++iter)
		{
			if (*iter == PadDir::BJUMP || *iter == PadDir::JUMP)
			{
				m_dir.erase(iter);
				return;
			}
		}
	}
}

void GameUI::SetLife()
{
	if (curLife == 2)
	{
		auto heart = dynamic_cast<Sprite*>(this->getChildByName("Life3"));
		heart->setVisible(false);
	}
	else if (curLife == 1)
	{
		auto heart = dynamic_cast<Sprite*>(this->getChildByName("Life3"));
		heart->setVisible(false);

		auto heart2 = dynamic_cast<Sprite*>(this->getChildByName("Life2"));
		heart2->setVisible(false);
	}
	else if (curLife == 0)
	{
		auto heart = dynamic_cast<Sprite*>(this->getChildByName("Life3"));
		heart->setVisible(false);

		auto heart2 = dynamic_cast<Sprite*>(this->getChildByName("Life2"));
		heart2->setVisible(false);

		auto heart3 = dynamic_cast<Sprite*>(this->getChildByName("Life1"));
		heart3->setVisible(false);
	}
}
void GameUI::GetLife(int num)
{
	if (curLife != num)
	{
		curLife = num;
		SetLife();
	}
}
void GameUI::GetTimer(float time)
{
	timer = time;

}