#pragma once
#include "cocos2d.h"

class Player;

// 需要延迟跟随的时间长度/fps
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
	// states:状态
	// 0:等待触发
	// 1:等待追踪
	// 2:开始追踪
	int states;
	// 等待追踪计数器
	int fcounter;
	// 目标位置记录
	cocos2d::Vec2 targetPoss[FRAMES];
};