#ifndef __BULLET__H__
#define __BULLET__H__

#include "cocos2d.h"
using namespace cocos2d;
#include "Tank.h"

class Bullet : public CCSprite
{
public:
	Bullet();
	~Bullet();

	static Bullet* createBulletWithTank(Tank* tank);
	void initBulletWithTank(Tank* tank);
	bool fire();
	bool stopFire();
	virtual void update(float delta);
	void bulletBoom();

private:
	Tank* mTank;
	TileMapInfo* mTileMapInfo;
	float stepX, stepY;

	CC_SYNTHESIZE(CCRect, mMovedRect, MovedRect);
	CC_SYNTHESIZE(bool, IsBlock, Block);
	CC_SYNTHESIZE(bool, mFlyState, FlyState);
};

#endif