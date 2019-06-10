///测试用的场景类

#pragma once

#include "cocos2d.h"
#include<string>
USING_NS_CC;

class Player;
class GameManager;
class MainController;

// 1080P时的相机放大倍率，其它分辨率按照比例调整,必须是16：9
static const float CAM_SCALE = 2;

class MainScene : public cocos2d::Layer
{
public:
	// 初始化
	virtual bool init();
	// 主循环
	void update(float dt);
	// 创建场景
	static cocos2d::Scene* createScene();
	// implement the "static create()" method manually
	CREATE_FUNC(MainScene);

private:
	// 总控对象
	GameManager* gameManager;
	// 主控制器
	MainController* controller;
	// 摄像机
	Camera* camera;
	// 相机移动范围
	Rect* cameraRange;
	// 玩家对象
	Player* player;
	// 出生点
	Vec2 birthPlace;
	// 当前地图
	TMXTiledMap *map;

	// 选择地图
	TMXTiledMap* selectMap();
	// 初始化地图部件
	void initMap();
	// 读取地图
	void loadMap();
	// 加载角色（不只是玩家）
	void loadCharacter();
	// 加载摄像机
	void loadCamera();
	// 摄像机在地图范围内跟随角色
	void cameraFollow();
};