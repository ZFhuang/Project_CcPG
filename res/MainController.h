///负责玩家控制交互的类

#pragma once
#include "cocos2d.h"
USING_NS_CC;

class Player;

// 摩擦力
static const float SCENE_X = -1000;

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
	// 控制器内部的回调
	void update(float dt);
	// 初始化
	void init();
	// 系统锁定计时器,用于倒计时
	float sysTimer = 0;

private:
	// 所控制的物体指针
	Player* player;
	// 所控制的层指针
	cocos2d::Layer* layer;
	// 碰撞要用到的tiledmap
	TMXTiledMap* map;
	// 本体指针
	static MainController* controller;
	// 按键map
	std::map<EventKeyboard::KeyCode, bool> keymap;
	// 当前帧间隔
	float dt;

	// 用来实现左右移动按键互相覆盖的变量
	int clickDirX = 0;
	// 用来实现上下移动按键互相覆盖的变量
	int clickDirY = 0;
	// 返回按照速度可以移动到的位置
	PlayerCol getNewPos(Vec2 speed);
	// 锁住按键，进入系统操作
	bool isSysMode = false;

	// 是否开启环境Y速度
	bool openY = true;
	// 是否开启环境X速度
	bool openX = true;
	// 重力
	float SCENE_Y = -1000;

	// 按键监听
	void addKeyListener();
	// 按键触发响应
	void keyClick(EventKeyboard::KeyCode code);
	// 按键按住响应,放在循环中每隔一段时间检测一次比较好
	void keyPress();
	// 锁住按键时的系统阶段
	void sysMode();
	// 环境赋予的被动加速度
	void environment();
	// 按键松开响应
	void keyRelease(EventKeyboard::KeyCode code);
	// 状态处理
	void condition();
	// 计时器刷新
	void timer();
};