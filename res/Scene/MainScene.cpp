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
		//�����ؿ�
		tileMap = TMXTiledMap::create(MAP_TEST);
		break;
	}
	return tileMap;
}

void MainScene::initMap()
{
	// ��ȡ��ͼ�Ӳ��������յ�
	TMXLayer* snare = map->getLayer(SNARE_LAYER);
	int w = map->getMapSize().width;
	int h = map->getMapSize().height;
	keyNum = 0;
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			Sprite* sprite = snare->getTileAt(Vec2(x, y));
			auto ID = snare->getTileGIDAt(Vec2(x, y));
			if (!sprite)	// ��ֹspriteΪNULL
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
	// ���ص�ͼ
	map = selectMap();
	CCLOG("MS: %d", nowSceneIdx);
	if (map != NULL)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		map->setName("MainScene");
		map->setPosition(Vec2(0, 0));
		this->addChild(map, 2);

		initMap();

		// ����TARGET_LAYER������(1*3,λ�û�����ƽ��)
		TMXLayer* target = map->getLayer(TARGET_LAYER);
		int w = map->getMapSize().width;
		int h = map->getMapSize().height;
		birthPlace = Vec2(0, 0);
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = target->getTileAt(Vec2(x, y));
				if (!sprite)	// ��ֹspriteΪNULL
					continue;
				if (target->getTileGIDAt(Vec2(x, y)) == BIRTH) {
					birthPlace.x += sprite->getPositionX();
					birthPlace.y += sprite->getPositionY();
					// �������
					sprite->setVisible(false);
				}
			}
		}

		// �����ƶ���������
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
	// ������
	player = new Player();
	player->init(Vec2(0, 0));
	this->addChild(player->getSprite(), 10);
	player->toNewPos(Vec2(-10, -10));
}

void MainScene::loadCamera()
{
	//�½�һ���û������
	camera = Camera::create();
	camera->setCameraFlag(CameraFlag::USER1);
	cameraRange = new Rect();

	//�ô˳����л�Ϊ�û������
	this->setCameraMask((short)CameraFlag::USER1);
	this->addChild(camera);

	//��ʼ���������
	auto sX = map->getContentSize().width;
	auto sY = map->getContentSize().height;
	Vec3 eyePosOld = camera->getPosition3D();
	Vec3 eyePos = Vec3(sX / 2, sY / 2, eyePosOld.z);
	camera->setPosition3D(eyePos);
	// ����Ӧ����
	float cam_scale = CAM_SCALE* (this->getContentSize().height / 1080);
	camera->setScaleX(camera->getScaleX() / cam_scale);
	camera->setScaleY(camera->getScaleY() / cam_scale);

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
}

void MainScene::loadUI()
{
	// ��ʼ���Ի���
	ui = new UI();
	ui->init(controller);
	if (nowStoryIdx == nowSceneIdx) {
		//�������
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

void MainScene::onEnterTransitionDidFinish()
{

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ������Ϸ������
	gameManager = GameManager::getInstance();

	// ���س�����ɫ
	loadCharacter();

	// ���ص�ͼ
	loadMap();

	// ����UI
	loadUI();

	// ���������
	loadCamera();

	// ���ؿ�����
	controller = MainController::getInstance(player, this,ui, map);

	// ������Ϸ�߼��ص�
	this->scheduleUpdate();

	// ��ʼ��������
	controller->init();

	// ����ʱ��ס0.1s����
	if (nowStoryIdx == nowSceneIdx)
		controller->sysTimer = 10;
	else
		controller->sysTimer = 0.1;

	return;
}

void MainScene::update(float dt)
{
	// �������ص�
	controller->update(dt);
	// �Ի����ص�,��Ҫ�������
	ui->update(dt,camera->getPosition());
	// �����������������
	cameraFollow();
}
