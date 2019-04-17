#include "TestScene.h"
#include "proj.win32\res\Character\Player.h"

cocos2d::Scene* TestScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = TestScene::create();

	//// 获得场景的PhysicsWorld添加到layer
	//layer->pw= scene->getPhysicsWorld();

	// 开启所有shape的debug绘制
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

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
		this->addChild(map, 2);
		map->setTag(MAP_TAG);
		map->setPosition(Vec2(0, 0));
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
	player->init(Vec2(250, 250));
	this->addChild(player->getSpite(), 10);
}

bool TestScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 加载游戏管理器
	gameManager = GameManager::getInstance();

	// 加载地图
	loadMap(MAP_TMX_FILE_PATH);

	// 加载场景角色
	loadCharacter();

	// 加载控制器
	controller = MainController::getInstance(player, this, MAP_TAG);

	//pw->setGravity(Vec2(0.0f, 1.0f));

	// 设置游戏逻辑回调
	this->scheduleUpdate();

	// 启动按键侦听
	controller->addKeyListener();

	return true;
}

void TestScene::update(float dt)
{
	// 在主调用中检查是否有键按下且处理
	controller->keyPress();

}
