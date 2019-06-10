#pragma once

#include "cocos2d.h"

class Player;
class GameManager;

//��صĻָ�ʱ�䣺3s
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

	// �����ڲ���ʱ���Ե���ʱ��
	float timer;
};