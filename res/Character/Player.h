///玩家操控的角色类

#pragma once

#include"proj.win32\res\GameManager.h"

using namespace std;

static const int PLAYER_TAG = 110;
static const int  PLAYER_WIDTH = 100;
static const int  PLAYER_HEIGHT = 100;
static const int  PLAYER_SPEEDX = 3;
static const int  PLAYER_SPEEDY = 3;

// 序列帧动画路径数组
static string PLAYER_IMG_PATH[4] = {
	"Character/player/player1.png",
	"Character/player/player2.png",
	"Character/player/player3.png",
	"Character/player/player4.png"
};

// 动画状态量
enum AniState {
	IDLE = 0,	// 常态
	RUN = 1,	// 跑步
	JUMP = 2,	// 跳跃中
	FALL = 3,	// 下降中
	WALL = 4	// 滑墙中
};

class Player
{
public:
	Player();
	~Player();
	// 初始化
	void init(Vec2 pos);
	// 设置将要播放的动画状态
	void setAnimation(AniState state);
	// 按照速度方向移动
	void move(Vec2 speed);
	// 以速度向目的地移动
	bool moveTo(Vec2 pos, Vec2 speed);
	// 设置当前面向的方向
	void setDir(Dir dir);
	// 返回此精灵
	Sprite* getSpite();
	// 奔跑速度
	Vec2 runSpeed = Vec2(PLAYER_SPEEDX, PLAYER_SPEEDY);

private:
	AniState nowAni = AniState::FALL;
	int life;
	int energy;
	bool isGround = false;;
	Dir dir = Dir::RIGHT;
	Animate* animate = nullptr;
	Sprite* center = nullptr;
};