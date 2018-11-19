#include "Default.h"

void ObjectEnemy::CreateObject(float x, float y, bool xy)
{
	m_Spr = Sprite::create("Enemy/EnemyRect.png");
	m_Spr->setPosition(Vec2(x, y + 40));
	m_Spr->setScaleY(0.7f);
	m_Spr->setVisible(ColiideRect);
	m_layer->addChild(m_Spr, 2);

	m_atl = new spAtlas;
	m_atl = spAtlas_createFromFile("Enemy/g_e2.atlas", m_atl->rendererObject);
	m_spineAni = NULL;
	m_spineAni = spine::SkeletonAnimation::createWithFile("Enemy/g_e2.json", m_atl, 0.5f);
	m_layer->addChild(m_spineAni, 2);

	m_spineAni->setScale(0.7f);
	m_spineAni->setPosition(x, y);
	m_spineAni->clearTracks();

	State = -1;
	m_death = false;
	change = xy;
	AnimationSwitch(EnemyMotion::EnemyWalk);
}

void ObjectEnemy::CreateObject2(float x, float y, bool xy)
{
	m_Spr = Sprite::create("Enemy/EnemyRect.png");
	m_Spr->setPosition(Vec2(x, y + 60));
	m_Spr->setScaleY(0.4f);
	m_Spr->setVisible(ColiideRect);
	m_layer->addChild(m_Spr, 2);

	m_atl = new spAtlas;
	m_atl = spAtlas_createFromFile("Enemy/S_e1.atlas", m_atl->rendererObject);
	m_spineAni = NULL;
	m_spineAni = spine::SkeletonAnimation::createWithFile("Enemy/S_e1.json", m_atl, 0.5f);
	m_layer->addChild(m_spineAni, 2);

	m_spineAni->setScale(0.7f);
	m_spineAni->setPosition(x, y);
	m_spineAni->clearTracks();

	m_death = false;
	State = -1;
	change = xy;
	AnimationSwitch(EnemyMotion::FLY);
}

void ObjectEnemy::CreateObject3(float x, float y, bool xy)
{
	m_Spr = Sprite::create("Enemy/EnemyRect.png");
	m_Spr->setPosition(Vec2(x, y + 60));
	m_Spr->setScaleY(0.4f);
	m_Spr->setVisible(ColiideRect);
	m_layer->addChild(m_Spr, 2);

	m_atl = new spAtlas;
	m_atl = spAtlas_createFromFile("Enemy/g_e1.atlas", m_atl->rendererObject);
	m_spineAni = NULL;
	m_spineAni = spine::SkeletonAnimation::createWithFile("Enemy/g_e1.json", m_atl, 0.5f);
	m_layer->addChild(m_spineAni, 2);

	m_spineAni->setScale(0.3f);
	m_spineAni->setPosition(x, y);
	m_spineAni->clearTracks();

	m_death = false;
	State = -1;
	change = xy;
	AnimationSwitch(EnemyMotion::GroundWalk);
}

void ObjectEnemy::EnemyUpdate()
{
	if (m_death == false)
		m_Spr->setPosition(m_spineAni->getPosition());
}

void ObjectEnemy::EnemyUpdate2()
{
	m_Spr->setPosition(Vec2(m_spineAni->getPositionX(), m_spineAni->getPositionY() + 60));
}

