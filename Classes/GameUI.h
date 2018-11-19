#ifndef __GAMEUI_SCENE_H__
#define __GAMEUI_SCENE_H__

#include "Default.h"

enum PadDir
{
	LeftMove = 201, RightMove = 202, JUMP = 203, INVEN = 204, BJUMP = 205, AJUMP = 206
};

enum Block {
	Coin = 111,
	Change0 = 300, Change1 = 301, Change2 = 302, Change3 = 303, Change4 = 304, Change5 = 305, Change6 = 306, Change7 = 307, Change8 = 308, Change9 = 309,
	ChangeX = 310, ChangeY = 311, ChangeZ = 312,
	Change_Mul = 313, Change_Div = 314, Change_Plus = 315, Change_Minus = 316,
	Change_Left = 317, Change_Right = 318, Change_Same = 319,

	Setting = 150, Break = 151,
	Get0 = 320, Get1 = 321, Get2 = 322, Get3 = 323, Get4 = 324, Get5 = 325, Get6 = 326, Get7 = 327, Get8 = 328, Get9 = 329,
	GetSquare = 330, GetMinus = 331, GetPlus = 332, GetDiv = 333
};

class GameUI : public Layer
{
public:
	CREATE_FUNC(GameUI);
	virtual bool init();

	//Default
	bool keychange = false;
	float jumpTimer = 0;
	float timer;
	void GetTimer(float);
	void GetLife(int);
	void SetLife();
	Size winSize;
	int curLife;

	//동작수행
	bool m_Inven;
	void GameOver();
	void GameClear();

	// 박스
	int m_boxNum;
	int m_boxMotionNum;
	bool m_HeroBoxExist;
	Sprite* m_HeroBox;
	Vector<Sprite*> m_Inventory;
	void GetInvenInfo(Vector<Sprite*> box);
	bool ShowInvenBox();
	void BoxShowMotion(Sprite*);
	void BoxHideMotion();
	bool m_ShowOn;
	void DeleteInvenBox(int tagNum);

	//터치
	int LeftTag;
	int RightTag;
	int JumpTag;
	bool InvenCheck = false;
	bool m_nonTouch;
	std::vector<int> m_dir;
	void DrawDir();
	Vec2 m_TouchPos;
	bool m_TouchCheck;
	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);

	//키보드
	int KeyDirection;
	void MakeKeyListener();
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif 
