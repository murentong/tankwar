#include "ChoiceScene.h"
#include "MyPaddle.h"
#include "CommonValue.h"
#include "CityScene.h"

using namespace cocos2d;

ChoiceScene::ChoiceScene() :mRound(1)
{

}

CCScene *ChoiceScene::scene()
{
	CCScene *scene = CCScene::create();
	ChoiceScene *layer = ChoiceScene::create();
	if (scene && layer)
	{
		scene->addChild(layer);
	}

	return scene;
}

bool ChoiceScene::init()
{
	// 1. ��ʼ�����࣬������Android���ؼ��Ͳ˵���
	if (!CCLayer::init())
	{
		return false;
	}
	setKeypadEnabled(true);
	
	//2.���һ����ɫ����,�����б�������
	CCSize szWin = CCDirector::sharedDirector()->getWinSize();
	CCSprite *pSprite = CCSprite::create("Black.png");
	pSprite->setPosition(ccp(szWin.width / 2, szWin.height / 2));
	CCSize szSprite = pSprite->getContentSize();
	pSprite->setScaleX(szWin.width / szSprite.width);
	pSprite->setScaleY(szWin.height / szSprite.height);
	this->addChild(pSprite, 0);

	//3.���ÿ�ʼ��Ϸ����һ�أ���һ�ذ�ť�Լ��ؿ�����ʾ��ͼƬ
	//�Լ��������ǵ����ű�����λ��
	const char *szImgs[4] = { "paddle/btn_play.png", "paddle/btn_rewind.png",
		"paddle/btn_fast_forward.png", "fonts/fps_images.png" };
	float fSettings[4][4] = {
		{ 20.0f / 480, 16.0f / 320, 280.0f / 480, 160.0f / 320 },
		{ 20.0f / 480, 16.0f / 320, 200.0f / 480, 160.0f / 320 },
		{ 20.0f / 480, 16.0f / 320, 240.0f / 480, 160.0f / 320 },
		{ 20.0f / 480, 22.0f / 320, 280.0f / 480, 172.0f / 320 }
	};

	//4.�ֱ�Կ�ʼ��Ϸ����һ�أ���һ�ذ�ť��ͼƬ����������ʾ������λ��
	enum_evt evts[4] = {evt_start, evt_pressA, evt_pressB, evt_text};
	for (int i = 0; i < 3; ++i)
	{
		float* fSetting = fSettings[i];
		CCTexture2D* paddleTexture = CCTextureCache::sharedTextureCache()->addImage(szImgs[i]);
		//CCSprite* pPaddle = CCSprite::createWithTexture(paddleTexture);
		MyPaddle* pPaddle = MyPaddle::paddleWithTexture(paddleTexture);
		CCSize szBtn = pPaddle->getContentSize();
		pPaddle->setScaleX(szWin.width / szBtn.width * fSetting[0]);
		pPaddle->setScaleY(szWin.height / szBtn.height * fSetting[1]);
		pPaddle->setPosition(ccp(szWin.width * fSetting[2], szWin.height * fSetting[3]));
		addChild(pPaddle);

		//���õ�ǰ������MyPaddle��,Ȼ�������¼����͵�MyPaddle��
		pPaddle->setpSence(this);
		pPaddle->setevttyp(evts[i]);
	}

	//5.����ʵ�ؿ������ֽ��в�����������ʾ����Ϊ1��
	//����ê�㣬�������ţ�����λ���Ѿ���ɫ��
	int i = 3;
	ccColor3B color = { 0, 0, 0 };
	float* fSetting = fSettings[i];
	CCLabelAtlas* label1 = CCLabelAtlas::create("1", szImgs[i], 16, 32, '.');

	CCSize sz = label1->getContentSize();
	label1->setAnchorPoint(ccp(0.5f, 0.5f));
	label1->setScaleX(szWin.width / sz.width * fSetting[0]);
	label1->setScaleY(szWin.height / sz.height * fSetting[1]);
	label1->setPosition(ccp(szWin.width * fSetting[2], szWin.height * fSetting[3]));
	label1->setColor(color);
	addChild(label1, 0, evt_text);

	return true;
}

void ChoiceScene::keyBackClicked(void)
{

}

void ChoiceScene::keyMenuClicked(void)
{

}

void ChoiceScene::touchDownAction(CCObject* sender, unsigned int controlEvent)
{
	if (controlEvent == evt_pressA)
	{
		mRound = 1 + (mRound - 1 + ROUNDS - 1) % ROUNDS;
		update();
	}
	else if (controlEvent == evt_pressB)
	{
		mRound = 1 + (mRound + 1 + ROUNDS - 1) % ROUNDS;
		update();
	}
	else if (controlEvent == evt_start)
	{
		//��ʼ��Ӧ�ؿ��ĳ���,�Ժ����
		CCScene* scene = CityScene::scene(mRound);
		CCDirector::sharedDirector()->replaceScene(scene);
	}
}

void ChoiceScene::update()
{
	char szTemp[260];
	CCLabelAtlas* label1 = (CCLabelAtlas*)getChildByTag(evt_text);
	sprintf(szTemp, "%d", mRound);
	label1->setString(szTemp);
}

//CCScene *pScene = CCScence_round(mRound - 1);
//CCDirector::sharedDirector()->replaceScene(pScene);