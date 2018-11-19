#ifndef __STAGE01_SCENE_H__
#define __STAGE01_SCENE_H__

#include "Default.h"

#define EffectNum 11

class Stage01 : public Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Stage01);

	//Default
	void CameraAction1();
	void CameraAction2();//�������� ���� �����ִ� ī�޶�

	int m_Score;
	Size m_winSize;
	int m_Life;
	int m_SaveNum;
	bool stoneOnce;

	//������Ʈ
	float m_Timer;
	void update(float dt);
	float curTime;

	//Ÿ�̸�
	float m_timer;
	float timer1;
	bool CameraFix;

	//Ÿ�ϸ�
	void CreateBack();
	void RoadTileMap();
	TMXTiledMap* m_tmap;
	TMXLayer* m_background;
	TMXLayer* m_metainfo;
	TMXLayer* m_metainfo2;
	TMXLayer* m_metainfo3;
	TMXLayer* interact;//�밢����


	Vector<Sprite*> m_metabox;

	//�ִϸ��̼�
	Animation* effect[EffectNum];
	void AnimationSwitch(Sprite* block);

	//��ü(Block)
	Vector<Sprite*> m_Inven;
	Vector<Sprite*> m_box;
	Vector<Sprite*> m_ChangeBox;
	void CreateBlock();
	void CreateBoxFromInven(Sprite*,int);
	void DelayCreate();
	Sprite* m_ToSave;

	void CheckBoxObject(float dt);
	void DeleteInvenBox(int tagNum);
	bool gravity;

	//����ü
	ObjectEnemy* m_enemy[3];
	ObjectEnemy* m_Fly[3];
	ObjectEnemy* m_Ground[3];

	void CraeteEnemy();
	bool m_EnemyColide;

	//�̺�Ʈ
	int coinEvent;
	Sprite* scaleBlock;
	bool once[3];

	void GameOver();
	void GameClear();

	bool m_Stage01Event1;
	bool m_condition[10];
	void ConditionCheck();
	void Stage01Event1();
	void EventCheck(Sprite* block);
	void CreateSetting1();
	void CreateEventBlock();
	void CreateDungeon();
	Vec2 DunPos;

	Vec2 For_SetBlock;
	bool event1;
	bool event2;
	bool event3;
	bool event4;
	bool event5;

	bool con1, con2, con3, con4, con5, con6, con7;

	bool OverCheck;
	bool duncon;
	int m_Scale;

	//ī�޶�
	void setViewPointCenter(Vec2 pos);

	//��������
	int HeroDir;
	PhysicsWorld* m_world;
	void setPhyWorld(PhysicsWorld* world)
	{
		m_world = world;
	}
	bool onContactBegin(PhysicsContact &contact);

	//��ġ
	void MoveCharacter(int dir);
	Vec2 m_SaveViewPos;
	Vec2 m_TouchPos;
	bool m_TouchCheck;

};

#endif 