void ObjectEnemy::AnimationSwitch(int num)
{
	Vec2 pos = m_spineAni->getPosition();
	if (num == EnemyMotion::EnemyWalk)//°È±â
	{
		if (State != num)
		{
			m_CurMotionNum = EnemyMotion::EnemyWalk;
			State = EnemyMotion::EnemyWalk;
			m_spineAni->clearTracks();
			m_spineAni->addAnimation(0, "work", true);
			//if (change == false)
			{
				m_spineAni->runAction(
					RepeatForever::create(
						Sequence::create(
							RotateTo::create(0.0f, Vec3(0.0f, 0, 45.0f)),
							MoveTo::create(1.0f, Vec2(pos.x - 300, pos.y + 300)),
							RotateTo::create(0.0f, Vec3(0.0f, 180.0f, 45.0f)),
							MoveTo::create(1.0f, Vec2(pos.x + 300, pos.y - 300)),
							NULL)));
			}
		}
	}
	else if (num == EnemyMotion::EnemyDeath)//Áö»óÇü ¸ó½ºÅÍ»ç¸Á
	{
		if (State != num)
		{
			CSoundManager::getInstance()->playEffect("Sound/Monster_Death1.mp3", false, 0.6f);
			m_CurMotionNum = EnemyMotion::EnemyWalk;
			State = EnemyMotion::EnemyWalk;
			m_death = true;
			m_spineAni->stopAllActions();
			m_spineAni->clearTracks();
			m_spineAni->runAction(
				Spawn::create(
					ScaleTo::create(0.3f,0.6f,0.1f),
					//RotateTo::create(0.0f, Vec3(180.0f, 0.0f, 0.0f)),
					//MoveTo::create(0.3f, Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() + 100))
					 NULL));
			RUN_WITH_DELAY(0.4f, CC_CALLBACK_0(ObjectEnemy::EnemyDeath, this));
		}
	}
	else if (num == EnemyMotion::FLY)//°È±â
	{
		if (State != num)
		{
			m_CurMotionNum = EnemyMotion::FLY;
			State = EnemyMotion::FLY;
			m_spineAni->clearTracks();
			m_spineAni->addAnimation(0, "animation", true);
			m_spineAni->runAction(
				RepeatForever::create(Sequence::create(
					MoveTo::create(3.0f, Vec2(m_Spr->getPositionX() - 350, m_Spr->getPositionY() - 60)),
					RotateTo::create(0.0f, Vec3(0.0f, 180.0f, 0.0f)),
					MoveTo::create(3.0f, Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() - 60)),
					RotateTo::create(0.0f, Vec3(0.0f, 0.0f, 0.0f)),
					NULL)));
		}
	}
	else if (num == EnemyMotion::FLYDeath)//»ç¸Á
	{
		if (State != num)
		{
			CSoundManager::getInstance()->playEffect("Sound/Monster_Death1.mp3", false, 0.6f);
			m_CurMotionNum = EnemyMotion::FLYDeath;
			State = EnemyMotion::FLYDeath;
			m_death = true;
			m_spineAni->stopAllActions();
			m_spineAni->clearTracks();
			if (m_spineAni->getRotationY() == 180)
			{
				m_spineAni->runAction(
					Spawn::create(
						RotateTo::create(0.5f, Vec3(0.0f, 0.0f, 180.0f)),
						MoveTo::create(0.5f, Vec2(m_spineAni->getPositionX() + 40, m_spineAni->getPositionY() + 60)),
						NULL));
			}
			else if (m_spineAni->getRotationY() == 0)
			{
				m_spineAni->runAction(
					Spawn::create(
						RotateTo::create(0.5f, Vec3(0.0f, 0.0f, 180.0f)),
						MoveTo::create(0.5f, Vec2(m_spineAni->getPositionX() - 40, m_spineAni->getPositionY() - 60)),
						NULL));
			}
			RUN_WITH_DELAY(1.0f, CC_CALLBACK_0(ObjectEnemy::EnemyDeath, this));
		}
	}
	else if (num == EnemyMotion::GroundWalk)//°È±â
	{
		if (State != num)
		{
			m_CurMotionNum = EnemyMotion::GroundWalk;
			State = EnemyMotion::GroundWalk;
			m_spineAni->clearTracks();
			m_spineAni->addAnimation(0, "walking", true);
			if (change == false)
			{
				m_spineAni->runAction(
					RepeatForever::create(
						Sequence::create(
							RotateTo::create(0.0f, Vec3(0.0f, 180.0f, 0.0f)),
							MoveTo::create(2.3f, Vec2(pos.x + 130, pos.y)),
							RotateTo::create(0.0f, Vec3(0.0f, 0, 0.0f)),
							MoveTo::create(2.1f, Vec2(pos.x - 100, pos.y)),
							NULL)));
			}
			else
			{
				m_spineAni->runAction(
					RepeatForever::create(
						Sequence::create(
							RotateTo::create(0.0f, Vec3(0.0f, 0, 0.0f)),
							MoveTo::create(2.3f, Vec2(pos.x - 100, pos.y)),
							RotateTo::create(0.0f, Vec3(0.0f, 180.0f, 0.0f)),
							MoveTo::create(2.5f, Vec2(pos.x + 130, pos.y)),
							NULL)));
			}
		}
	}
}

void ObjectEnemy::GetLayer(Layer* layer)
{
	m_layer = layer;
}

void ObjectEnemy::EnemyDeath()
{
	m_layer->removeChild(m_spineAni);
	m_layer->removeChild(m_Spr);
}
