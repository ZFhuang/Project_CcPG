#include "MainScene.h"
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\MainController.h"
#include "proj.win32\res\Snare\Needle.h"
#include "proj.win32\res\Snare\Battery.h"
#include "proj.win32\res\Snare\Win.h"
#include "proj.win32\res\Snare\Key.h"
#include "proj.win32\res\MainConfig.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\UI.h"

extern int nowSceneIdx;
extern int nowStoryIdx;

cocos2d::Scene* MainScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainScene::create();

	layer->setTag(10);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

TMXTiledMap * MainScene::selectMap()
{
	TMXTiledMap* tileMap = nullptr;
	switch (nowSceneIdx)
	{
	case 0:
		tileMap = TMXTiledMap::create(MAP_00);
		break;
	case 1:
		tileMap = TMXTiledMap::create(MAP_01);
		break;
	case 2:
		tileMap = TMXTiledMap::create(MAP_02);
		break;
	case 3:
		tileMap = TMXTiledMap::create(MAP_03);
		break;
	case 4:
		tileMap = TMXTiledMap::create(MAP_04);
		break;
	case 5:
		tileMap = TMXTiledMap::create(MAP_05);
		break;
	case 6:
		tileMap = TMXTiledMap::create(MAP_06);
		break;
	case 7:
		tileMap = TMXTiledMap::create(MAP_07);
		break;
	case 8:
		tileMap = TMXTiledMap::create(MAP_08);
		break;
	case 9:
		tileMap = TMXTiledMap::create(MAP_09);
		break;
	case 10:
		tileMap = TMXTiledMap::create(MAP_10);
		break;
	default:
		//重启关卡
		tileMap = TMXTiledMap::create(MAP_TEST);
		break;
	}
	return tileMap;
}

void MainScene::initMap()
{
	// 读取地图子部件包括终点
	TMXLayer* snare = map->getLayer(SNARE_LAYER);
	int w = map->getMapSize().width;
	int h = map->getMapSize().height;
	keyNum = 0;
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			Sprite* sprite = snare->getTileAt(Vec2(x, y));
			auto ID = snare->getTileGIDAt(Vec2(x, y));
			if (!sprite)	// 防止sprite为NULL
				continue;
			if (ID == NEEDLE_UP) {
				auto needle = new Needle();
				needle->init(sprite, player, gameManager, 1);
				this->addChild(needle);
			}
			else if (ID == NEEDLE_DOWN) {
				auto needle = new Needle();
				needle->init(sprite, player, gameManager, 2);
				this->addChild(needle);
			}
			else if (ID == NEEDLE_LEFT) {
				auto needle = new Needle();
				needle->init(sprite, player, gameManager, 3);
				this->addChild(needle);
			}
			else if (ID == NEEDLE_RIGHT) {
				auto needle = new Needle();
				needle->init(sprite, player, gameManager, 4);
				this->addChild(needle);
			}
			else if (ID == BATTERY) {
				auto battery = new Battery();
				battery->init(sprite, player);
				this->addChild(battery);
			}
			else if (ID == WIN) {
				auto win = new Win();
				win->init(sprite, player, gameManager, nowSceneIdx);
				this->addChild(win);
			}
			else if (ID == KEY) {
				auto key = new Key();
				key->init(sprite, player);
				this->addChild(key);
				keyNum++;
			}
		}
	}
	if (keyNum == 1) {
		keyNum = 2;
	}
	player->initKeyNum(keyNum);
}

void MainScene::loadMap()
{
	// 加载地图
	map = selectMap();
	CCLOG("MS: %d", nowSceneIdx);
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
	player->toNewPos(Vec2(-10, -10));
}

void MainScene::loadCamera()
{
	//新建一个用户摄像机
	camera = Camera::create();
	camera->setCameraFlag(CameraFlag::USER1);
	cameraRange = new Rect();

	//让此场景切换为用户摄像机
	this->setCameraMask((short)CameraFlag::USER1);
	this->addChild(camera);

	//初始化相机参数
	auto sX = map->getContentSize().width;
	auto sY = map->getContentSize().height;
	Vec3 eyePosOld = camera->getPosition3D();
	Vec3 eyePos = Vec3(sX / 2, sY / 2, eyePosOld.z);
	camera->setPosition3D(eyePos);
	// 自适应缩放
	float cam_scale = CAM_SCALE* (this->getContentSize().height / 1080);
	camera->setScaleX(camera->getScaleX() / cam_scale);
	camera->setScaleY(camera->getScaleY() / cam_scale);

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
}

void MainScene::loadUI()
{
	// 初始化对话器
	ui = new UI();
	ui->init(controller);
	if (nowStoryIdx == nowSceneIdx) {
		//输出剧情
		switch (nowStoryIdx)
		{
		case 0:
			ui->loadTextFile(STORY_0);
			nowStoryIdx++;
			break;
		case 1:
			ui->loadTextFile(STORY_1);
			nowStoryIdx++;
			break;
		case 2:
			ui->loadTextFile(STORY_2);
			nowStoryIdx++;
			break;
		case 3:
			ui->loadTextFile(STORY_3);
			nowStoryIdx++;
			break;
		case 4:
			ui->loadTextFile(STORY_4);
			nowStoryIdx++;
			break;
		case 5:
			ui->loadTextFile(STORY_5);
			nowStoryIdx++;
			break;
		case 6:
			ui->loadTextFile(STORY_6);
			nowStoryIdx++;
			break;
		case 7:
			ui->loadTextFile(STORY_7);
			nowStoryIdx++;
			break;
		case 8:
			ui->loadTextFile(STORY_8);
			nowStoryIdx++;
			break;
		case 9:
			ui->loadTextFile(STORY_9);
			nowStoryIdx++;
			break;
		case 10:
			ui->loadTextFile(STORY_10);
			nowStoryIdx++;
			break;
		default:
			break;
		}
	}
	else {
		ui->loadTextFile(STORY_DEAD);
	}
	this->addChild(ui->talker, 10);
	this->addChild(ui->text, 10);
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

void MainScene::onEnterTransitionDidFinish()
{

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 加载游戏管理器
	gameManager = GameManager::getInstance();

	// 加载场景角色
	loadCharacter();

	// 加载地图
	loadMap();

	// 加载UI
	loadUI();

	// 加载摄像机
	loadCamera();

	// 加载控制器
	controller = MainController::getInstance(player, this,ui, map);

	// 设置游戏逻辑回调
	this->scheduleUpdate();

	// 初始化控制器
	controller->init();

	// 启动时锁住0.1s操作
	if (nowStoryIdx == nowSceneIdx)
		controller->sysTimer = 10;
	else
		controller->sysTimer = 0.1;

	return;
}

void MainScene::update(float dt)
{
	// 控制器回调
	controller->update(dt);
	// 对话器回调,需要与相机绑定
	ui->update(dt,camera->getPosition());
	// 摄像机跟随相机跟随点
	cameraFollow();
}
