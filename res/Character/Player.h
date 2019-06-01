///玩家操控的角色类

#pragma once

#include"proj.win32\res\GameManager.h"

using namespace std;

//速度即每秒改变的像素
//每个图块是32*32的

static const int PLAYER_TAG = 110;
static int PLAYER_WIDTH = 40;
static int PLAYER_HEIGHT = 50;

// 角色最大X移动速度
static const float  MAX_RUN = 200;
// 角色最大下滑速度
static const float	MAX_SLIP = 70;
// X世界最大速度
static const float  MAX_X = 1000;
// Y世界最大速度
static const float  MAX_Y = 1000;
// Y世界最小速度
static const float  MIN_Y = -1000;
// 最多抓住墙3s
static const float	TIMER_HOLDING = 3;
// 最多跳跃1s
static const float	TIMER_JUMPING = 1;



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
	void toNewPos(Vec2 pos);
	// 返回此精灵
	Sprite* getSpite();
	// 时钟调用
	void update(float dt);

	// 设置跑步等本身的加速度
	//void setAcceX(float x, bool isGround);
	// 设置上下爬的加速度
	//void setAcceY(float y);
	// 反身跳
	//void sysBackjump(float x);
	// 跳跃
	void jump();
	// 跑步
	void run(int dir);
	// 下落
	void fall(float speed);
	// 落地
	void ground();
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
	//PhysicsBody *triggerX;
	//PhysicsBody *triggerY;
	// 当前速度
	Vec2 Speed = Vec2(0, 0);

	// 是否在地面
	bool isGround = false;
	// 是否在跳跃
	bool isJumping = false;
	// 是否滑墙
	bool isSliping = false;

	// 上帧的间隔
	float dt=0;
	// 计算按下跳跃多久的计时
	float jumpStart = 0;
	// 计算按下反身跳多久的计时
	float backjumpStart = 0;
	// 计算离开地面多久的计时，用来做边缘跳跃容错
	float fallStart = 0;
	// 计算离开墙面多久的计时，用来做反身跳容错
	float outStart = 0;
	// 提早跳跃计时器，用来做落地跳跃容错
	float prejumpStart = 0;

	// 设置X速度
	void setSpeedX(float x);
	// 叠加X速度
	void addSpeedX(float x);
	// 设置Y速度
	void setSpeedY(float y);
	// 叠加Y速度
	void addSpeedY(float y);
};