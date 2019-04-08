///负责玩家控制交互的类

#pragma once
#include "cocos2d.h"
#include "Character\Player.h"
USING_NS_CC;

class MainController
{
public:
	MainController();
	virtual ~MainController();
	// 工厂函数，需要所控制的物体及所控制的层传入
	static MainController* getInstance(Player* in,cocos2d::Layer* layer);

	// 加载按键监听
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
	// 本身指针
	static MainController* controller;
};