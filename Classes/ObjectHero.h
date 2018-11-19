#ifndef __OBJECT_HERO_H__
#define __OBJECT_HERO_H__

#include "Default.h"

enum PhysicNum
{
	Gravity = 8, JumpPower = 26, MovePower = 7, SJumpPower = 21
};

enum HeroMotion
{
	Walk = 10, Idle =11, Jump = 12, Damage = 13
};

class ObjectHero
{
public:
	bool StageClear;
	bool m_Have;
	Sprite* m_Spr;
	Sprite* m_box;
	std::vector<int> m_dir;
	float JumpPower;
	int State;
	int m_CurMotionNum;
	Layer* m_layer;
	bool m_Control;
	bool m_Death;
	bool CanJump;
	bool JumpCheck;
	bool col;
	Vec2 fmx;
	float mx;
	float my;
	bool graph;

	int CoinNum;
	int SmallJump;
	int BigJump;

	//이동박스
	bool move;
	Vec2 mb;
	bool Sin_Event;

	void CreateObject(float x, float y);
	void MoveCharacter(std::vector<int> *dir, Vector<Sprite*> metabox);
	void ShowSelectBox(Sprite*);
	void HeroUpdate();
	void InitSelectBox();
	void AnimationSwitch(int num);
	void GetLayer(Layer*);

	void CallJump()
	{
		m_Control = false;
		AnimationSwitch(HeroMotion::Jump);
		m_Spr->runAction(Sequence::create(
			MoveTo::create(0.3f, Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() + 260)),
			CallFunc::create(CC_CALLBACK_0(ObjectHero::ReturnControl, this)),
			NULL));
	}
	
	void SmallCallJump()
	{
		m_Control = false;
		AnimationSwitch(HeroMotion::Jump);
		m_Spr->runAction(Sequence::create(
			MoveTo::create(0.3f, Vec2(m_Spr->getPositionX(), m_Spr->getPositionY() + 130)),
			CallFunc::create(CC_CALLBACK_0(ObjectHero::ReturnControl, this)),
			NULL));
	}
	
	void ReturnControl()
	{
		m_Control = true;

	}

	//Spine
	spine::SkeletonAnimation* m_spineAni;
	spAtlas* m_atl;
};

#endif 
