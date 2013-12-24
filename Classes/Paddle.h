#ifndef __PADDLE__H__
#define __PADDLE__H__
#include "cocos2d.h"

using namespace cocos2d;

typedef enum tagPaddleState
{
	kPaddleStateGrabbed,
	kPaddleStateUngrabbed
} PaddleState;

class Paddle :
	public CCSprite, public CCTargetedTouchDelegate
{
	CC_SYNTHESIZE(PaddleState, m_state, state);
public:
	Paddle();
	~Paddle();

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

	virtual bool initWithTexture(CCTexture2D* pTexture);
	static Paddle* paddleWithTexture(CCTexture2D* pTexture);
	CCRect rect();
	bool containsTouchLocation(CCTouch* touch);

	virtual void onEnter();
	virtual void onExit();
};

#endif