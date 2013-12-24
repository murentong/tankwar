#ifndef __CHOICE__SCENE__H__
#define __CHOICE__SCENE__H__

#include "cocos2d.h"

class ChoiceScene : public cocos2d::CCLayer
{
public :
	ChoiceScene();
	virtual bool init();
	CREATE_FUNC(ChoiceScene);
	static cocos2d::CCScene *scene();

	virtual void keyBackClicked(void);
	virtual void keyMenuClicked(void);

	void touchDownAction(CCObject* sender, unsigned int controlEvent);
private:
	void update();
	int mRound;
};


#endif