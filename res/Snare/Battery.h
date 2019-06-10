#pragma once

#include "cocos2d.h"

class Player;
class GameManager;

//电池的恢复时间：3s
static const float TIMER = 3;

class Battery :public cocos2d::Sprite
{
public:
	Battery();
	bool init(cocos2d::Sprite *sprite, Player *player);
	void update(float dt);
	~Battery();

private:
	cocos2d::Sprite *sprite;
	Player *player;

	// 用于内部计时被吃掉的时间
	float timer;
};