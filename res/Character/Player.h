///玩家操控的角色类

#pragma once

#include"proj.win32\res\GameManager.h"

using namespace std;

//速度即每秒改变的像素
//每个图块是32*32的

static const int	PLAYER_TAG = 110;
static int  PLAYER_WIDTH = 40;
static int  PLAYER_HEIGHT = 50;
static const float	JUMP_ACCE = 1.2;
static const float	FALL_ACCE = 1.2;
static const float	SLIP_ACCE = 0.5;
static const float  MAX_PLAYER_SPEED_X = 200;
static const float	MAX_SPEED_SLIP = 50;
static const float	MAX_ENERGY = 3000;	// 最多抓住墙3s

// 地面停止的惯性
static const float  SLOW_DOWN_X = 400;
// 空中停止的惯性
static const float  SLOW_DOWN_AIR = 200;

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
	// 返回此精灵
	Sprite* getSpite();
	// 设置跑步等本身的加速度
	void setAcceX(float x, bool isGround);
	// 设置上下爬的加速度
	void setAcceY(float y);
	// 反身跳
	void sysBackjump(float x);
	// 设置Y速度
	void setSpeedY(float y);
	// 叠加Y速度
	void addSpeedY(float y);
	// 返回当前速度
	Vec2 getSpeed();
	// 计算体力
	bool useEnergy(float used);
	// 当前水平面向的方向
	bool isRight = false;

private:
	AniState nowAni = AniState::FALL;
	int life;
	clock_t energy = 0;
	Animate* animate = nullptr;
	Sprite* center = nullptr;
	PhysicsBody *triggerX;
	PhysicsBody *triggerY;

	// 当前速度
	Vec2 Speed = Vec2(0, 0);
};