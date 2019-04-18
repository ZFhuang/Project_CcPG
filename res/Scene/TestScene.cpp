#include "TestScene.h"
#include "proj.win32\res\Character\Player.h"

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

	// �ر��������Լ���ģ��ȽϺ�
	//layer->pw->setGravity(Vec2(0.0f, 0.0f));

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
		this->addChild(map, 2);
		map->setTag(MAP_TAG);

		map->setPhysicsBody(PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3));
		map->setPosition(Vec2(0, 0));

		//��ש���������
		TMXLayer* platforms = map->getLayer(PLATFORM_LAYER);
		int w = map->getMapSize().width;
		int h = map->getMapSize().height;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = platforms->getTileAt(Vec2(x, y));//��tile������ȡ����Ӧ�ľ���
				if (!sprite)//��ֹspriteΪNULL
					continue;
				PhysicsBody* body = PhysicsBody::createEdgeBox(sprite->getContentSize());//����������һ������
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
	// ������
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

	// ������Ϸ������
	gameManager = GameManager::getInstance();

	// ���ص�ͼ
	loadMap(MAP_TMX_FILE_PATH);

	// ���س�����ɫ
	loadCharacter();

	// ���ؿ�����
	controller = MainController::getInstance(player, this, MAP_TAG);

	// ������Ϸ�߼��ص�
	this->scheduleUpdate();

	// ��ʼ��������
	controller->init();

	return true;
}

void TestScene::update(float dt)
{
	// �������ص�
	controller->update();

}
