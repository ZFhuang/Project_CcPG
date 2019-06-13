#pragma once
#include "cocos2d.h"

class Player;

// ��Ҫ�ӳٸ����ʱ�䳤��/fps
static const int FRAMES = 10;

class Key : public cocos2d::Sprite
{
public:
	bool init(cocos2d::Sprite *sprite, Player *player);
	//void setTag(int tag);
	void update(float dt);

private:
	cocos2d::Sprite *sprite;
	Player *player;
	Sprite *target;
	//int tag;
	// states:״̬
	// 0:�ȴ�����
	// 1:�ȴ�׷��
	// 2:��ʼ׷��
	int states;
	// �ȴ�׷�ټ�����
	int fcounter;
	// Ŀ��λ�ü�¼
	cocos2d::Vec2 targetPoss[FRAMES];
};