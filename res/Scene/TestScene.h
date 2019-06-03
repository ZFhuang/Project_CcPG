///测试用的场景类

#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Player;
class GameManager;
class MainController;

//地图的xml文件位置
static const std::string MAP_TMX_FILE_PATH = "Map/testPlatform.tmx";

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
	// 物理世界指针
	cocos2d::PhysicsWorld* pw;
	Vec2 birthPlace;

	// 加载地图
	void loadMap(std::string mapPath);
	// 加载角色（不只是玩家）
	void loadCharacter();
};