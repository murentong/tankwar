#include "Paddle.h"

Paddle::Paddle()
{
}


Paddle::~Paddle()
{
}

CCRect Paddle::rect()
{
	CCSize size = getTexture()->getContentSize();
	return CCRectMake(-size.width / 2, -size.height / 2, size.width, size.height);
}

bool Paddle::containsTouchLocation(CCTouch* touch)
{
	CCPoint ccp = convertTouchToNodeSpaceAR(touch);
	return rect().containsPoint(ccp);
}

bool Paddle::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if (m_state != kPaddleStateUngrabbed) return false;
	if (!containsTouchLocation(touch)) return false;

	m_state = kPaddleStateGrabbed;
	return true;
}

void Paddle::ccTouchMoved(CCTouch* touch, CCEvent* event)
{

}

void Paddle::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	m_state = kPaddleStateUngrabbed;
}

bool Paddle::initWithTexture(CCTexture2D* pTexture)
{
	if (CCSprite::initWithTexture(pTexture))
	{
		m_state = kPaddleStateUngrabbed;
	}

	return true;
}

Paddle* Paddle::paddleWithTexture(CCTexture2D* pTexture)
{
	Paddle *pPaddle = new Paddle();
	pPaddle->initWithTexture(pTexture); 
	pPaddle->autorelease();

	return pPaddle;
}

void Paddle::onEnter()
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	CCSprite::onEnter();
}

void Paddle::onExit()
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}