#ifndef __MainScene__
#define __MainScene__

#include "cocos2d.h"
#include "Player.h"
#include "Progress.h"

USING_NS_CC;

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(MainScene);
    bool onTouchBegan(Touch* touch, Event* event);
    void attackCallback(Ref* pSender);
private:
    Player* _hero;
    Player* _enemy;
    EventListenerTouchOneByOne* _listener_touch;
    Progress* _progress;
};

#endif
