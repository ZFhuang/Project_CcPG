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

void MainController::update(float dt)
{
	// 刷新当前帧
	this->dt = dt;
	// 刷新计时器
	timer();
	// 是否在系统模式
	if (isSysMode) {
		sysMode();
	}
	else {
		// 按键按住响应
		keyPress();
		// 环境处理(重力，风向之类)
		environment();
		// 判定临时状态
		condition();
		// 矫正新状态
		auto Condition = getNewPos(player->getSpeed());
		//CCLOG("%f, %f", player->getSpeed().x, player->getSpeed().y);
		// 应用新位置
		player->toNewPos(Condition.newPos);
		player->update(dt);
	}
}

// 初始化按键监听器
void MainController::addKeyListener()
{
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


// 按键按下的响应
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
			//dash的按键判断
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
					//单个的D
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
					//单个的A
					player->dash(7);
				}
			}
			else if (keymap[EventKeyboard::KeyCode::KEY_W] == true && clickDirY >= 0) {
				//单个的W
				player->dash(1);
			}
			else if (keymap[EventKeyboard::KeyCode::KEY_S] == true && clickDirY <= 0) {
				//单个的S
				player->dash(5);
			}
			else {
				//没有匹配到的按键
				player->dash(0);
			}
			break;
		default:
			break;
		}
	}
}

// 按键按住的响应，利用按键表实现
void MainController::keyPress()
{
	// 按住按键时判断是否抓住墙壁
	if (keymap[EventKeyboard::KeyCode::KEY_J] == true) {
		player->hold(true);
	}

	// 按键按下时的处理
		// 左右
	if (keymap[EventKeyboard::KeyCode::KEY_D] == true && clickDirX >= 0) {
		player->run(1);
	}
	if (keymap[EventKeyboard::KeyCode::KEY_A] == true && clickDirX <= 0) {
		player->run(-1);
	}
	// 上下
	if (keymap[EventKeyboard::KeyCode::KEY_W] == true && clickDirY >= 0) {
		player->climb(1);
	}
	if (keymap[EventKeyboard::KeyCode::KEY_S] == true && clickDirY <= 0) {
		player->climb(-1);
	}

	if (keymap[EventKeyboard::KeyCode::KEY_A] == false && keymap[EventKeyboard::KeyCode::KEY_D] == false) {
		// 左右键都放开时
		clickDirX = 0;
		player->slow(SCENE_X);
	}

	if (keymap[EventKeyboard::KeyCode::KEY_W] == false && keymap[EventKeyboard::KeyCode::KEY_S] == false) {
		// 上下键都放开时
		clickDirY = 0;
		player->climb(0);
	}
}

void MainController::sysMode()
{
	// 环境处理(重力，风向之类)
	environment();
	// 判定临时状态
	condition();
	// 矫正新状态
	auto Condition = getNewPos(player->getSpeed());
	//CCLOG("%f, %f", player->getSpeed().x, player->getSpeed().y);
	// 应用新位置
	player->toNewPos(Condition.newPos);
	player->update(dt);
}

// 环境力
void MainController::environment()
{
	//Y重力
	if (openY) {
		player->fall(SCENE_Y);
	}
	//X阻力
	if (openX) {
		//player->wind(SCENE_X);
	}
}

// 对放开按键事件的响应
void MainController::keyRelease(EventKeyboard::KeyCode code)
{
	//CCLOG("%d", code);
	if (!isSysMode) {
		switch (code)
		{
			//前面一段是为了优化wasd按下次序不受影响的手感
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
			// 停止跳跃
			player->jumpend();
			break;
		}
		case EventKeyboard::KeyCode::KEY_J:
		{
			// 放开墙壁
			player->hold(false);
			break;
		}
		default:
			break;
		}
	}
}

// 判断状态，也就是各种碰撞的情况
void MainController::condition()
{
	auto Condition = getNewPos(player->getSpeed());
	//CCLOG("%f, %f", player->getSpeed().x, player->getSpeed().y);

	if (Condition.yCol == -1) {
		// 落地
		player->ground(true);
	}
	else if (Condition.yCol == 1) {
		// 当撞到头时，强制开始下落
		player->headCol();
	}
	else if (Condition.yCol == 0) {
		// 离开地面
		player->ground(false);
	}
	if (Condition.xCol == 1 || Condition.xCol == -1) {
		// 墙璧
		player->slip(Condition.xCol);
		player->jumpend();
	}
	else if (Condition.xCol == 0) {
		// 离开墙
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

// 返回可达的新位置和碰撞检测
PlayerCol MainController::getNewPos(Vec2 speed)
{
	TMXLayer* platforms = map->getLayer(PLATFORM_LAYER);
	auto center = player->getSprite();
	auto cenSize = center->getContentSize();
	auto cenX = center->getPositionX();
	auto cenY = center->getPositionY();
	//乘上变化的时间以使游戏与帧率无关
	auto speedX = speed.x*dt;
	auto speedY = speed.y*dt;
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
					float dis = abs(cenX - box.getMidX());
					if (dis < minDisX) {
						// 放置到新的位置
						minDisX = dis;
						// 要往外多挪动一点点(几个像素像素)防止检测到轴向不同的方块
						newX = box.getMidX() - mark*(box.size.width + cenSize.width) / 2 - 1 * mark;
					}
					xCol = mark;
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
		float minDisY = INT_MAX;
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
					if (dis < minDisY) {
						// 放置到新的位置
						minDisY = dis;
						// 要往外多挪动一点点(几个像素)防止检测到轴向不同的方块
						newY = box.getMidY() - mark*(box.size.height + cenSize.height) / 2 - 1 * mark;
					}
					yCol = mark;
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
