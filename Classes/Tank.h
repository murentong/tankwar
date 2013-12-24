#ifndef __TANK__H__
#define __TANK__H__

#include "cocos2d.h"
using namespace cocos2d;
#include "Panel.h"
#include "TileMapInfo.h"

class Bullet;

class Tank : public CCSprite
{
public :
	Tank();
	~Tank();

	static Tank* createTankWithTankType(const char* tankTypeName, TileMapInfo* tileMapInfo);
	void initTankWithTankType(const char* tankTypeName, TileMapInfo* tileMapInfo);
	bool command(enumOrder order);
	
private:
	Bullet* mBullet;
	CC_SYNTHESIZE(TileMapInfo*, mTileMapInfo, TileMapInfo);
	CC_SYNTHESIZE(float, mBulletDelta, BulletDelta);

};

#endif