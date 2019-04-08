#include "TestScene.h"
#include "proj.win32\res\Character\Player.h"

cocos2d::Scene* TestScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = TestScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void TestScene::loadMap(std::string mapPath)
{
	// ���ص�ͼ
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
	// ������
	player = new Player();
	player->init(Vec2(250, 250));
	this->addChild(player->getPlayer(), 10);
}

bool TestScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ������Ϸ������
	gameManager = GameManager::getInstance();

	// ���ص�ͼ
	loadMap(MAP_TMX_FILE_PATH);

	// ���س�����ɫ
	loadCharacter();

	// ���ؿ�����
	controller = MainController::getInstance(player,this);

	// ������Ϸ�߼��ص�
	this->scheduleUpdate();

	// ������������
	controller->addKeyListener();

	return true;
}

void TestScene::update(float dt)
{
	// ���������м���Ƿ��м������Ҵ���
	controller->keyPress();

}
