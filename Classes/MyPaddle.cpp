#include "MyPaddle.h"


MyPaddle::MyPaddle() :bFlag(false)
{
}


MyPaddle::~MyPaddle()
{
}

bool MyPaddle::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if (Paddle::ccTouchBegan(touch, event))
	{
		bFlag = true;
		return true;
	}
	return false;
}

void MyPaddle::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	Paddle::ccTouchEnded(touch, event);
	if (bFlag)
	{
		bFlag = false;
		m_pSence->touchDownAction(this, m_evttyp);
	}
}

MyPaddle* MyPaddle::paddleWithTexture(CCTexture2D* pTexture) 
{
	MyPaddle* pPaddle = new MyPaddle();
	pPaddle->initWithTexture(pTexture);
	pPaddle->autorelease();
	return pPaddle;
}