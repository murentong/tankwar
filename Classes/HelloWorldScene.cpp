#include "HelloWorldScene.h"
#include "ChoiceScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	//1.����Android���غͲ˵�����
	setKeypadEnabled(true);

	//2.�����رղ˵���
	CCMenuItemImage	*pCloseItem = CCMenuItemImage::create(
		"CloseNormal.png", "CloseSelected.png", this, menu_selector(HelloWorld::menuCloseCallback));

	//3.��ȡ��������Ĵ�С��ԭ��
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//4.���ò˵�λ�õ����½�
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width / 2, 
		origin.y + visibleSize.height - pCloseItem->getContentSize().height/2));
	CCMenu *pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	//5.����һ����play game����ť����ʼ��Ϸ,����λ�ã�Ȼ���������ű�������Ӧ��Ļ,
	//Ȼ����Ӳ˵���˵��У��������
	CCMenuItemImage *pItemPlay = CCMenuItemImage::create(
		"playgameNormal.png", "playgameClose.png", this, menu_selector(HelloWorld::menuPlayGameCallback));
	pItemPlay->setPosition(ccp(visibleSize.width / 2, visibleSize.height*1.0f / 4.0f));
	pItemPlay->setScaleX(visibleSize.width / 600);
	pItemPlay->setScaleY(visibleSize.height / 400);
	
	CCMenu *pMenuPlay = CCMenu::create(pItemPlay, NULL);
	pMenuPlay->setPosition(CCPointZero);
	this->addChild(pMenuPlay, 1);

	//6.����һ�������ı�������
	CCSprite *pSprite = CCSprite::create("ScenceStart.png");
	pSprite->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
	CCSize sz = pSprite->getContentSize();
	pSprite->setScaleX(visibleSize.width / sz.width);
	pSprite->setScaleY(visibleSize.height / sz.height);
	this->addChild(pSprite, 0);

    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	//�رղ˵������ʱ����
	CCDirector::sharedDirector()->end();
}

void HelloWorld::menuPlayGameCallback(CCObject* pSender)
{
	//��ʼ�˵������ʱ����
	CCScene* pScene = ChoiceScene::scene();
	CCDirector::sharedDirector()->pushScene(pScene);
	return;
}

void HelloWorld::keyBackClicked()
{
	CCLayer::keyBackClicked();
	CCDirector::sharedDirector()->end();
}

void HelloWorld::keyMenuClicked()
{
	CCLayer::keyMenuClicked();
}
