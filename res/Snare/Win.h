#pragma once
#include "cocos2d.h"

class Player;
class GameManager;

class Win : public cocos2d::Sprite
{
public:
	bool init(cocos2d::Sprite *sprite, Player *player, GameManager *gameManager);
	void update(float dt);

private:
	cocos2d::Sprite *sprite;
	Player *player;
	GameManager *gameManager;
};