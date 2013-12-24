#ifndef __PANEL__H__
#define __PANEL__H__

#include "Paddle.h"
#include "cocos2d.h"

#define RES_PADDLE_LEFT			"paddle/left.png"
#define RES_PADDLE_LEFT_PRESS	"paddle/left_press.png"
#define RES_PADDLE_RIGHT		"paddle/right.png"
#define RES_PADDLE_RIGHT_PRESS	"paddle/right_press.png"
#define RES_PADDLE_UP			"paddle/top.png"
#define RES_PADDLE_UP_PRESS		"paddle/top_press.png"
#define RES_PADDLE_DOWN			"paddle/buttom.png"
#define RES_PADDLE_DOWN_PRESS	"paddle/buttom_press.png"
#define RES_PADDLE_FIRE			"paddle/fire.png"
#define RES_PADDLE_FIRE_PRESS	"paddle/fire_press.png"

enum enumOrder
{
	cmdNothing,
	cmdGoUP,
	cmdGoRight,
	cmdGoDown,
	cmdGoLeft,
	cmdFire
};

class Panel : public CCLayer
{
public:
	virtual bool init();
	CREATE_FUNC(Panel);
	enumOrder getOrder();
protected:
	Paddle* m_pPaddleUp;
	Paddle* m_pPaddleDown;
	Paddle* m_pPaddleLeft;
	Paddle* m_pPaddleRight;
	Paddle* m_pPaddleFire;
};

#endif