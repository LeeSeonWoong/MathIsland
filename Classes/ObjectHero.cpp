#include "Default.h"

ObjectHero* Hero = new ObjectHero();

void ObjectHero::CreateObject(float x, float y)
{
	StageClear = false;
	m_Have = false;
	Sin_Event = false;
	m_box = Sprite::create("block/BreakBlock.png");
	m_box->setVisible(ColiideRect);
	m_layer->addChild(m_box, 2);
	m_Spr = Sprite::create("Hero/HeroRect.png");
	m_Spr->setScaleX(0.9f);
	m_Spr->setPosition(Vec2(x, y));//318 1032
	m_Spr->setVisible(ColiideRect);
	m_layer->addChild(m_Spr, 2);
	JumpPower = PhysicNum::JumpPower;

	m_atl = new spAtlas;
	m_atl = spAtlas_createFromFile("Hero/P1.atlas", m_atl->rendererObject);
	m_spineAni = NULL;
	m_spineAni = spine::SkeletonAnimation::createWithFile("Hero/P1.json", m_atl, 0.5f);

	m_layer->addChild(m_spineAni, 3);

	m_spineAni->setScale(0.3f);
	m_spineAni->setPosition(x, y);
	m_spineAni->clearTracks();

	CoinNum = UserDefault::getInstance()->getIntegerForKey("HeroCoin", 0);
	m_Death = false;
	State = -1;
	m_Control = true;
	move = false;
	SmallJump = PhysicNum::SJumpPower;
	BigJump = PhysicNum::JumpPower;

	CanJump = false;
	JumpCheck = false;
	col = false;
	fmx.x = 0;
	fmx.y = 0;
	mx = 0;
	my = 0;
	graph = NULL;
}

void ObjectHero::GetLayer(Layer* layer)
{
	m_layer = layer;
}

void ObjectHero::AnimationSwitch(int num)
{
	int trackIndex = 0;

	if (num == HeroMotion::Walk)//걷기
	{
		if (State != num)
		{
			m_CurMotionNum = HeroMotion::Walk;
			State = HeroMotion::Walk;
			m_spineAni->clearTracks();
			/*m_spineAni->setMix("idle", "stand", 0.6);
			m_spineAni->setMix("stand", "run", 0.6);
			m_spineAni->setAnimation(0, "idle", false);
			m_spineAni->setAnimation(0, "stand", false);
			m_spineAni->addAnimation(0, "run", true);*/

			m_spineAni->addAnimation(0, "walking", true);
		}
	}
	else if (num == HeroMotion::Idle)//서있기
	{
		if (State != num)
		{
			m_CurMotionNum = HeroMotion::Idle;
			State = HeroMotion::Idle;
			m_spineAni->clearTracks();

			m_spineAni->addAnimation(0, "idle", true);
		}
	}
	else if (num == HeroMotion::Jump)//점프
	{
		if (State != num)
		{
			m_CurMotionNum = HeroMotion::Jump;
			State = HeroMotion::Jump;
			m_spineAni->clearTracks();
			m_spineAni->setMix("idle", "jump", 0.06);
			m_spineAni->setMix("jump", "idle", 0.9);
			m_spineAni->setAnimation(0, "idle", false);
			m_spineAni->setAnimation(0, "jump", false);
			m_spineAni->addAnimation(0, "idle", true);
		}
	}
	else if (num == HeroMotion::Damage)//죽음
	{
		if (State != num)
		{
			CSoundManager::getInstance()->playEffect("Sound/cat_sound4.mp3", false, 0.8f);
			m_Control = false;
			m_Spr->stopAllActions();
			m_CurMotionNum = HeroMotion::Damage;
			State = HeroMotion::Damage;
			m_spineAni->stopAllActions();
			m_spineAni->clearTracks();
			m_spineAni->addAnimation(0, "damage", false);

			if (m_spineAni->getRotationY() == 0)
			{
				Vec2 pos = m_spineAni->getPosition();
				pos.x -= 50;
				pos.y += 50;
				m_spineAni->runAction(Sequence::create(
					JumpTo::create(0.5f, m_spineAni->getPosition(), 100,1),
					FadeOut::create(1.5f)
						,NULL));
			}
			else if(m_spineAni->getRotationY() == 180)
			{
				Vec2 pos = m_spineAni->getPosition();
				pos.x += 50;
				pos.y += 50;
				m_spineAni->runAction(Sequence::create(
					JumpTo::create(0.5f, m_spineAni->getPosition(), 100, 1),
					FadeOut::create(1.5f)
					, NULL));
			}
		}
	}
}

void ObjectHero::InitSelectBox()
{
	m_Have = false;
	m_box->setVisible(false);
	m_box->setPosition(Vec2(-100, -100));
}

void ObjectHero::HeroUpdate()
{
	if (m_Have)
		m_box->setPosition(Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() + 90));
}

