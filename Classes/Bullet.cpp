#include "Bullet.h"

Bullet::Bullet() :mFlyState(false)
{
}

Bullet::~Bullet()
{

}

Bullet* Bullet::createBulletWithTank(Tank* tank)
{
	Bullet* bullet = new Bullet();
	bullet->initBulletWithTank(tank);
	bullet->autorelease();

	return bullet;
}

void Bullet::initBulletWithTank(Tank* tank)
{
	mTank = tank;
	mTileMapInfo = mTank->getTileMapInfo();

	initWithSpriteFrameName("bullet.png");
	setScale(1.5 / 3.0);
	setVisible(false);
	mTileMapInfo->getTileMap()->addChild(this);

}

bool Bullet::fire()
{
	if (!mFlyState)
	{
		mFlyState = true;
		setVisible(true);
		setPosition(mTank->getPosition());
		//�����ӵ����з���
		int Rotation = mTank->getRotation();
		switch (Rotation)
		{
		case 0:
			stepX = 0.0f, stepY = 2.0f;
			break;
		case 90:
			stepX = 2.0f, stepY = 0.0f;
			break;
		case 180:
			stepX = 0.0f, stepY = -2.0f;
			break;
		case 270:
			stepX = -2.0f, stepY = 0.0f;
			break;
		default:
			break;
		}
		scheduleUpdate();

		return true;
	}

	return false;
}

bool Bullet::stopFire()
{
	if (mFlyState == true)
	{
		mFlyState = false;
		setVisible(false);
		unscheduleUpdate();

		return true;
	}

	return false;
}

void Bullet::bulletBoom()
{
	CCRect rect = boundingBox();
	CCSize mapSize = mTileMapInfo->getTileMap()->getContentSize();

	if (rect.getMinX() < 0 || rect.getMaxX() >= mapSize.width ||
		rect.getMinY() < 0 || rect.getMaxY() >= mapSize.height)
		return ;

	CCTMXLayer* tmxLayer = mTileMapInfo->getTileMap()->layerNamed("layer_0");
	CCSize tileSize = tmxLayer->getMapTileSize();

	//����Y��λtmx��ͼ�е�Y��
	float MinY = mapSize.height - rect.getMinY();
	float MaxY = mapSize.height - rect.getMaxY();

	//�����ӵ���ײ��ǽ��tileWallͼ��ɾ��
	CCPoint pt = ccp((int)rect.getMinX() / tileSize.width,(int)(MinY / tileSize.height));
	if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileWall)
		tmxLayer->setTileGID(gidToTileType[tileNone], pt);

	pt = ccp((int)rect.getMinX() / tileSize.width,(int)(MaxY / tileSize.height));
	if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileWall)
		tmxLayer->setTileGID(gidToTileType[tileNone], pt);

	pt = ccp((int)rect.getMaxX() / tileSize.width,(int)(MinY / tileSize.height));
	if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileWall)
		tmxLayer->setTileGID(gidToTileType[tileNone], pt);

	pt = ccp((int)rect.getMaxX() / tileSize.width,(int)(MaxY / tileSize.height));
	if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileWall)
		tmxLayer->setTileGID(gidToTileType[tileNone], pt);
}

void Bullet::update(float delta)
{
	CCSprite::update(delta);

	//�����ƶ����λ��
	mMovedRect = boundingBox();
	mMovedRect.origin.x += stepX;
	mMovedRect.origin.y += stepY;
	setPosition(ccp(getPositionX() + stepX, getPositionY() + stepY));

	//����Ƿ�����ײ
	CCRect rect = boundingBox();
	if (mTileMapInfo->collisionTest(rect))
	{
		//ֹͣ����
		stopFire();
		//�����ӵ�
		bulletBoom();
	}
}

