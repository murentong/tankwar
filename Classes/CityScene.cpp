#include "CityScene.h"

CityScene::CityScene() :mRound(1)
{
	mPlayerTank[0] = NULL;
	mPlayerTank[1] = NULL;
}

CityScene::~CityScene()
{

}

bool  CityScene::init()
{
	CCLayer::init();

	//初始化地图信息
	char szRound[260];
	sprintf(szRound, "Round%d.tmx", mRound);
	TileMapInfo* tileMapInfo = TileMapInfo::createMapInfoWithFile(szRound);
	CCTMXTiledMap* tmxTileMap = tileMapInfo->getTileMap();
	this->addChild(tmxTileMap);

	//在已有的地图上，创建玩家坦克
	mPlayerTank[0] = Tank::createTankWithTankType("player2U.png", tileMapInfo);

	//放到地图中初始化位置
	CCSize tileSize = tmxTileMap->getTileSize();
	CCSize mapSize = tmxTileMap->getContentSize();
	mPlayerTank[0]->setPosition(ccp(mapSize.width / 2 - tileSize.width * 3, tileSize.height));

	//添加虚拟手柄的显示
	mLayerPanel = Panel::create();
	addChild(mLayerPanel, 3);

	//创建敌人坦克的AI
	mEnemyAI = EnemyAI::createEnemyAIWithTank(mPlayerTank[0]);

	return true;
}

CCScene *CityScene::scene()
{
	CCScene* scene = CCScene::create();
	CityScene* layer = CityScene::create();

	scene->addChild(layer);

	return scene;
}

CCScene *CityScene::scene(int round)
{
	CCScene* scene = CCScene::create();
	CityScene* layer = CityScene::create(round);

	scene->addChild(layer);

	return scene;
}

CityScene *CityScene::create(int round)
{
	CityScene* pRet = new CityScene();
	if (pRet)
	{
		pRet->setRound(round);
		if (pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
	}

	return NULL;
}

void CityScene::onEnter()
{
	CCLayer::onEnter();
	this->scheduleUpdate();
}

void CityScene::update(float delta)
{
	CCLayer::update(delta);
	//将控制面板中的mLayerPanel获取的命令传给坦克
	if (mPlayerTank[0] != NULL)
		mPlayerTank[0]->command(mLayerPanel->getOrder());

	//调用敌人AI的update
	mEnemyAI->update(delta);
}