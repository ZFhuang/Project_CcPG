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

	// ��������shape��debug����
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	// ��ó�����PhysicsWorld��ӵ�layer
	layer->pw = scene->getPhysicsWorld();

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
		Size visibleSize = Director::getInstance()->getVisibleSize();
		map->setTag(TEST_SCENE);
		map->setName("TestScene");
		this->addChild(map, 2);

		map->setPhysicsBody(PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3));
		map->setPosition(Vec2(0, 0));

		// ��ʼ������
		TMXLayer* snare = map->getLayer(SNARE_LAYER);
		int w = map->getMapSize().width;
		int h = map->getMapSize().height;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = snare->getTileAt(Vec2(x, y));
				if (!sprite)	// ��ֹspriteΪNULL
					continue;
				if (snare->getTileGIDAt(Vec2(x, y)) == NEEDLE) {
					auto needle = new Needle();
					needle->init(sprite, player, gameManager);
					this->addChild(needle);
				}
			}
		}

		// ���������(������4��������ϳɵĴ󷽿飬λ�û�����ƽ��)���յ�
		TMXLayer* target = map->getLayer(TARGET_LAYER);
		birthPlace = Vec2(0, 0);
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = target->getTileAt(Vec2(x, y));
				if (!sprite)	// ��ֹspriteΪNULL
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
	// ������
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

	// ������Ϸ������
	gameManager = GameManager::getInstance();

	// ���س�����ɫ
	loadCharacter();

	// ���ص�ͼ
	loadMap(MAP_TMX_FILE_PATH);

	// ���ؿ�����
	controller = MainController::getInstance(player, this, TEST_SCENE);

	// ������Ϸ�߼��ص�
	this->scheduleUpdate();

	// ��ʼ��������
	controller->init();

	// ����ʱ��ס1s����
	controller->sysTimer = 0.5;

	return true;
}

void TestScene::update(float dt)
{
	// �������ص�
	controller->update(dt);
}