void ObjectHero::ShowSelectBox(Sprite* spr)
{
	m_box->setTexture(spr->getTexture());
	int tn = spr->getTag();
	m_box->setTag(tn);
	m_box->setVisible(true);
	m_box->setPosition(Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() + 90));
}

void ObjectHero::MoveCharacter(std::vector<int> *dir, Vector<Sprite*> metabox)
{
	static float gravity;
	if (m_Control && m_Death == false)
	{
		int AniNum = HeroMotion::Jump;
		///////////////////////////////////////////////////////////////////////////////
		// 충돌박스와 주인공 중력체크
		//static bool noGravity = false;
		//CanJump = true;


		//if (noGravity == false)
		{
			if (CanJump)
				gravity += 0.1f;
			// 중력 적용
			CanJump = true;

			m_Spr->setPosition(Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() - PhysicNum::Gravity - gravity));
			for (Sprite* box : metabox)
			{
				if (m_Spr->getBoundingBox().intersectsRect(box->getBoundingBox()) && box->getTag() == 44)
				{
					gravity = 0;
					//AnimationSwitch(HeroMotion::Damage);
					//m_Death = true;
					//m_Control = false;
					return;
				}
				// 박스체크 : 중력으로 인한 하락체크이기 때문에 박스Y가 주인공Y 이하값 이여야 함
				if (m_Spr->getBoundingBox().intersectsRect(box->getBoundingBox()))
				{
					if (box->getName() == "MoveBox" && box->getPositionY() + box->getContentSize().height * 0.5f <= m_Spr->getPositionY())
					{
						if (move == false)
						{
							mx = box->getPositionX();
							move = true;
						}
						else
						{
							CanJump = false; // 지면에 닿은경우 CanJump = false를 대입한다
							AniNum = HeroMotion::Idle;
							m_Spr->setPosition(Vec2(m_Spr->getPositionX() + box->getPositionX() - mx, m_Spr->getPositionY() + PhysicNum::Gravity + gravity));
							mx = box->getPositionX();
							gravity = 0;
						}
					}
					else if (box->getName() == "FXmeta" && Sin_Event && box->getPositionY() < m_Spr->getPositionY())
					{
						if (move == false)
						{
							//fmx = m_Spr->getPosition() - box->getPosition();
							mx = box->getPositionX();
							my = box->getPositionY();
							move = true;
							if (graph == false)
							{
								auto fx = dynamic_cast<Sprite*>(m_layer->getChildByName("FX"));
								fx->runAction(Sequence::create(
									MoveTo::create(1.1f, Vec2(7395, 1055 + 20)),
									MoveTo::create(1.1f, Vec2(7555, 975 + 20)),
									MoveTo::create(1.1f, Vec2(7728, 1120 + 20)),
									MoveTo::create(1.1f, Vec2(7808, 1360 + 20)),
									MoveTo::create(1.5f, Vec2(7980, 1610 + 20)),
									MoveTo::create(0.6f, Vec2(8050, 1660 + 20)),
									MoveTo::create(0.6f, Vec2(8130, 1650 + 20)),
									MoveTo::create(0.6f, Vec2(8230, 1570 + 20)),
									MoveTo::create(0.6f, Vec2(8330, 1450 + 20)),
									MoveTo::create(1.0f, Vec2(8380, 1250 + 20)),
									MoveTo::create(0.8f, Vec2(8505, 1080 + 20)),
									MoveTo::create(0.8f, Vec2(8625, 1000 + 20)),
									MoveTo::create(1.0f, Vec2(8785, 1080 + 20)),
									MoveTo::create(1.3f, Vec2(8915, 1310 + 20)),
									NULL));
								graph = true;
								box->runAction(Sequence::create(
									MoveTo::create(1.1f, Vec2(7395, 1055)),
									MoveTo::create(1.1f, Vec2(7555, 975)),
									MoveTo::create(1.1f, Vec2(7728, 1120)),
									MoveTo::create(1.1f, Vec2(7808, 1360)),
									MoveTo::create(1.5f, Vec2(7980, 1610)),
									MoveTo::create(0.6f, Vec2(8050, 1660)),
									MoveTo::create(0.6f, Vec2(8130, 1650)),
									MoveTo::create(0.6f, Vec2(8230, 1570)),
									MoveTo::create(0.6f, Vec2(8330, 1450)),
									MoveTo::create(1.0f, Vec2(8380, 1250)),
									MoveTo::create(0.8f, Vec2(8505, 1080)),
									MoveTo::create(0.8f, Vec2(8625, 1000)),
									MoveTo::create(1.0f, Vec2(8785, 1080)),
									MoveTo::create(1.3f, Vec2(8915, 1310)),
									NULL));
							}
						}
						else
						{
							CanJump = false; // 지면에 닿은경우 CanJump = false를 대입한다
							AniNum = HeroMotion::Idle;
							m_Spr->setPosition(Vec2(m_Spr->getPositionX() + box->getPositionX() - mx, m_Spr->getPositionY() + box->getPositionY() - my + PhysicNum::Gravity + gravity));
							//m_Spr->setPosition(Vec2(m_Spr->getPositionX() + fmx.x, m_Spr->getPositionY() + fmx.y));
							mx = box->getPositionX();
							my = box->getPositionY();
							gravity = 0;

						}
					}
					else if (box->getPositionY() + box->getContentSize().height * 0.5f <= m_Spr->getPositionY())
					{
						CanJump = false; // 지면에 닿은경우 CanJump = false를 대입한다
						AniNum = HeroMotion::Idle;

						move = false;
						m_Spr->setPosition(Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() + PhysicNum::Gravity + gravity));//중력으로 인해 감소한 값만큼 다시 높혀준다
						gravity = 0;
					}
					//CanJump = false; // 지면에 닿은경우 CanJump = false를 대입한다
					//AniNum = HeroMotion::Idle;
				}
				
				/*else
					move = false;*/

			}
		}
		////////// 이동 체크 ////////////////////////////////////////////////////
		std::vector<int>::const_iterator iter = dir->begin();
		for (iter; iter != dir->end(); ++iter)
		{
			if (*iter == PadDir::JUMP && JumpCheck == false && CanJump == false)
			{
				CSoundManager::getInstance()->playEffect("Sound/Jump.mp3", false, 1.0f);
				JumpPower = PhysicNum::SJumpPower;
				dir->erase(iter);
				col = false;
				JumpCheck = true; // 방향, CanJump(지면닿음확인), JumpCheck(중복누름방지) 만족시 true
				return;
			}
			else if (*iter == PadDir::BJUMP && JumpCheck == false && CanJump == false)
			{
				CSoundManager::getInstance()->playEffect("Sound/Jump.mp3", false, 1.0f);
				JumpPower = PhysicNum::JumpPower;
				dir->erase(iter);
				col = false;
				JumpCheck = true; // 방향, CanJump(지면닿음확인), JumpCheck(중복누름방지) 만족시 true
				return;
			}

			// 이동
			if (*iter == PadDir::LeftMove)// <--
			{
				m_spineAni->setRotationY(180);
				AniNum = HeroMotion::Walk;
				if(m_Spr->getPositionX() >= 5160 || m_Spr->getPositionX() <= 5150)
					m_Spr->setPositionX(m_Spr->getPositionX() - PhysicNum::MovePower);
			}
			else if (*iter == PadDir::RightMove)// -->
			{
				m_spineAni->setRotationY(0);
				AniNum = HeroMotion::Walk;
				m_Spr->setPositionX(m_Spr->getPositionX() + PhysicNum::MovePower);
			}

			for (Sprite* box : metabox)
			{
				if (m_Spr->getBoundingBox().intersectsRect(box->getBoundingBox()) && box->getTag() != 55 && box->getName() != "MoveBox")
				{
					if (*iter == PadDir::LeftMove)
						m_Spr->setPositionX(m_Spr->getPositionX() + PhysicNum::MovePower);
					if (*iter == PadDir::RightMove)
						m_Spr->setPositionX(m_Spr->getPositionX() - PhysicNum::MovePower);
				}
			}
		}
		m_spineAni->setPosition(Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() - 70));

		///// 점프 체크 ////////////////////////
		if (JumpCheck && JumpPower != 5)
		{
			gravity = 0;
			move = false;
			AniNum = HeroMotion::Jump;
			if (col == false)
			{
				m_Spr->setPositionY(m_Spr->getPositionY() + JumpPower);
				JumpPower -= 0.5f;
			}
			for (Sprite* box : metabox)
			{
				if (m_Spr->getBoundingBox().intersectsRect(box->getBoundingBox()) && box->getTag() != 55)
				{
					JumpCheck = false;
					//JumpPower = PhysicNum::JumpPower;
					col = true;
					m_spineAni->setPosition(Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() - 70));
					switch (AniNum)
					{
					case HeroMotion::Walk:
						AnimationSwitch(HeroMotion::Walk);
						break;
					case HeroMotion::Idle:
						AnimationSwitch(HeroMotion::Idle);
						break;
					case HeroMotion::Jump:
						AnimationSwitch(HeroMotion::Jump);
						break;
					}
					return;
				}
			}
		}
		else//점프종료
		{
			JumpCheck = false;
		}
		m_spineAni->setPosition(Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() - 70));

		switch (AniNum)
		{
		case HeroMotion::Walk:
			AnimationSwitch(HeroMotion::Walk);
			break;
		case HeroMotion::Idle:
			AnimationSwitch(HeroMotion::Idle);
			break;
		case HeroMotion::Jump:
			AnimationSwitch(HeroMotion::Jump);
			break;
		}
	}
	else
	{
		m_spineAni->setPosition(Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() - 70));
		//AnimationSwitch(HeroMotion::Jump);
	}
}

//void ObjectHero::CallJump()
//{
//	
//}
//void ObjectHero::SmallCallJump()
//{
//	
//
//}
//
//void ObjectHero::ReturnControl()
//{
//
//}