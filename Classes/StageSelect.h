#ifndef __STAGE_SELECT_H__
#define __STAGE_SELECT_H__

#include "Default.h"

class StageSelect : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(StageSelect);

	virtual bool onTouchBegan(Touch* touch, Event* unused_event);

};


#endif 
