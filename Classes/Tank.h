#ifndef __TANK__H__
#define __TANK__H__

#include "cocos2d.h"
using namespace cocos2d;
#include "Panel.h"
#include "TileMapInfo.h"
#include "CommonValue.h"

class Bullet;

class Tank : public CCSprite
{
public :
	Tank();
	~Tank();

	static Tank* createTankWithTankType(const char* tankTypeName, TileMapInfo* tileMapInfo);
	void initTankWithTankType(const char* tankTypeName, TileMapInfo* tileMapInfo);
	void remove();
	bool command(enumOrder order);
	void move();

	void setBlock(bool isBlock);
	bool getBlock();
	
private:
	bool mIsBlock;

	CC_SYNTHESIZE(Bullet*, mBullet, Bullet);
	CC_SYNTHESIZE(TileMapInfo*, mTileMapInfo, TileMapInfo);
	CC_SYNTHESIZE(float, mBulletDelta, BulletDelta);
	CC_SYNTHESIZE(enumObjectType, mObjType, ObjType);
	CC_SYNTHESIZE(CCRect, mMovedRect, MovedRect);
};

#endif