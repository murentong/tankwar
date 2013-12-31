#include "EnemyAI.h"
#include "Bullet.h"

EnemyAI::EnemyAI()
{

}

EnemyAI::~EnemyAI()
{

}

EnemyAI* EnemyAI::createEnemyAIWithTank(Tank* tank)
{
	EnemyAI* enemyTank = new EnemyAI();
	enemyTank->initEnemyAIWithTank(tank);

	return enemyTank;
}

void EnemyAI::initEnemyAIWithTank(Tank* tank)
{
	mTank = tank;
	mTileMapInfo = tank->getTileMapInfo();
	mEnemyTanks = CCArray::createWithCapacity(4);
	mEnemyTanks->retain();

	//初始化出现地点
	CCSize mapSize = mTileMapInfo->getTileMap()->getContentSize();
	CCSize tileSize = mTileMapInfo->getTileMap()->layerNamed("layer_0")->getMapTileSize();
	bornPoint[0] = ccp(tileSize.width, mapSize.height - tileSize.height);
	bornPoint[1] = ccp(mapSize.width / 2, mapSize.height - tileSize.height);
	bornPoint[2] = ccp(mapSize.width - tileSize.width, mapSize.height - tileSize.height);
}

void EnemyAI::update(float delta)
{
	//坦克不足4个，补充坦克
	addTank(delta);

	//坦克行为控制
	tankAction(delta);
}

void EnemyAI::addTank(float delta)
{
	static float deltaTimes = 0.0f;
	deltaTimes += delta;
	if (deltaTimes >= 2.0f)
	{
		deltaTimes = 0.0f;
		int count = mEnemyTanks->count();
		if (count < 3)	//先从固定位置添加三个坦克
		{
			Tank* enemyTank = Tank::createTankWithTankType(enemyTankType[count], mTileMapInfo);
			enemyTank->setPosition(bornPoint[count]);
			enemyTank->setRotation(180.0f);
			mEnemyTanks->addObject(enemyTank);
		}
		else if (count == 3)	//第四个坦克随机添加
		{
			int tankTypeIndex = (int)(CCRANDOM_0_1() * 4) % 3;
			Tank* enemyTank = Tank::createTankWithTankType(enemyTankType[tankTypeIndex], mTileMapInfo);
			enemyTank->setPosition(bornPoint[tankTypeIndex]);
			enemyTank->setRotation(180.0f);
			mEnemyTanks->addObject(enemyTank);
		}
	}
}

void EnemyAI::tankAction(float delta)
{
	CCObject* pObj;
	CCARRAY_FOREACH(mEnemyTanks, pObj)
	{
		Tank* tank = (Tank*)pObj;

		//坦克按照上次的方向一直往前走
		int Rotation = tank->getRotation();
		tank->command((enumOrder)(Rotation / 90 + 1));

		//坦克每隔一秒开一次火
		tank->setBulletDelta(tank->getBulletDelta() + delta);
		if (tank->getBulletDelta() > 1)
		{
			//开火后，如果子弹在飞行中，归零计时
			if (tank->command(cmdFire))
			{
				tank->setBulletDelta(0.0);
			}
		}
	}
	//检测坦克之间的碰撞
	collisionTest();
	move();
}

void EnemyAI::move()
{
	mTank->move();
	CCObject* pObj;
	CCARRAY_FOREACH(mEnemyTanks, pObj)
	{
		Tank* tank = (Tank*)pObj;
		//如果坦克阻塞，换个方向
		if (tank->getBlock())
			tank->setRotation((int)(CCRANDOM_0_1() * 3.8) * 90);
		//如果上面的判断完成后，坦克根据自己的阻塞状态移动
		tank->move();
	}
}

void EnemyAI::collisionTest()
{
	Bullet* playerBullet = mTank->getBullet();

	//坦克之间的碰撞
	CCArray* ccTmpArray = CCArray::create();
	ccTmpArray->addObjectsFromArray(mEnemyTanks);
	ccTmpArray->addObject(mTank);
	CCObject* pObjSrc;
	CCARRAY_FOREACH(ccTmpArray, pObjSrc)
	{
		Tank* tankSrc = (Tank*)pObjSrc;
		CCObject* pObjdst;
		CCARRAY_FOREACH(ccTmpArray, pObjdst)
		{
			Tank* tankDst = (Tank*)pObjdst;
			if (tankSrc != tankDst)
			{
				CCRect rectDst;
				if (ccTmpArray->indexOfObject(pObjSrc) > ccTmpArray->indexOfObject(pObjdst))
				{
					//之前的坦克已经确定了要移动的位置
					rectDst = tankDst->getMovedRect();
				}
				else
				{
					//还没有确定要移动的坦克
					rectDst = tankDst->boundingBox();
				}
				if (IsRectIntersect(tankSrc->getMovedRect(), rectDst))
				{
					//确保在移动之前没有重合
					if (!IsRectIntersect(tankSrc->boundingBox(), rectDst))
					{
						tankSrc->setBlock(true);
					}
				}
			}
		}
	}

	//玩家子弹和敌方子弹之间的碰撞
	CCObject* pObj;
	CCARRAY_FOREACH(mEnemyTanks, pObj)
	{
		Bullet* enemyBullet = ((Tank*)pObj)->getBullet();
		if (playerBullet->getFlyState() && enemyBullet->getFlyState())
		{
			if (IsRectIntersect(playerBullet->getMovedRect(), enemyBullet->getMovedRect()))
			{
				enemyBullet->stopFire();
				playerBullet->stopFire();
			}
		}
	}

	//玩家子弹和敌方坦克的碰撞
	CCARRAY_FOREACH(mEnemyTanks, pObj)
	{
		if (playerBullet->getFlyState())
		{
			Tank* enemyTank = (Tank*)pObj;
			if (IsRectIntersect(playerBullet->getMovedRect(), enemyTank->getMovedRect()))
			{
				mEnemyTanks->removeObject(pObj);
				enemyTank->remove();
				playerBullet->stopFire();
				break;
			}
		}
	}

	//敌人子弹和玩家的碰撞
	CCARRAY_FOREACH(mEnemyTanks, pObj)
	{
		Bullet* enemyBullet = ((Tank*)pObj)->getBullet();
		if (enemyBullet->getFlyState())
		{
			if (IsRectIntersect(enemyBullet->getMovedRect(), mTank->getMovedRect()))
			{
				//mTank->setVisible(false);
				enemyBullet->stopFire();
				break;
			}
		}
	}
}

bool EnemyAI::IsRectIntersect(CCRect rectA, CCRect rectB)
{
	float left = max(rectA.getMinX(), rectB.getMinX());
	float right = min(rectA.getMaxX(), rectB.getMaxX());
	if (left > right)
		return false;

	float top = min(rectA.getMaxY(), rectB.getMaxY());
	float bottom = max(rectA.getMinY(), rectB.getMinY());
	if (top < bottom)
		return false;

	return true;
}