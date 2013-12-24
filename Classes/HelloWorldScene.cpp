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

	//1.设置Android返回和菜单按键
	setKeypadEnabled(true);

	//2.创建关闭菜单项
	CCMenuItemImage	*pCloseItem = CCMenuItemImage::create(
		"CloseNormal.png", "CloseSelected.png", this, menu_selector(HelloWorld::menuCloseCallback));

	//3.获取可视区域的大小和原点
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//4.设置菜单位置到右下角
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width / 2, 
		origin.y + visibleSize.height - pCloseItem->getContentSize().height/2));
	CCMenu *pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	//5.创建一个“play game”按钮来开始游戏,设置位置，然后设置缩放比例来适应屏幕,
	//然后添加菜单项到菜单中，加入层中
	CCMenuItemImage *pItemPlay = CCMenuItemImage::create(
		"playgameNormal.png", "playgameClose.png", this, menu_selector(HelloWorld::menuPlayGameCallback));
	pItemPlay->setPosition(ccp(visibleSize.width / 2, visibleSize.height*1.0f / 4.0f));
	pItemPlay->setScaleX(visibleSize.width / 600);
	pItemPlay->setScaleY(visibleSize.height / 400);
	
	CCMenu *pMenuPlay = CCMenu::create(pItemPlay, NULL);
	pMenuPlay->setPosition(CCPointZero);
	this->addChild(pMenuPlay, 1);

	//6.创建一个启动的背景界面
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
	//关闭菜单被点击时调用
	CCDirector::sharedDirector()->end();
}

void HelloWorld::menuPlayGameCallback(CCObject* pSender)
{
	//开始菜单被点击时调用
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
