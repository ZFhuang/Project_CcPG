///负责玩家控制交互的类

#pragma once
#include "cocos2d.h"
#include "Character\Player.h"
USING_NS_CC;

static const string PLATFORM_LAYER = "Platform";
// 左右移动的加速度
static const float RUNACCE = 4;
// 跳跃总时间ms
static const clock_t JUMPTIME = 400;
// 下落容错时间ms
static const clock_t FAULT_FALLTIME = 40;
// 提早跳跃容错时间ms
static const clock_t FAULT_JUMPTIME = 40;
// 跳跃的上升段所占的比例
static const float UPRATE = 0.7;
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
	static MainController* getInstance(Player* in,cocos2d::Layer* layer,int maptag);
	// 初始化
	void init();
	// 控制器内部的回调
	void update();
	// 按键监听
	void addKeyListener();
	// 按键触发响应
	void keyClick(EventKeyboard::KeyCode code);
	// 按键按住响应,放在循环中每隔一段时间检测一次比较好
	void keyPress();
	// 按键松开响应
	void keyRelease(EventKeyboard::KeyCode code);
	// 按键map
	std::map<EventKeyboard::KeyCode, bool> keymap;

private:
	// 所控制的物体指针
	Player* player;
	// 所控制的层指针
	cocos2d::Layer* layer;
	// 碰撞要用到的tiledmap
	TMXTiledMap* map;
	// 本身指针
	static MainController* controller;
	//用来实现左右移动按键互相覆盖的变量
	int clickDir = 0;
	// 返回按照速度可以移动到的位置
	PlayerCol getNewPos(Vec2 speed);
	// 是否在地面
	bool isGround = false;
	// 锁住按住，用来做一些固定移动操作
	bool lockPress = false;
	// 计算按下跳跃多久的计时
	clock_t jumpStart = 0;
	// 计算离开地面多久的计时，用来做跳跃容错
	clock_t fallStart = 0;
	// 提早跳跃计时器
	clock_t prejumpStart = 0;
};