#pragma once
#include "cocos2d.h"

class Player;
class GameManager;

class Needle : public cocos2d::Sprite
{
public:
	// ��ʼ����dir�ǰڷŷ���1�ϣ�2�£�3��4��
	bool init(cocos2d::Sprite *sprite,Player *player,GameManager *gameManager,int dir);
	void update(float dt);

private:
	cocos2d::Sprite *sprite;
	Player *player;
	GameManager *gameManager;
	cocos2d::Rect *box;
};