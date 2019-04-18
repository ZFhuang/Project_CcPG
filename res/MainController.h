///负责玩家控制交互的类

#pragma once
#include "cocos2d.h"
#include "Character\Player.h"
USING_NS_CC;

static const string PLATFORM_LAYER = "Platform";
static const float RUNACCE = 2;

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
	// 碰撞监听
	void addCollideListener();
	// 按键触发响应
	void keyClick(EventKeyboard::KeyCode code);
	// 按键按住响应,放在循环中每隔一段时间检测一次比较好
	void keyPress();
	// 按键松开响应
	void keyRelease(EventKeyboard::KeyCode code);
	// 按键map
	std::map<EventKeyboard::KeyCode, bool> keymap;
	// 处理碰撞
	bool onContactBegin(const PhysicsContact &contact);

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
	// 左右移动的加速度
	int acceSpeed = RUNACCE;
};