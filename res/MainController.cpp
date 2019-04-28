#include "MainController.h"
#include "GameManager.h"
#include "Character/Player.h"
#include <ctime>

MainController::MainController()
{
}

MainController::~MainController()
{
	controller = NULL;
}

MainController* MainController::controller = NULL;
MainController* MainController::getInstance(Player* in, cocos2d::Layer* layer, int maptag)
{
	//���˿������볡���;����
	if (controller == NULL)
	{
		controller = new MainController();
	}
	if (in != nullptr&&layer != nullptr) {
		controller->player = in;
		controller->layer = layer;
		controller->map = (TMXTiledMap *)layer->getChildByTag(maptag);
		return controller;
	}
	return NULL;
}

void MainController::init()
{
	addKeyListener();
}

void MainController::update()
{
	// ��������
	player->air(5);
	// ��������
	keyPress();
	auto Condition = getNewPos(player->getSpeed());
	if (isGround == true && jumpStart == 0 && clock() - prejumpStart <= FAULT_JUMPTIME) {
		isGround = false;
		jumpStart = clock();
	}
	if (clock() - prejumpStart > FAULT_JUMPTIME) {
		prejumpStart = 0;
	}
	if (Condition.yCol == 1) {
		// ��ײ��ͷʱ��ǿ�ƿ�ʼ����
		player->air(5);
		jumpStart = 0;
	}
	if (Condition.yCol == -1) {
		// ���
		player->air(7);
		isGround = true;
		fallStart = 0;
	}
	if (Condition.yCol == 0) {
		// �뿪����
		if (fallStart == 0)
			fallStart = clock();
	}
	if (Condition.xCol == 1 || Condition.xCol == -1) {
		// ǽ�
		isWall = true;
		if (isGround == false && isHold == false) {
			// û��ץסʱ����ǽ
			jumpStart = 0;
			player->air(6);
		}
	}
	else if (Condition.xCol == 0) {
		// �뿪ǽ��
		isWall = false;
	}
	if (fallStart != 0 && clock() - fallStart > FAULT_FALLTIME) {
		isGround = false;
	}
	//CCLOG("%lf %lf", Condition.newPos.x, Condition.newPos.y);
	Condition = getNewPos(player->getSpeed());
	player->move(Condition.newPos);
}

void MainController::addKeyListener()
{
	// ��������
	auto listener = EventListenerKeyboard::create();
	// ��¼����
	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keymap[keyCode] == false) {
			keyClick(keyCode);
		}
		keymap[keyCode] = true;
	};
	// ��¼�ſ�
	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keymap[keyCode] == true) {
			keyRelease(keyCode);
		}
		keymap[keyCode] = false;
	};
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	// ��ӵ��¼��ַ���
	dispatcher->addEventListenerWithSceneGraphPriority(listener, layer);
}



void MainController::keyClick(EventKeyboard::KeyCode code)
{
	//CCLOG("%d",code);
	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_A:
		clickDir = -1;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		clickDir = 1;
		break;
	case  EventKeyboard::KeyCode::KEY_K:
		prejumpStart = clock();
		break;
	case EventKeyboard::KeyCode::KEY_J:
		if (isWall==true&&isGround==false)
			isHold = true;
		break;
	default:
		break;
	}
}

void MainController::keyPress()
{
	// ��������ʱ�Ĵ���
	if (isHold == false) {
		// ��û��ץ��ǽ��ʱ��������X
		if (keymap[EventKeyboard::KeyCode::KEY_D] == true && clickDir >= 0) {
			player->setAnimation(AniState::RUN);
			// �������ҷ���
			player->isRight=true;
			player->setAcceX(RUNACCE, isGround);
		}
		if (keymap[EventKeyboard::KeyCode::KEY_A] == true && clickDir <= 0) {
			player->setAnimation(AniState::RUN);
			// �������ҷ���
			player->isRight = false;
			player->setAcceX(-RUNACCE, isGround);
		}
	}
	if (keymap[EventKeyboard::KeyCode::KEY_K] == true) {
		if (jumpStart != 0) {
			if (clock() - jumpStart >= JUMPTIME) {
				// ������Ծʱ��ʱ
				jumpStart = 0;
			}
			else if (clock() - jumpStart <= JUMPTIME*UPRATE) {
				// ��Ծ��һ�׶�
				player->setAnimation(AniState::JUMP);
				//player->setDir(Dir::UP);
				player->air(1);
			}
			else {
				// ��Ծ�����׶�
				player->air(3);
			}
		}
	}
	if (keymap[EventKeyboard::KeyCode::KEY_J] == true && isWall==true&&isGround == false) {
		// ��ס����ʱ�ж��Ƿ�ץסǽ��
		isHold = true;
		// ֹͣ����
		player->air(4);
		if(player->isRight)
			player->setAcceX(5, isGround);
		else
			player->setAcceX(-5, isGround);
		// ������������
	}

	if (keymap[EventKeyboard::KeyCode::KEY_A] == false && keymap[EventKeyboard::KeyCode::KEY_D] == false) {
		// ���Ҽ����ſ�ʱ
		clickDir = 0;
		player->setAcceX(0, isGround);
	}
}

