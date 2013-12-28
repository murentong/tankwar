#include "Tank.h"
#include "Bullet.h"

Tank::Tank() :mBulletDelta(0.0), mIsBlock(false)
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

	//将坦克放入地图层中
	mTileMapInfo->getTileMap()->addChild(this);

	//缩放到合适大小
	CCTMXTiledMap* tmxTileMap = mTileMapInfo->getTileMap();
	CCSize tileSize = tmxTileMap->getTileSize();
	CCSize tankSize = getContentSize();
	//比地图上砖块小一点
	setScale((tileSize.height * 2-4) / (tankSize.height));

	//初始化坦克的子弹
	mBullet = Bullet::createBulletWithTank(this);
}

void Tank::remove()
{
	mTileMapInfo->getTileMap()->removeChild(this);
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
		//调用子弹开火
		return mBullet->fire();
	default:
		break;
	}

	//根据运行方向旋转坦克
	setRotation(fRotation);

	CCRect rect = this->boundingBox();
	mMovedRect.setRect(rect.getMinX() + stepX,rect.getMinY() + stepY, 
		rect.size.width, rect.size.height);
	//检测地图上的碰撞
	if (!mTileMapInfo->collisionTest(mMovedRect))
	{
		setBlock(false);
		return true;
	}
	//如果碰撞了就不要移动,设置为阻塞状态
	setBlock(true);

	return false;
}

void Tank::move()
{
	setPosition(ccp(mMovedRect.getMidX(), mMovedRect.getMidY()));
}

void Tank::setBlock(bool isBlock)
{
	mIsBlock = isBlock;
	if (mIsBlock == true)
	{
		mMovedRect = boundingBox();
	}
}

bool Tank::getBlock()
{
	return mIsBlock;
}