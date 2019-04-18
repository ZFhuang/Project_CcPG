#include "TestScene.h"
#include "proj.win32\res\Character\Player.h"

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

	// 关闭重力，自己来模拟比较好
	//layer->pw->setGravity(Vec2(0.0f, 0.0f));

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
		this->addChild(map, 2);
		map->setTag(MAP_TAG);

		map->setPhysicsBody(PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3));
		map->setPosition(Vec2(0, 0));

		//给砖块加上物理
		TMXLayer* platforms = map->getLayer(PLATFORM_LAYER);
		int w = map->getMapSize().width;
		int h = map->getMapSize().height;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = platforms->getTileAt(Vec2(x, y));//从tile的坐标取出对应的精灵
				if (!sprite)//防止sprite为NULL
					continue;
				PhysicsBody* body = PhysicsBody::createEdgeBox(sprite->getContentSize());//给精灵设置一个刚体
				//body->setDynamic(false);
				body->setTag(3);
				//body->setCategoryBitmask(0x00000030);
				//body->setContactTestBitmask(0x00000003);
				body->setGravityEnable(false);
				sprite->setPhysicsBody(body);
			}
		}
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
	player->init(Vec2(150, 250));
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

	// 加载地图
	loadMap(MAP_TMX_FILE_PATH);

	// 加载场景角色
	loadCharacter();

	// 加载控制器
	controller = MainController::getInstance(player, this, MAP_TAG);

	// 设置游戏逻辑回调
	this->scheduleUpdate();

	// 初始化控制器
	controller->init();

	return true;
}

void TestScene::update(float dt)
{
	// 控制器回调
	controller->update();

}
