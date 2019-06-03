#include "TestScene.h"
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\MainController.h"
#include "proj.win32\res\Snare\Needle.h"
#include "proj.win32\res\MainConfig.h"
#include "proj.win32\res\GameManager.h"


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
	map = TMXTiledMap::create(mapPath);
	if (map != NULL)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		map->setTag(TEST_SCENE);
		map->setName("TestScene");
		map->setPosition(Vec2(0, 0));
		this->addChild(map, 2);

		// ��ʼ��SNARE_LAYER
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

		// ����TARGET_LAYER������(������4��������ϳɵĴ󷽿飬λ�û�����ƽ��)���յ�
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

		// �����ƶ���������
		birthPlace.x = birthPlace.x / 4 + player->getSprite()->getContentSize().width / 2;
		birthPlace.y = birthPlace.y / 4 + player->getSprite()->getContentSize().width / 2;
		player->toNewPos(birthPlace);

		// ��¼��ǰ��ͼ���
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
	this->addChild(player->getSprite(), 10);
}

void TestScene::loadCamera()
{
	//�½�һ���û������
	camera = Camera::create();
	camera->setCameraFlag(CameraFlag::USER1);
	cameraRange = new Rect();
	//followPoint = Node::create();

	//�ô˳����л�Ϊ�û������
	this->setCameraMask((short)CameraFlag::USER1);
	this->addChild(camera);
	//this->addChild(followPoint);

	//��ʼ���������
	auto sX = map->getContentSize().width;
	auto sY = map->getContentSize().height;
	Vec3 eyePosOld = camera->getPosition3D();
	Vec3 eyePos = Vec3(sX / 2, sY / 2, eyePosOld.z);
	camera->setPosition3D(eyePos);
	// ����Ӧ����
	float cam_scale =CAM_SCALE* (this->getContentSize().height/1080);
	camera->setScaleX(camera->getScaleX() / cam_scale);
	camera->setScaleY(camera->getScaleY() / cam_scale);
	//followPoint->setPositionX(sX / 2);
	//followPoint->setPositionY(sY / 2);

	//��������Ŀ��ƶ��ռ�
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

	//����Ϊ���ƶ�����
	cameraRange->setRect(rangeXMin, rangeYMin, rangeXMax - rangeXMin, rangeYMax - rangeYMin);
	//camera->runAction(Follow::create(followPoint));
}

void TestScene::cameraFollow()
{
	////���ƶ��������޸�λ��
	//auto pos = player->getPos();
	//if (cameraRange->size.width > 0) {
	//	if ((pos.x > cameraRange->getMinX()) &&
	//		(pos.x < cameraRange->getMaxX())) {
	//		followPoint->setPositionX(pos.x);
	//	}
	//	else if ((pos.x < cameraRange->getMinX())) {
	//		followPoint->setPositionX(cameraRange->getMinX());
	//	}
	//	else if ((pos.x > cameraRange->getMaxX())) {
	//		followPoint->setPositionX(cameraRange->getMaxX());
	//	}
	//}
	//if (cameraRange->size.height > 0) {
	//	if ((pos.y > cameraRange->getMinY()) &&
	//		(pos.y < cameraRange->getMaxY())) {
	//		followPoint->setPositionY(pos.y);
	//	}
	//	else if ((pos.y < cameraRange->getMinY())) {
	//		followPoint->setPositionY(cameraRange->getMinY());
	//	}
	//	else if ((pos.y > cameraRange->getMaxY())) {
	//		followPoint->setPositionY(cameraRange->getMaxY());
	//	}
	//}
	//followPoint->setPositionX(this->getContentSize().width / 2 - followPoint->getPositionX());
	//followPoint->setPositionY(this->getContentSize().height/2 - followPoint->getPositionY());
	//CCLOG("fo:%f %f", followPoint->getPositionX(), followPoint->getPositionY());
	//CCLOG("pl:%f %f", player->getSprite()->getPositionX(), player->getSprite()->getPositionY());
	//CCLOG("ca:%f %f", camera->getPositionX(), camera->getPositionY());

	//���ƶ��������޸�λ��
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

	// ���������
	loadCamera();

	// ���ؿ�����
	controller = MainController::getInstance(player, this, TEST_SCENE);

	// ������Ϸ�߼��ص�
	this->scheduleUpdate();

	// ��ʼ��������
	controller->init();

	// ����ʱ��ס0.1s����
	controller->sysTimer = 0.1;

	return true;
}

void TestScene::update(float dt)
{
	// �������ص�
	controller->update(dt);
	// �����������������
	cameraFollow();
}
