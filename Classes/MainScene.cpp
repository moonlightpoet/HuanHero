#include "MainScene.h"

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}
bool MainScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/role.plist","images/role.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/ui.plist","images/ui.pvr.ccz");

    Sprite* background = Sprite::create("images/background.png");
    background->setPosition(origin + visibleSize/2);
    this->addChild(background);

    //add player
	_hero = Player::create(Player::PlayerType::HERO);
	_hero->setPosition(origin.x + _hero->getContentSize().width/2, origin.y + visibleSize.height/2);
	this->addChild(_hero);

	//add enemy1
	_enemy = Player::create(Player::PlayerType::ENEMY);
	_enemy->setPosition(origin.x + visibleSize.width - _enemy->getContentSize().width/2, origin.y + visibleSize.height/2);
	this->addChild(_enemy);

    _hero->playAnimationForever("stay");
    _enemy->playAnimationForever("stay");

    _listener_touch = EventListenerTouchOneByOne::create();
	_listener_touch->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener_touch, this);

	auto attackItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MainScene::attackCallback, this));

    attackItem->setPosition(Vec2(origin.x + visibleSize.width - attackItem->getContentSize().width/2 ,
                                origin.y + attackItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(attackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    _progress = Progress::create("player-progress-bg.png","player-progress-fill.png");
    _progress->setPosition(_progress->getContentSize().width/2, this->getContentSize().height - _progress->getContentSize().height/2);
    this->addChild(_progress);

    return true;
}
void MainScene::menuCloseCallback(cocos2d::Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool MainScene::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 pos = this->convertToNodeSpace(touch->getLocation());
	_hero->walkTo(pos);
	log("MainScene::onTouchBegan");
	_enemy->autoDoAction(_hero);
	return true;
}

void MainScene::attackCallback(Ref* pSender)
{
    _hero->stopAllActions();
    _hero->playAnimation("attack");
    Vec2 del = _hero->getPosition() - _enemy->getPosition();
    float distance = del.length();
    log(String::createWithFormat("distance == %f", distance)->getCString());
    if (distance <= 100.0) {
        _enemy->getHit();
    }
    _enemy->autoAttack(_hero);
    _progress->setProgress(_hero->getProgress()->getProgress());
}


