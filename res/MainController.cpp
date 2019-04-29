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
	// 尝试下落
	player->setAir(5);
	// 按键按下
	keyPress();
	// 处理按键和下落判定后的临时状态的改变
	condition();
	// 矫正新状态
	auto Condition = getNewPos(player->getSpeed());
	//CCLOG("%lf %lf", Condition.newPos.x, Condition.newPos.y);
	// 应用新位置
	player->move(Condition.newPos);
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
		prejumpStart = clock();
		break;
	case EventKeyboard::KeyCode::KEY_J:
		if (isWall == true && isGround == false)
			isHold = true;
		break;
	default:
		break;
	}
}

void MainController::keyPress()
{
	// 按住按键时判断是否抓住墙壁
	if (keymap[EventKeyboard::KeyCode::KEY_J] == true && isWall == true && isGround == false) {
		if (player->calEnergy(clock()) == true) {
			// 还有能量时，停止下落
			isHold = true;
			player->setAir(4);
			if (player->isRight)
				player->setAcceX(5, isGround);
			else
				player->setAcceX(-5, isGround);
		}
		else {
			// 无能量时设置为普通下滑
			isHold = false;
		}
	}
	else if (keymap[EventKeyboard::KeyCode::KEY_J] == false || isWall == false || isGround == true) {
		//不再是爬墙时
		isHold = false;
	}

	// 按键按下时的处理
	if (isHold == false) {
		// 当没有抓着墙壁时才允许触发X
		if (keymap[EventKeyboard::KeyCode::KEY_D] == true && clickDirX >= 0) {
			player->setAnimation(AniState::RUN);
			// 设置左右方向
			player->isRight = true;
			player->setAcceX(RUNACCE, isGround);
		}
		if (keymap[EventKeyboard::KeyCode::KEY_A] == true && clickDirX <= 0) {
			player->setAnimation(AniState::RUN);
			// 设置左右方向
			player->isRight = false;
			player->setAcceX(-RUNACCE, isGround);
		}
	}
	else {
		// 当有抓着墙壁时才允许触发Y
		if (keymap[EventKeyboard::KeyCode::KEY_W] == true && clickDirY >= 0) {
			player->setAnimation(AniState::RUN);
			player->setAcceY(CLIMBACCE);
		}
		if (keymap[EventKeyboard::KeyCode::KEY_S] == true && clickDirY <= 0) {
			player->setAnimation(AniState::RUN);
			player->setAcceY(-CLIMBACCE);
		}
	}

	if (keymap[EventKeyboard::KeyCode::KEY_K] == true) {
		if (jumpStart != 0) {
			if (clock() - jumpStart >= JUMPTIME) {
				// 超过跳跃时间时
				jumpStart = 0;
			}
			else if (clock() - jumpStart <= JUMPTIME*UPRATE) {
				// 跳跃的一阶段
				player->setAnimation(AniState::JUMP);
				//player->setDir(Dir::UP);
				player->setAir(1);
			}
			else {
				// 跳跃的三阶段
				player->setAir(3);
			}
		}
	}

	if (keymap[EventKeyboard::KeyCode::KEY_A] == false && keymap[EventKeyboard::KeyCode::KEY_D] == false) {
		// 左右键都放开时
		clickDirX = 0;
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

void MainController::condition()
{
	auto Condition = getNewPos(player->getSpeed());
	if (isGround == true && jumpStart == 0 && clock() - prejumpStart <= FAULT_JUMPTIME) {
		isGround = false;
		jumpStart = clock();
	}
	if (clock() - prejumpStart > FAULT_JUMPTIME) {
		prejumpStart = 0;
	}
	if (Condition.yCol == 1) {
		// 当撞到头时，强制开始下落
		player->setAir(5);
		jumpStart = 0;
	}
	if (Condition.yCol == -1) {
		// 落地
		player->setAir(7);
		isGround = true;
		fallStart = 0;
		player->calEnergy(-1);
	}
	if (Condition.yCol == 0) {
		// 离开地面
		if (fallStart == 0)
			fallStart = clock();
	}
	if (Condition.xCol == 1 || Condition.xCol == -1) {
		// 墙璧
		isWall = true;
		if (isGround == false && isHold == false) {
			// 没有抓住时，滑墙
			jumpStart = 0;
			player->setAir(6);
		}
	}
	else if (Condition.xCol == 0) {
		// 离开墙壁
		isWall = false;
	}
	if (fallStart != 0 && clock() - fallStart > FAULT_FALLTIME) {
		isGround = false;
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

	// 计算新的X位置
	if (speedX != 0) {
		// 取正负
		int mark;
		// raybox的计算建立在center的锚点在中心的情况下
		Rect raybox;
		if (speedX > 0) {
			mark = 1;
			// 绘制从角色右下角延伸speed长度的盒
			raybox.setRect(cenX + cenSize.width / 2, cenY - cenSize.height / 2, speedX, cenSize.height);
		}
		else {
			// 绘制从角色左边speed远的左下角延伸speed长度的盒
			mark = -1;
			raybox.setRect(cenX - mark*speedX - cenSize.width / 2, cenY - cenSize.height / 2, mark*speedX, cenSize.height);
		}

		newX += speedX;

		int w = map->getMapSize().width;
		int h = map->getMapSize().height;
		float minDisX = 1000;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = platforms->getTileAt(Vec2(x, y));	// 从tile的坐标取出对应的精灵
				if (!sprite)	// 防止sprite为NULL
					continue;
				Rect box = sprite->getBoundingBox();
				if (raybox.intersectsRect(box)) {
					//CCLOG("%d %d", x, y);
					// 只选择最接近的一个
					float dis = abs(cenX - box.getMidX());
					if (dis < minDisX) {
						// 放置到新的位置
						minDisX = dis;
						// 要往外多挪动一点点(几个像素像素)防止检测到轴向不同的方块
						newX = box.getMidX() - mark*(box.size.width + cenSize.width) / 2 - 1 * mark;
						xCol = mark;
					}
				}
			}
		}
	}

	// 计算新的Y位置
	if (speedY != 0) {
		// 取正负
		int mark;
		// raybox的计算建立在center的锚点在中心的情况下
		Rect raybox;
		if (speedY > 0) {
			mark = 1;
			// 绘制从角色左上角延伸speed长度的盒
			raybox.setRect(cenX - cenSize.width / 2, cenY + cenSize.height / 2, cenSize.width, speedY);
		}
		else {
			// 绘制从角色下边speed远的左下角延伸speed长度的盒
			mark = -1;
			raybox.setRect(cenX - cenSize.width / 2, cenY - mark*speedY - cenSize.height / 2, cenSize.width, mark*speedY);
		}

		newY += speedY;

		int w = map->getMapSize().width;
		int h = map->getMapSize().height;
		float minDisX = INT_MAX;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				Sprite* sprite = platforms->getTileAt(Vec2(x, y));	// 从tile的坐标取出对应的精灵
				if (!sprite)	// 防止sprite为NULL
					continue;
				Rect box = sprite->getBoundingBox();
				if (raybox.intersectsRect(box)) {
					//CCLOG("%d %d", x, y);
					// 只选择最接近的一个
					float dis = abs(cenY - box.getMidY());
					if (dis < minDisX) {
						// 放置到新的位置
						minDisX = dis;
						// 要往外多挪动一点点(几个像素)防止检测到轴向不同的方块
						newY = box.getMidY() - mark*(box.size.height + cenSize.height) / 2 - 1 * mark;
						yCol = mark;
					}
				}
			}
		}
	}

	// 返回计算好的新位置
	PlayerCol ret;
	ret.newPos = Vec2(newX, newY);
	ret.xCol = xCol;
	ret.yCol = yCol;
	return ret;
}
