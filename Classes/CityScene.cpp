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

	//��ʼ����ͼ��Ϣ
	char szRound[260];
	sprintf(szRound, "Round%d.tmx", mRound);
	TileMapInfo* tileMapInfo = TileMapInfo::createMapInfoWithFile(szRound);
	CCTMXTiledMap* tmxTileMap = tileMapInfo->getTileMap();
	this->addChild(tmxTileMap);

	//�����еĵ�ͼ�ϣ��������̹��
	mPlayerTank[0] = Tank::createTankWithTankType("player2U.png", tileMapInfo);

	//�ŵ���ͼ�г�ʼ��λ��
	CCSize tileSize = tmxTileMap->getTileSize();
	CCSize mapSize = tmxTileMap->getContentSize();
	mPlayerTank[0]->setPosition(ccp(mapSize.width / 2 - tileSize.width * 3, tileSize.height));

	//��������ֱ�����ʾ
	mLayerPanel = Panel::create();
	addChild(mLayerPanel, 3);

	//��������̹�˵�AI
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
	//����������е�mLayerPanel��ȡ�������̹��
	if (mPlayerTank[0] != NULL)
		mPlayerTank[0]->command(mLayerPanel->getOrder());

	//���õ���AI��update
	mEnemyAI->update(delta);
}