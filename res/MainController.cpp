#include "MainController.h"
#include "GameManager.h"
#include "Character/Player.h"

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
	//将此控制器与场景和精灵绑定
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
	//按键按下
	keyPress();
	//没有左右移动时
	if (clickDir == 0) {
		player->moveX(0);
	}
}

void MainController::addKeyListener()
{
	// 按键监听
	auto listener = EventListenerKeyboard::create();
	// 记录按下
	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keymap[keyCode] == false) {
			keyClick(keyCode);
		}
		keymap[keyCode] = true;
	};
	// 记录放开
	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keymap[keyCode] == true) {
			keyRelease(keyCode);
		}
		keymap[keyCode] = false;
	};
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	// 添加到事件分发器
	dispatcher->addEventListenerWithSceneGraphPriority(listener, layer);
}

void MainController::addCollideListener()
{
	//添加碰撞检测
	auto contactListener = EventListenerPhysicsContact::create();//创建碰撞监听
	contactListener->onContactBegin = CC_CALLBACK_1(onContactBegin, this);//回调函数

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(contactListener, layer); //加入事件监听
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
	default:
		break;
	}
}

void MainController::keyPress()
{
	if (keymap[EventKeyboard::KeyCode::KEY_D] == true && clickDir >= 0) {
		player->setAnimation(AniState::RUN);
		player->setDir(Dir::RIGHT);
		player->moveX(acceSpeed);
	}
	else if (keymap[EventKeyboard::KeyCode::KEY_A] == true && clickDir <= 0) {
		player->setAnimation(AniState::RUN);
		player->setDir(Dir::LEFT);
		player->moveX(-acceSpeed);
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
	default:
		break;
	}
}

bool MainController::onContactBegin(const PhysicsContact & contact)
{
	//传入物理碰撞对象
	log("contact");
	return true;
}
