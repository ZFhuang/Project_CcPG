#include "MainScene.h"
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\MainController.h"
#include "proj.win32\res\Snare\Needle.h"
#include "proj.win32\res\Snare\Battery.h"
#include "proj.win32\res\Snare\Win.h"
#include "proj.win32\res\MainConfig.h"
#include "proj.win32\res\GameManager.h"

extern int nowSceneIdx;
extern bool sceneIdxLock;

cocos2d::Scene* MainScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

TMXTiledMap * MainScene::selectMap()
{
	TMXTiledMap* tileMap=nullptr;
	switch (nowSceneIdx)
	{
	case 0:
		tileMap = TMXTiledMap::create(MAP_TEST);
		break;
	case 1:
		tileMap = TMXTiledMap::create(MAP_01);
		break;
	case 2:
		tileMap = TMXTiledMap::create(MAP_02);
		break;
	default:
		//重启关卡
		tileMap = TMXTiledMap::create(MAP_TEST);
		break;
	}
	sceneIdxLock = true;
	return tileMap;
}

void MainScene::initMap()
{
	// 读取地图子部件包括终点
	TMXLayer* snare = map->getLayer(SNARE_LAYER);
	int w = map->getMapSize().width;
	int h = map->getMapSize().height;
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			Sprite* sprite = snare->getTileAt(Vec2(x, y));
			auto ID = snare->getTileGIDAt(Vec2(x, y));
			if (!sprite)	// 防止sprite为NULL
				continue;
			if (ID == NEEDLE) {
				auto needle = new Needle();
				needle->init(sprite, player, gameManager);
				this->addChild(needle);
			}
			else if (ID == BATTERY) {
				auto battery = new Battery();
				battery->init(sprite, player);
				this->addChild(battery);
			}
			else if (ID == WIN) {
				auto win = new Win();
				win->init(sprite, player, gameManager);
				this->addChild(win);
			}
		}
	}
}

void MainScene::loadMap()
{
	// 加载地图
	map = selectMap();
	if (map != NULL)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		map->setName("MainScene");
		map->setPosition(Vec2(0, 0));
		this->addChild(map, 2);

		initMap();

		// 载入TARGET_LAYER出生点(1*3,位置会是其平均)
		TMXLayer* target = map->getLayer(TARGET_LAYER);
		int w = map->getMapSize().width;
		int h = map->getMapSize().height;
		birthPlace = Vec2(0, 0);
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = target->getTileAt(Vec2(x, y));
				if (!sprite)	// 防止sprite为NULL
					continue;
				if (target->getTileGIDAt(Vec2(x, y)) == BIRTH) {
					birthPlace.x += sprite->getPositionX();
					birthPlace.y += sprite->getPositionY();
					// 隐藏起点
					sprite->setVisible(false);
				}
			}
		}

		// 人物移动到出生点
		birthPlace.x = birthPlace.x / 4 + player->getSprite()->getContentSize().width / 2;
		birthPlace.y = birthPlace.y / 4 + player->getSprite()->getContentSize().width / 2;
		player->toNewPos(birthPlace);
	}
	else
	{
		CCLOG("load map failed!");
		return;
	}
}

void MainScene::loadCharacter()
{
	// 添加玩家
	player = new Player();
	player->init(Vec2(0, 0));
	this->addChild(player->getSprite(), 10);
}

void MainScene::loadCamera()
{
	//新建一个用户摄像机
	camera = Camera::create();
	camera->setCameraFlag(CameraFlag::USER1);
	cameraRange = new Rect();
	//followPoint = Node::create();

	//让此场景切换为用户摄像机
	this->setCameraMask((short)CameraFlag::USER1);
	this->addChild(camera);
	//this->addChild(followPoint);

	//初始化相机参数
	auto sX = map->getContentSize().width;
	auto sY = map->getContentSize().height;
	Vec3 eyePosOld = camera->getPosition3D();
	Vec3 eyePos = Vec3(sX / 2, sY / 2, eyePosOld.z);
	camera->setPosition3D(eyePos);
	// 自适应缩放
	float cam_scale =CAM_SCALE* (this->getContentSize().height/1080);
	camera->setScaleX(camera->getScaleX() / cam_scale);
	camera->setScaleY(camera->getScaleY() / cam_scale);
	//followPoint->setPositionX(sX / 2);
	//followPoint->setPositionY(sY / 2);

	//计算相机的可移动空间
	auto rangeXMin = 0 + this->getContentSize().width / cam_scale / 2;
	auto rangeXMax = sX - this->getContentSize().width / cam_scale / 2;
	auto rangeYMin = 0 + this->getContentSize().height / cam_scale / 2;
	auto rangeYMax = sY - this->getContentSize().height / cam_scale / 2;

	if (rangeXMin >= rangeXMax) {
		rangeXMin = 0;
		rangeXMax = 0;
	}
	if (rangeYMin >= rangeYMax) {
		rangeYMin = 0;
		rangeYMax = 0;
	}

	//设置为可移动矩形
	cameraRange->setRect(rangeXMin, rangeYMin, rangeXMax - rangeXMin, rangeYMax - rangeYMin);
	//camera->runAction(Follow::create(followPoint));
}

void MainScene::cameraFollow()
{
	//在移动矩形中修改位置
	auto pos = player->getPos();
	if (cameraRange->size.width > 0) {
		if ((pos.x > cameraRange->getMinX()) &&
			(pos.x < cameraRange->getMaxX())) {
			camera->setPositionX(pos.x);
		}
		else if ((pos.x < cameraRange->getMinX())) {
			camera->setPositionX(cameraRange->getMinX());
		}
		else if ((pos.x > cameraRange->getMaxX())) {
			camera->setPositionX(cameraRange->getMaxX());
		}
	}
	if (cameraRange->size.height > 0) {
		if ((pos.y > cameraRange->getMinY()) &&
			(pos.y < cameraRange->getMaxY())) {
			camera->setPositionY(pos.y);
		}
		else if ((pos.y < cameraRange->getMinY())) {
			camera->setPositionY(cameraRange->getMinY());
		}
		else if ((pos.y > cameraRange->getMaxY())) {
			camera->setPositionY(cameraRange->getMaxY());
		}
	}
}

bool MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 加载游戏管理器
	gameManager = GameManager::getInstance();

	// 加载场景角色
	loadCharacter();

	// 加载地图
	loadMap();

	// 加载摄像机
	loadCamera();

	// 加载控制器
	controller = MainController::getInstance(player,this, map);

	// 设置游戏逻辑回调
	this->scheduleUpdate();

	// 初始化控制器
	controller->init();

	// 启动时锁住0.1s操作
	controller->sysTimer = 0.1;

	return true;
}

void MainScene::update(float dt)
{
	// 控制器回调
	controller->update(dt);
	// 摄像机跟随相机跟随点
	cameraFollow();
}
