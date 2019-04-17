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
		player->move(Vec2(player->runSpeed.x, 0));
	}
	else if (keymap[EventKeyboard::KeyCode::KEY_A] == true && clickDir <= 0) {
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

Vec2 MainController::collideMap(Vec2 speed)
{
	double newX, newY;
	auto spite = player->getSpite();

	// 玩家相对于屏幕的坐标
	float player_screen_x = spite->getPositionX();
	float player_screen_y = spite->getPositionY() - spite->getContentSize().height / 2;

	// 地图相对于屏幕的x坐标
	float map_screen_x = abs(map->getPositionX());
	float map_screen_y = 0;

	// 玩家相对于地图的坐标
	float player_map_x = player_screen_x + map_screen_x;
	float player_map_y = player_screen_y + map_screen_y;

	//int playerTiledID = map->getLayer(PLATFORM)->getTileGIDAt(Vec2((int)(player_map_x / map->getTileSize().width), (int)(map->getMapSize().height - 1 - player_map_y / map->getTileSize().height)));
	////增加148，132两块，需要加一
	//if ((m_isJump == false) && (m_jumpDir == Dir::STOP) && (playerTiledID != 8) && (playerTiledID != 7)
	//	&& (playerTiledID != 151) && (playerTiledID != 170) && (playerTiledID != 171) && (playerTiledID != 172) && (playerTiledID != 149) && (playerTiledID != 133)) {
	//	m_isJump = true;
	//	m_jumpDir = Dir::DOWN;
	//	m_jumpSpeed = PLAYER_DOWN_SPEED;
	//}

	return Vec2(newX, newY);
}
