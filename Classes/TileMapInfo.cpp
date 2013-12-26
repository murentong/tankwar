#include "TileMapInfo.h"



void TileMapInfo::initMapInfoWithFile(const char* tmxFile)
{
	mTMXTileMap = CCTMXTiledMap::create(tmxFile);
	mTMXLayers[0] = mTMXTileMap->layerNamed("layer_0");
	mTMXLayers[1] = mTMXTileMap->layerNamed("layer_1");


	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize mapSize = mTMXTileMap->getContentSize();

	//���ŵ�ͼ��������Ļ��С
	mTMXTileMap->setScale(winSize.height / mTMXTileMap->getContentSize().height);

	//����ͼ�ŵ���Ļ�м�
	mTMXTileMap->setPosition(ccp((winSize.width - mapSize.width * mTMXTileMap->getScale()) / 2,
		(winSize.height - mapSize.height * mTMXTileMap->getScale()) / 2));
}

TileMapInfo* TileMapInfo::createMapInfoWithFile(const char* tmxFile)
{
	TileMapInfo* tileMapInfo = new TileMapInfo();
	tileMapInfo->initMapInfoWithFile(tmxFile);

	return tileMapInfo;
}

bool TileMapInfo::collisionTest(CCRect rect)
{
	int gid = 0;
	CCSize mapSize = mTMXTileMap->getContentSize();
	CCSize tileSize = mTMXTileMap->getTileSize();

	//������ͼ����ֱ�ӷ���
	if (rect.getMinX() <= 0.1 || rect.getMaxX() >= mapSize.width ||
		rect.getMinY() <= 0.1 || rect.getMaxY() >= mapSize.height)
		return true;

	//��̹��Y����ת��Ϊ��ͼ�ϵ�Y����
	float MinY = mapSize.height - rect.getMinY();
	float MaxY = mapSize.height - rect.getMaxY();
	//��̹���ĸ����������ײ���
	gid = mTMXLayers[0]->tileGIDAt(ccp((int)(rect.getMinX() / tileSize.width), 
		(int)(MinY / tileSize.height)));
	if (gidToTileType[gid] != tileNone && gidToTileType[gid] != tileGrass)
		return true;
	gid = mTMXLayers[0]->tileGIDAt(ccp((int)(rect.getMinX() / tileSize.width), 
		(int)(MaxY / tileSize.height)));
	if (gidToTileType[gid] != tileNone && gidToTileType[gid] != tileGrass)
		return true;
	gid = mTMXLayers[0]->tileGIDAt(ccp((int)(rect.getMaxX() / tileSize.width), 
		(int)(MaxY / tileSize.height)));
	if (gidToTileType[gid] != tileNone && gidToTileType[gid] != tileGrass)
		return true;
	gid = mTMXLayers[0]->tileGIDAt(ccp((int)(rect.getMaxX() / tileSize.width), 
		(int)(MinY / tileSize.height)));
	if (gidToTileType[gid] != tileNone && gidToTileType[gid] != tileGrass)
		return true;

	return false;
}