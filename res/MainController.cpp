#include "MainController.h"
#include "GameManager.h"
#include "Character\Player.h"
#include "proj.win32\res\MainConfig.h"


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

void MainController::update(float dt)
{
	// ˢ�µ�ǰ֡
	this->dt = dt;
	// ˢ�¼�ʱ��
	timer();
	// �Ƿ���ϵͳģʽ
	if (isSysMode) {
		sysMode();
	}
	else {
		// ������ס��Ӧ
		keyPress();
		// ��������(����������֮��)
		environment();
		// �ж���ʱ״̬
		condition();
		// ������״̬
		auto Condition = getNewPos(player->getSpeed());
		//CCLOG("%f, %f", player->getSpeed().x, player->getSpeed().y);
		// Ӧ����λ��
		player->toNewPos(Condition.newPos);
		player->update(dt);
	}
}

// ��ʼ������������
void MainController::addKeyListener()
{
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


// �������µ���Ӧ
void MainController::keyClick(EventKeyboard::KeyCode code)
{
	//CCLOG("%d",code);
	if (!isSysMode) {
		switch (code)
		{
		case EventKeyboard::KeyCode::KEY_A:
			clickDirX = -1;
			break;
		case EventKeyboard::KeyCode::KEY_D:
			clickDirX = 1;
			break;
		case EventKeyboard::KeyCode::KEY_S:
			clickDirY = -1;
			break;
		case EventKeyboard::KeyCode::KEY_W:
			clickDirY = 1;
			break;
		case  EventKeyboard::KeyCode::KEY_K:
			player->jump();
			break;
		case EventKeyboard::KeyCode::KEY_J:
			player->hold(true);
			break;
		case EventKeyboard::KeyCode::KEY_L:
			//dash�İ����ж�
			if (keymap[EventKeyboard::KeyCode::KEY_D] == true && clickDirX >= 0) {
				if (keymap[EventKeyboard::KeyCode::KEY_W] == true && clickDirY >= 0) {
					//WD
					player->dash(2);
				}
				else if (keymap[EventKeyboard::KeyCode::KEY_S] == true && clickDirY <= 0) {
					//DS
					player->dash(4);
				}
				else {
					//������D
					player->dash(3);
				}
			}
			else if (keymap[EventKeyboard::KeyCode::KEY_A] == true && clickDirX <= 0) {
				if (keymap[EventKeyboard::KeyCode::KEY_W] == true && clickDirY >= 0) {
					//AW
					player->dash(8);
				}
				else if (keymap[EventKeyboard::KeyCode::KEY_S] == true && clickDirY <= 0) {
					//SA
					player->dash(6);
				}
				else {
					//������A
					player->dash(7);
				}
			}
			else if (keymap[EventKeyboard::KeyCode::KEY_W] == true && clickDirY >= 0) {
				//������W
				player->dash(1);
			}
			else if (keymap[EventKeyboard::KeyCode::KEY_S] == true && clickDirY <= 0) {
				//������S
				player->dash(5);
			}
			else {
				//û��ƥ�䵽�İ���
				player->dash(0);
			}
			break;
		default:
			break;
		}
	}
}

// ������ס����Ӧ�����ð�����ʵ��
void MainController::keyPress()
{
	// ��ס����ʱ�ж��Ƿ�ץסǽ��
	if (keymap[EventKeyboard::KeyCode::KEY_J] == true) {
		player->hold(true);
	}

	// ��������ʱ�Ĵ���
		// ����
	if (keymap[EventKeyboard::KeyCode::KEY_D] == true && clickDirX >= 0) {
		player->run(1);
	}
	if (keymap[EventKeyboard::KeyCode::KEY_A] == true && clickDirX <= 0) {
		player->run(-1);
	}
	// ����
	if (keymap[EventKeyboard::KeyCode::KEY_W] == true && clickDirY >= 0) {
		player->climb(1);
	}
	if (keymap[EventKeyboard::KeyCode::KEY_S] == true && clickDirY <= 0) {
		player->climb(-1);
	}

	if (keymap[EventKeyboard::KeyCode::KEY_A] == false && keymap[EventKeyboard::KeyCode::KEY_D] == false) {
		// ���Ҽ����ſ�ʱ
		clickDirX = 0;
		player->slow(SCENE_X);
	}

	if (keymap[EventKeyboard::KeyCode::KEY_W] == false && keymap[EventKeyboard::KeyCode::KEY_S] == false) {
		// ���¼����ſ�ʱ
		clickDirY = 0;
		player->climb(0);
	}
}

void MainController::sysMode()
{
	// ��������(����������֮��)
	environment();
	// �ж���ʱ״̬
	condition();
	// ������״̬
	auto Condition = getNewPos(player->getSpeed());
	//CCLOG("%f, %f", player->getSpeed().x, player->getSpeed().y);
	// Ӧ����λ��
	player->toNewPos(Condition.newPos);
	player->update(dt);
}

// ������
void MainController::environment()
{
	//Y����
	if (openY) {
		player->fall(SCENE_Y);
	}
	//X����
	if (openX) {
		//player->wind(SCENE_X);
	}
}

// �Էſ������¼�����Ӧ
void MainController::keyRelease(EventKeyboard::KeyCode code)
{
	//CCLOG("%d", code);
	if (!isSysMode) {
		switch (code)
		{
			//ǰ��һ����Ϊ���Ż�wasd���´�����Ӱ����ָ�
		case EventKeyboard::KeyCode::KEY_D:
		{
			player->setAnimation(AniState::IDLE);
			--clickDirX;
			break;
		}
		case EventKeyboard::KeyCode::KEY_A:
		{
			player->setAnimation(AniState::IDLE);
			++clickDirX;
			break;
		}
		case EventKeyboard::KeyCode::KEY_W:
		{
			player->setAnimation(AniState::IDLE);
			--clickDirY;
			break;
		}
		case EventKeyboard::KeyCode::KEY_S:
		{
			player->setAnimation(AniState::IDLE);
			++clickDirY;
			break;
		}
		case EventKeyboard::KeyCode::KEY_K:
		{
			player->setAnimation(AniState::FALL);
			// ֹͣ��Ծ
			player->jumpend();
			break;
		}
		case EventKeyboard::KeyCode::KEY_J:
		{
			// �ſ�ǽ��
			player->hold(false);
			break;
		}
		default:
			break;
		}
	}
}

// �ж�״̬��Ҳ���Ǹ�����ײ�����
void MainController::condition()
{
	auto Condition = getNewPos(player->getSpeed());
	//CCLOG("%f, %f", player->getSpeed().x, player->getSpeed().y);

	if (Condition.yCol == -1) {
		// ���
		player->ground(true);
	}
	else if (Condition.yCol == 1) {
		// ��ײ��ͷʱ��ǿ�ƿ�ʼ����
		player->headCol();
	}
	else if (Condition.yCol == 0) {
		// �뿪����
		player->ground(false);
	}
	if (Condition.xCol == 1 || Condition.xCol == -1) {
		// ǽ�
		player->slip(Condition.xCol);
		player->jumpend();
	}
	else if (Condition.xCol == 0) {
		// �뿪ǽ
		player->slip(Condition.xCol);
	}
	//CCLOG("%d", Condition.xCol);
}

void MainController::timer()
{
	if (sysTimer > 0) {
		isSysMode = true;
		sysTimer -= dt;
		if (sysTimer <= 0) {
			isSysMode = false;
			sysTimer = 0;
		}
	}
}

// ���ؿɴ����λ�ú���ײ���
PlayerCol MainController::getNewPos(Vec2 speed)
{
	TMXLayer* platforms = map->getLayer(PLATFORM_LAYER);
	auto center = player->getSprite();
	auto cenSize = center->getContentSize();
	auto cenX = center->getPositionX();
	auto cenY = center->getPositionY();
	//���ϱ仯��ʱ����ʹ��Ϸ��֡���޹�
	auto speedX = speed.x*dt;
	auto speedY = speed.y*dt;
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
					float dis = abs(cenX - box.getMidX());
					if (dis < minDisX) {
						// ���õ��µ�λ��
						minDisX = dis;
						// Ҫ�����Ų��һ���(������������)��ֹ��⵽����ͬ�ķ���
						newX = box.getMidX() - mark*(box.size.width + cenSize.width) / 2 - 1 * mark;
					}
					xCol = mark;
				}
			}
		}
	}

	// �����µ�Yλ��
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
		float minDisY = INT_MAX;
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
					if (dis < minDisY) {
						// ���õ��µ�λ��
						minDisY = dis;
						// Ҫ�����Ų��һ���(��������)��ֹ��⵽����ͬ�ķ���
						newY = box.getMidY() - mark*(box.size.height + cenSize.height) / 2 - 1 * mark;
					}
					yCol = mark;
				}
			}
		}
	}

	// ���ؼ���õ���λ��
	PlayerCol ret;
	ret.newPos = Vec2(newX, newY);
	ret.xCol = xCol;
	ret.yCol = yCol;
	return ret;
}
