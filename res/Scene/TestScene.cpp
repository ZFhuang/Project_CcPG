#include "TestScene.h"
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\MainController.h"
#include "proj.win32\res\Snare\Needle.h"
#include "proj.win32\res\MainConfig.h"
#include "proj.win32\res\GameManager.h"


cocos2d::Scene* TestScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

	// 'layer' is an autorelease object
	auto layer = TestScene::create();

	// 开启所有shape的debug绘制
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	// 获得场景的PhysicsWorld添加到layer
	layer->pw = scene->getPhysicsWorld();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void TestScene::loadMap(std::string mapPath)
{
	// 加载地图
	auto map = TMXTiledMap::create(mapPath);
	if (map != NULL)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		map->setTag(TEST_SCENE);
		map->setName("TestScene");
		this->addChild(map, 2);

		map->setPhysicsBody(PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3));
		map->setPosition(Vec2(0, 0));

		// 初始化机关
		TMXLayer* snare = map->getLayer(SNARE_LAYER);
		int w = map->getMapSize().width;
		int h = map->getMapSize().height;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = snare->getTileAt(Vec2(x, y));
				if (!sprite)	// 防止sprite为NULL
					continue;
				if (snare->getTileGIDAt(Vec2(x, y)) == NEEDLE) {
					auto needle = new Needle();
					needle->init(sprite, player, gameManager);
					this->addChild(needle);
				}
			}
		}

		// 载入出生点(必须是4个方块组合成的大方块，位置会是其平均)和终点
		TMXLayer* target = map->getLayer(TARGET_LAYER);
		birthPlace = Vec2(0, 0);
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = target->getTileAt(Vec2(x, y));
				if (!sprite)	// 防止sprite为NULL
					continue;
				if (target->getTileGIDAt(Vec2(x, y)) == BIRTH) {
					birthPlace.x += sprite->getPositionX();
					birthPlace.y += sprite->getPositionY();
				}
			}
		}
		birthPlace.x = birthPlace.x / 4 + player->getSpite()->getContentSize().width / 2;
		birthPlace.y = birthPlace.y / 4 + player->getSpite()->getContentSize().width / 2;

		player->toNewPos(birthPlace);

		gameManager->nowScene(TEST_SCENE);
	}
	else
	{
		CCLOG("load map failed!");
		return;
	}
}

void TestScene::loadCharacter()
{
	// 添加玩家
	player = new Player();
	player->init(Vec2(0, 0));
	this->addChild(player->getSpite(), 10);
}

bool TestScene::init()
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
	loadMap(MAP_TMX_FILE_PATH);

	// 加载控制器
	controller = MainController::getInstance(player, this, TEST_SCENE);

	// 设置游戏逻辑回调
	this->scheduleUpdate();

	// 初始化控制器
	controller->init();

	// 启动时锁住1s操作
	controller->sysTimer = 0.5;

	return true;
}

void TestScene::update(float dt)
{
	// 控制器回调
	controller->update(dt);
}
