#pragma once
#include "cocos2d.h"

class Player;
class GameManager;

class Needle : public cocos2d::Sprite
{
public:
	// 初始化，dir是摆放方向，1上，2下，3左，4右
	bool init(cocos2d::Sprite *sprite,Player *player,GameManager *gameManager,int dir);
	void update(float dt);

private:
	cocos2d::Sprite *sprite;
	Player *player;
	GameManager *gameManager;
	cocos2d::Rect *box;
};