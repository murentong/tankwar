#ifndef __ENEMY__AI__H__
#define __ENEMY__AI__H__

#include "cocos2d.h"
using namespace cocos2d;
#include "Tank.h"

static const char* enemyTankType[] = 
{
	"normalU.png", "speedU.png", "armor1U.png"
};

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

class EnemyAI
{
public:
	EnemyAI();
	~EnemyAI();

	static EnemyAI* createEnemyAIWithTank(Tank* tank);
	void initEnemyAIWithTank(Tank* tank);
	void update(float delta);

	void collisionTest();
	bool IsRectIntersect(CCRect rectA, CCRect rectB);

private:
	void addTank(float delta);
	void tankAction(float delta);
	void move();

private:
	CCArray* mEnemyTanks;
	Tank* mTank;
	TileMapInfo* mTileMapInfo;

	//出现地点
	CCPoint bornPoint[3];
};

#endif