///玩家操控的角色类

#pragma once

#include"proj.win32\res\GameManager.h"

using namespace std;

static const int PLAYER_TAG = 110;
static int  PLAYER_WIDTH = 40;
static int  PLAYER_HEIGHT = 50;
static const float JUMP_ACCE = 1.2;
static const float FALL_ACCE = 1.2;
static const float  MAX_PLAYER_SPEED_X = 10;
static const float  MAX_SPEED_JUMP = 7;
static const float  MAX_SPEED_FALL = 7;
// 停止的惯性
static const float  SLOW_DOWN_X = 5;

// 序列帧动画路径数组
static string PLAYER_IMG_PATH[4] = {
	"Character/player/run1.png",
	"Character/player/run2.png",
	"Character/player/run3.png",
	"Character/player/run4.png"
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
	// 以速度向目的地移动
	bool moveTo(Vec2 pos, Vec2 speed);
	// 移动
	void move(Vec2 pos);
	// 设置当前面向的方向
	void setDir(Dir dir);
	// 返回此精灵
	Sprite* getSpite();
	// 设置加速度
	void setAcceX(float x);
	// 设置空中速度控制
	void air(int step);
	// 返回速度
	Vec2 getSpeed();

private:
	AniState nowAni = AniState::FALL;
	int life;
	int energy;
	bool isGround = false;;
	Dir dir = Dir::STOP;
	Animate* animate = nullptr;
	Sprite* center = nullptr;
	PhysicsBody *triggerX;
	PhysicsBody *triggerY;

	// 当前速度
	Vec2 Speed = Vec2(0, 0);
};