void MainController::keyRelease(EventKeyboard::KeyCode code)
{
	//CCLOG("%d", code);
	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_D:
	{
		player->setAnimation(AniState::IDLE);
		--clickDir;
		break;
	}
	case EventKeyboard::KeyCode::KEY_A:
	{
		player->setAnimation(AniState::IDLE);
		++clickDir;
		break;
	}
	case EventKeyboard::KeyCode::KEY_K:
	{
		player->setAnimation(AniState::FALL);
		jumpStart = 0;
		break;
	}
	case EventKeyboard::KeyCode::KEY_J:
	{
		isHold = false;
		break;
	}
	default:
		break;
	}
}

PlayerCol MainController::getNewPos(Vec2 speed)
{
	TMXLayer* platforms = map->getLayer(PLATFORM_LAYER);
	auto center = player->getSpite();
	auto cenSize = center->getContentSize();
	auto cenX = center->getPositionX();
	auto cenY = center->getPositionY();
	auto speedX = speed.x;
	auto speedY = speed.y;
	auto newX = cenX, newY = cenY;
	int xCol = 0;
	int yCol = 0;

	// �����µ�Xλ��
	if (speedX != 0) {
		// ȡ����
		int mark;
		// raybox�ļ��㽨����center��ê�������ĵ������
		Rect raybox;
		if (speedX > 0) {
			mark = 1;
			// ���ƴӽ�ɫ���½�����speed���ȵĺ�
			raybox.setRect(cenX + cenSize.width / 2, cenY - cenSize.height / 2, speedX, cenSize.height);
		}
		else {
			// ���ƴӽ�ɫ���speedԶ�����½�����speed���ȵĺ�
			mark = -1;
			raybox.setRect(cenX - mark*speedX - cenSize.width / 2, cenY - cenSize.height / 2, mark*speedX, cenSize.height);
		}

		newX += speedX;

		int w = map->getMapSize().width;
		int h = map->getMapSize().height;
		float minDisX = 1000;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = platforms->getTileAt(Vec2(x, y));	// ��tile������ȡ����Ӧ�ľ���
				if (!sprite)	// ��ֹspriteΪNULL
					continue;
				Rect box = sprite->getBoundingBox();
				if (raybox.intersectsRect(box)) {
					//CCLOG("%d %d", x, y);
					// ֻѡ����ӽ���һ��
					float dis = abs(cenX - box.getMidX());
					if (dis < minDisX) {
						// ���õ��µ�λ��
						minDisX = dis;
						// Ҫ�����Ų��һ���(������������)��ֹ��⵽����ͬ�ķ���
						newX = box.getMidX() - mark*(box.size.width + cenSize.width) / 2 - 1 * mark;
						xCol = mark;
					}
				}
			}
		}
	}

	if (speedY != 0) {
		// ȡ����
		int mark;
		// raybox�ļ��㽨����center��ê�������ĵ������
		Rect raybox;
		if (speedY > 0) {
			mark = 1;
			// ���ƴӽ�ɫ���Ͻ�����speed���ȵĺ�
			raybox.setRect(cenX - cenSize.width / 2, cenY + cenSize.height / 2, cenSize.width, speedY);
		}
		else {
			// ���ƴӽ�ɫ�±�speedԶ�����½�����speed���ȵĺ�
			mark = -1;
			raybox.setRect(cenX - cenSize.width / 2, cenY - mark*speedY - cenSize.height / 2, cenSize.width, mark*speedY);
		}

		newY += speedY;

		int w = map->getMapSize().width;
		int h = map->getMapSize().height;
		float minDisX = INT_MAX;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = platforms->getTileAt(Vec2(x, y));	// ��tile������ȡ����Ӧ�ľ���
				if (!sprite)	// ��ֹspriteΪNULL
					continue;
				Rect box = sprite->getBoundingBox();
				if (raybox.intersectsRect(box)) {
					//CCLOG("%d %d", x, y);
					// ֻѡ����ӽ���һ��
					float dis = abs(cenY - box.getMidY());
					if (dis < minDisX) {
						// ���õ��µ�λ��
						minDisX = dis;
						// Ҫ�����Ų��һ���(��������)��ֹ��⵽����ͬ�ķ���
						newY = box.getMidY() - mark*(box.size.height + cenSize.height) / 2 - 1 * mark;
						yCol = mark;
					}
				}
			}
		}
	}
	PlayerCol ret;
	ret.newPos = Vec2(newX, newY);
	ret.xCol = xCol;
	ret.yCol = yCol;
	return ret;
}
