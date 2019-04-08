///测试用的场景类

#pragma once

#include "cocos2d.h"
#include "proj.win32\res\MainConfig.h"
#include "proj.win32\res\Character/Player.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\MainController.h"
USING_NS_CC;

//地图tag,通过tag来区分读入的不同地图
#define MAP_TAG 111
//地图的xml文件位置
#define MAP_TMX_FILE_PATH "Map/testPlatform.tmx"


class TestScene : public cocos2d::Layer
{
public:
	// 初始化
	virtual bool init();
	// 主循环
	void update(float dt);
	// 创建场景
	static cocos2d::Scene* createScene();
	// implement the "static create()" method manually
	CREATE_FUNC(TestScene);

private:
	// 总控对象
	GameManager* gameManager;
	// 主控制器
	MainController* controller;
	// 玩家对象
	Player* player;

	// 加载地图
	void loadMap(std::string mapPath);
	// 加载角色（不只是玩家）
	void loadCharacter();
};