///玩家操控的角色类

#pragma once
#include<string>
#include"cocos2d.h"

//速度即每秒改变的像素
//每个图块是32*32的
using namespace cocos2d;

static const int PLAYER_TAG = 110;
static int PLAYER_WIDTH = 30;
static int PLAYER_HEIGHT = 45;

// 角色最大X移动速度
static const float  MAX_RUN = 220;
// 角色最大Y移动速度
static const float  MAX_CLIMB = 110;
// 角色最大下滑速度
static const float	MAX_SLIP = 300;
// X世界最大速度
static const float  MAX_X = 800;
// Y世界最大速度
static const float  MAX_Y = 800;
// 跳跃起始速度
static const float  JUMPSPEED = 500;
// 最多抓住墙6s, 上爬3dt, 不动2dt, 下爬1dt, 落地刷新
static const float	TIMER_CLIMBING = 6;
// 反身跳操作锁定0.3s
static const float	TIMER_BACKJUMP = 0.3;
// 冲刺操作锁定0.3s
static const float	TIMER_DASH = 0.2;
// 边缘跳跃容错0.1s
static const float	TIMER_FALL = 0.1;
// 提前跳跃容错0.1s
static const float	TIMER_PREJUMP = 0.1;
// 反身跳跃容错0.1s
static const float	TIMER_OUT = 0.1;
// 最大冲刺次数
static const int	DASH_TIMES = 1;



// 序列帧动画路径数组
static std::string PLAYER_IMG_PATH[4] = {
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
	void init(cocos2d::Vec2 pos);
	// 设置将要播放的动画状态
	void setAnimation(AniState state);
	// 以速度向目的地移动
	bool moveTo(cocos2d::Vec2 pos, cocos2d::Vec2 speed);
	// 移动
	void toNewPos(cocos2d::Vec2 pos);
	// 返回此精灵
	cocos2d::Sprite* getSprite();
	// 时钟调用
	void update(float dt);
	// 返回位置
	Vec2 getPos();

	// 跳跃
	void jump();
	// 冲刺
	void dash(int dir);
	// 结束跳跃
	void jumpend();
	// 跑步
	void run(int dir);
	// 爬墙
	void climb(int dir);
	// 下落
	void fall(float speed);
	// 撞到头
	void headCol();
	// 滑墙
	void slip(int wallDir);
	// 抓墙
	void hold(bool isHolding);
	// 落地
	void ground(bool isGround);
	// 移动阻力
	void slow(float speed);
	// 风阻
	//void wind(float speed);
	// 返回当前速度
	cocos2d::Vec2 getSpeed();

private:
	AniState nowAni = AniState::FALL;
	int life;
	cocos2d::Animate* animate = nullptr;
	cocos2d::Sprite* center = nullptr;

	// 当前速度
	cocos2d::Vec2 Speed = cocos2d::Vec2(0, 0);
	// 是否在地面
	bool isGround = false;
	// 是否在跳跃
	bool isJumping = false;
	// 是否滑墙
	bool isSliping = false;
	// 是否抓墙
	bool isHolding = false;
	// 墙的方向
	int wallDir = 0;
	// 保存下反身跳的朝向
	int backDir = 0;
	// 可冲刺的次数
	int dashNum = 1;
	// 当前水平面向的方向
	bool isRight = true;

	// 与上帧的间隔
	float dt=0;
	// 离开地面计时器，用来做边缘跳跃容错
	float fallTimer = -1;
	// 离开墙面计时器，用来做反身跳容错
	float outTimer = -1;
	// 提早跳跃计时器，用来做落地跳跃容错
	float prejumpTimer = -1;
	// 反身跳计时器，在一小段时间内不能左右移动
	float backjumpTimer = -1;
	// 冲刺计时器，在一小段时间内不能左右移动
	float dashTimer = -1;
	// 攀爬计时器，用来作为能量
	float climbTimer = -1;

	// 设置X速度
	void setSpeedX(float x);
	// 叠加X速度
	void addSpeedX(float x);
	// 设置Y速度
	void setSpeedY(float y);
	// 叠加Y速度
	void addSpeedY(float y);
	// 计时器刷新
	void timer();
};