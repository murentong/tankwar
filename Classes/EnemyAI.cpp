#include "EnemyAI.h"

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

		//���̹��֮�����ײ
		collisionTest();

		//���̹����������������
		if (tank->getBlock())
			tank->setRotation((int)(CCRANDOM_0_1() * 3.2) * 90);
		//���������ж���ɺ�̹�˸����Լ�������״̬�ƶ�
		tank->move();
	}
	mTank->move();
}

void EnemyAI::collisionTest()
{
	//̹�˺͵з�̹��֮�����ײ���
	CCObject* pObj;
	CCARRAY_FOREACH(mEnemyTanks, pObj)
	{
		Tank* enemyTank = (Tank*)pObj;
		if (IsRectIntersect(mTank->getMovedRect(), enemyTank->getMovedRect()))
		{
			enemyTank->setBlock(true);
			mTank->setBlock(true);
		}
	}

	//�з�̹��֮�����ײ
	CCArray* ccTmpArray = CCArray::create();
	ccTmpArray->addObjectsFromArray(mEnemyTanks);
	while (ccTmpArray->count())
	{
		CCObject* pObj;
		Tank* tmpTank = (Tank*)ccTmpArray->lastObject();
		ccTmpArray->removeLastObject();
		CCARRAY_FOREACH(ccTmpArray, pObj)
		{
			if (IsRectIntersect(tmpTank->getMovedRect(), ((Tank*)pObj)->getMovedRect()))
			{
				tmpTank->setBlock(true);
				((Tank*)pObj)->setBlock(true);
				ccTmpArray->removeObject(pObj);
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