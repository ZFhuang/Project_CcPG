///负责玩家控制交互的类

#pragma once
#include "cocos2d.h"
#include "Character\Player.h"
USING_NS_CC;

//速度即每秒改变的像素
//每个图块是32*32的

static const string PLATFORM_LAYER = "Platform";
// 纵向加速度
static const float SCENE_Y = -20;
// 横向加速度
static const float SCENE_X = 0;
// 纵向最大速度
static const float MAX_Y = 450;
// 左右移动的加速度
static const float RUNSPEED = 200;
// 上下爬的加速度
static const float CLIMBACCE = 2;
// 使用能量的速度
static const float	ENERGYACCE = 1000/1;
// 跳跃总时间ms
static const clock_t JUMPTIME = 200;
// 跳跃起始速度
static const float JUMPSPEED = 50;
// 最大下滑速度
static const float SLIPSPEED = -300;
// 反身跳总时间ms
static const clock_t BACKJUMPTIME = 200;
// 下落容错时间ms
static const clock_t FAULT_FALLTIME = 40;
// 提早跳跃容错时间ms
static const clock_t FAULT_JUMPTIME = 40;
// 跳跃的上升段所占的比例
static const float UPRATE = 0.8;
struct PlayerCol {
	Vec2 newPos;
	int xCol = 0;
	int yCol = 0;
};

class MainController
{
public:
	MainController();
	virtual ~MainController();
	// 工厂函数，需要所控制的物体及所控制的层传入
	static MainController* getInstance(Player* in, cocos2d::Layer* layer, int maptag);
	// 初始化
	void init();
	// 控制器内部的回调
	void update(float dt);
	// 按键监听
	void addKeyListener();
	// 按键触发响应
	void keyClick(EventKeyboard::KeyCode code);
	// 按键按住响应,放在循环中每隔一段时间检测一次比较好
	void keyPress();
	// 锁住按键时的系统阶段
	void sysCtrl();
	// 环境赋予的被动加速度
	void environment();
	// 按键松开响应
	void keyRelease(EventKeyboard::KeyCode code);
	// 状态处理
	void condition();
	// 按键map
	std::map<EventKeyboard::KeyCode, bool> keymap;

private:
	// 所控制的物体指针
	Player* player;
	// 所控制的层指针
	cocos2d::Layer* layer;
	// 碰撞要用到的tiledmap
	TMXTiledMap* map;
	// 本体指针
	static MainController* controller;
	// 用来实现左右移动按键互相覆盖的变量
	int clickDirX = 0;
	// 用来实现上下移动按键互相覆盖的变量
	int clickDirY = 0;
	// 返回按照速度可以移动到的位置
	PlayerCol getNewPos(Vec2 speed);
	// 是否在地面
	bool isGround = false;
	// 锁住按住反馈，进入系统移动操作
	bool sysMove = false;
	// 墙壁方向,0代表不贴墙
	int wallDir = 0;
	// 是否抓墙
	bool isHold = false;
	// 是否开启环境Y
	bool openY = true;
	// 是否开启环境X
	bool openX = true;
	// 计算按下跳跃多久的计时
	clock_t jumpStart = 0;
	// 计算按下反身跳多久的计时
	clock_t backjumpStart = 0;
	// 计算离开地面多久的计时，用来做跳跃容错
	clock_t fallStart = 0;
	// 计算离开墙面多久的计时，用来做反身跳容错
	clock_t outStart = 0;
	// 提早跳跃计时器
	clock_t prejumpStart = 0;
	// 当前帧间隔
	float dt;
};