#include "MainController.h"
#include "Character/Player.h"

MainController::MainController()
{
}

MainController::~MainController()
{
	controller = NULL;
}

MainController* MainController::controller = NULL;
MainController* MainController::getInstance(Player* in, cocos2d::Layer* layer)
{
	if (controller == NULL)
	{
		controller = new MainController();
	}
	if (in != nullptr&&layer != nullptr) {
		controller->player = in;
		controller->layer = layer;
		return controller;
	}
	return NULL;
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
	default:
		break;
	}
}

void MainController::keyPress()
{
	if (keymap[EventKeyboard::KeyCode::KEY_D] == true) {
		player->setAnimation(AniState::RUN);
		player->setDir(Dir::RIGHT);
		player->move(Vec2(player->runSpeed.x, 0));
	}
	else if (keymap[EventKeyboard::KeyCode::KEY_A] == true) {
		player->setAnimation(AniState::RUN);
		player->setDir(Dir::LEFT);
		player->move(Vec2(-player->runSpeed.x, 0));
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
		break;
	}
	case EventKeyboard::KeyCode::KEY_A:
	{
		player->setAnimation(AniState::IDLE);
		break;
	}
	default:
		break;
	}
}