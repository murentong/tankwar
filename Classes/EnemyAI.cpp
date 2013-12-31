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

	//��ʼ�����ֵص�
	CCSize mapSize = mTileMapInfo->getTileMap()->getContentSize();
	CCSize tileSize = mTileMapInfo->getTileMap()->layerNamed("layer_0")->getMapTileSize();
	bornPoint[0] = ccp(tileSize.width, mapSize.height - tileSize.height);
	bornPoint[1] = ccp(mapSize.width / 2, mapSize.height - tileSize.height);
	bornPoint[2] = ccp(mapSize.width - tileSize.width, mapSize.height - tileSize.height);
}

void EnemyAI::update(float delta)
{
	//̹�˲���4��������̹��
	addTank(delta);

	//̹����Ϊ����
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
		if (count < 3)	//�ȴӹ̶�λ���������̹��
		{
			Tank* enemyTank = Tank::createTankWithTankType(enemyTankType[count], mTileMapInfo);
			enemyTank->setPosition(bornPoint[count]);
			enemyTank->setRotation(180.0f);
			mEnemyTanks->addObject(enemyTank);
		}
		else if (count == 3)	//���ĸ�̹��������
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

		//̹�˰����ϴεķ���һֱ��ǰ��
		int Rotation = tank->getRotation();
		tank->command((enumOrder)(Rotation / 90 + 1));

		//̹��ÿ��һ�뿪һ�λ�
		tank->setBulletDelta(tank->getBulletDelta() + delta);
		if (tank->getBulletDelta() > 1)
		{
			//���������ӵ��ڷ����У������ʱ
			if (tank->command(cmdFire))
			{
				tank->setBulletDelta(0.0);
			}
		}
	}
	//���̹��֮�����ײ
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
		//���̹����������������
		if (tank->getBlock())
			tank->setRotation((int)(CCRANDOM_0_1() * 3.8) * 90);
		//���������ж���ɺ�̹�˸����Լ�������״̬�ƶ�
		tank->move();
	}
}

void EnemyAI::collisionTest()
{
	Bullet* playerBullet = mTank->getBullet();

	//̹��֮�����ײ
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
					//֮ǰ��̹���Ѿ�ȷ����Ҫ�ƶ���λ��
					rectDst = tankDst->getMovedRect();
				}
				else
				{
					//��û��ȷ��Ҫ�ƶ���̹��
					rectDst = tankDst->boundingBox();
				}
				if (IsRectIntersect(tankSrc->getMovedRect(), rectDst))
				{
					//ȷ�����ƶ�֮ǰû���غ�
					if (!IsRectIntersect(tankSrc->boundingBox(), rectDst))
					{
						tankSrc->setBlock(true);
					}
				}
			}
		}
	}

	//����ӵ��͵з��ӵ�֮�����ײ
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

	//����ӵ��͵з�̹�˵���ײ
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

	//�����ӵ�����ҵ���ײ
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