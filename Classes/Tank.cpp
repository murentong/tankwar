#include "Tank.h"
#include "Bullet.h"

Tank::Tank() :mBulletDelta(0.0)
{

}

Tank::~Tank()
{

}

Tank* Tank::createTankWithTankType(const char* tankTypeName, TileMapInfo* tileMapInfo)
{
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	pCache->addSpriteFramesWithFile("tank.plist");

	Tank* tank = new Tank();
	tank->initTankWithTankType(tankTypeName, tileMapInfo);
	tank->autorelease();

	return tank;
}

void Tank::initTankWithTankType(const char* tankTypeName, TileMapInfo* tileMapInfo)
{
	initWithSpriteFrameName(tankTypeName);
	mTileMapInfo = tileMapInfo;

	//��̹�˷����ͼ����
	mTileMapInfo->getTileMap()->addChild(this);

	//���ŵ����ʴ�С
	CCTMXTiledMap* tmxTileMap = mTileMapInfo->getTileMap();
	CCSize tileSize = tmxTileMap->getTileSize();
	CCSize tankSize = getContentSize();
	//�ȵ�ͼ��ש��Сһ��
	setScale((tileSize.height * 2-4) / (tankSize.height));

	//��ʼ��̹�˵��ӵ�
	mBullet = Bullet::createBulletWithTank(this);
}

bool Tank::command(enumOrder order)
{
	float stepX = 0.0f;
	float stepY = 0.0f;
	float fRotation = getRotation();

	switch (order)
	{
	case cmdNothing:
		break;
	case cmdGoUP:
		stepY = 1.0f;
		fRotation = 0.0f;
		break;
	case cmdGoDown:
		stepY = -1.0f;
		fRotation = 180.0f;
		break;
	case cmdGoLeft:
		stepX = -1.0f;
		fRotation = 270.0f;
		break;
	case cmdGoRight:
		stepX = 1.0f;
		fRotation = 90.0f;
		break;
	case cmdFire:
		//�����ӵ�����
		return mBullet->fire();
	default:
		break;
	}

	//�������з�����ת̹��
	setRotation(fRotation);

	//����ͼ�ϵ���ײ
	CCRect rect = this->boundingBox();
	if (!mTileMapInfo->collisionTest(CCRectMake(rect.getMinX() + stepX, 
		rect.getMinY() + stepY, rect.size.width, rect.size.height)))
	{
		setPositionX(getPositionX() + stepX);
		setPositionY(getPositionY() + stepY);
		return true;
	}

	return false;
}