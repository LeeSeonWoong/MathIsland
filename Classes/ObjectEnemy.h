#ifndef __OBJECT_ENEMY_H__
#define __OBJECT_ENEMY_H__

#include "Default.h"

enum EnemyMotion
{
	EnemyWalk, EnemyIdle, EnemyDeath, FLY, FLYDeath, GroundWalk, GroundDeath
};

class ObjectEnemy
{
public:
	bool m_Have;
	Sprite* m_Spr;
	Sprite* m_box;
	std::vector<int> m_dir;
	float JumpPower;
	int State;
	int m_CurMotionNum;
	Layer* m_layer;
	bool m_Control;
	bool m_death;
	bool change;

	//�̲���Ʋ ��
	void CreateObject(float x, float y, bool xy);
	void EnemyUpdate();
	void AnimationSwitch(int num);
	void GetLayer(Layer*);
	void EnemyDeath();

	//���ߺξ���
	void CreateObject2(float x, float y, bool xy);
	void EnemyUpdate2();

	//������ ��
	void CreateObject3(float x, float y, bool xy);


	//Spine
	spine::SkeletonAnimation* m_spineAni;
	spAtlas* m_atl;
};

#endif 