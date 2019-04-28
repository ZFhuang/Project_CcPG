#include"Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::init(Vec2 pos)
{
	if (center == nullptr) {
		// 添加玩家
		center = Sprite::create(PLAYER_IMG_PATH[0]);
		// 强制设置大小
		center->setContentSize(Size(PLAYER_WIDTH, PLAYER_HEIGHT));
		// 设置tag
		center->setTag(PLAYER_TAG);

		// 注意要调好锚点方便接下来的旋转等操作
		center->setAnchorPoint(Vec2(0.5, 0.5));
		// 刷新大小
		PLAYER_WIDTH = center->getContentSize().width;
		PLAYER_HEIGHT = center->getContentSize().height;

		// 添加物理碰撞盒
		auto size = center->getContentSize();
		auto body = PhysicsBody::createBox(center->getContentSize());
		body->setGravityEnable(false);
		//center->setPhysicsBody(body);
		// 初始位置
		center->setPosition(pos);
	}
	setAnimation(AniState::IDLE);
}

void Player::setAnimation(AniState state)
{
	if (state == nowAni) {
		return;
	}
	nowAni = state;
	//CCLOG("%d", nowAni);

	// 帧动画向量
	Vector<SpriteFrame*> frameVector;
	Animation* animation = nullptr;

	// 各种动画的载入
	switch (nowAni)
	{
	case IDLE:
	{
		// 玩家常态动画占位
		frameVector.pushBack(SpriteFrame::create(PLAYER_IMG_PATH[0], Rect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT)));
		animation = Animation::createWithSpriteFrames(frameVector);
		animation->setDelayPerUnit(1.0f);
		break;
	}
	case RUN:
	{
		// 玩家跑动动画占位
		for (int i = 0; i < 4; i++)
		{
			auto spriteFrame = SpriteFrame::create(PLAYER_IMG_PATH[i], Rect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));
			frameVector.pushBack(spriteFrame);
		}
		animation = Animation::createWithSpriteFrames(frameVector);
		// 设置播放动画的两帧间隔时间
		animation->setDelayPerUnit(1.0f / 4.0f);
		break;
	}
	default:
	{
		break;
	}
	}

	// 启动动画
	if (animation != nullptr) {
		// 先停止正在执行的所有动画
		center->stopAllActions();
		animate = Animate::create(animation);
		// 运行新的动画
		center->runAction(RepeatForever::create(animate));
	}
}

bool Player::moveTo(Vec2 pos, Vec2 speed)
{
	// 计算出位置差值然后再加上增量
	// 当位置在其附近时将会不能移动,返回true表示到达
	Vec2 delta = pos - center->getPosition();
	// 获得其归一化向量
	Vec2 dir = delta.getNormalized();
	// x是否在附近
	if (delta.x - dir.x*speed.x<1 && delta.x - dir.x*speed.x>-1) {
		speed.x = 0;
	}
	else {
		speed.x *= dir.x;
	}
	// y是否在附近
	if (delta.y - dir.y*speed.y<1 && delta.y - dir.y*speed.y>-1) {
		speed.y = 0;
	}
	else {
		speed.y *= dir.y;
	}
	// 都在附近返回true
	if (speed.x == 0 && speed.y == 0) {
		return true;
	}
	//否则移动
	center->setPosition(center->getPosition() + speed);
	return false;
}

void Player::move(Vec2 pos)
{
	center->setPosition(pos);
}

//void Player::setDir(Dir dir)
//{
//	if (this->dir == dir) {
//		return;
//	}
//	this->dir = dir;
//
//	// 加了物理后翻转出现bug
//	// 翻转不要用flip，因为flip的翻转是无关锚点的
//	//if (this->dir == Dir::LEFT) {
//	//	center->setRotationY(180);
//	//}
//	//else {
//	//	center->setRotationY(0);
//	//}
//}

Sprite* Player::getSpite()
{
	return center;
}

void Player::setAcceX(float x,bool isGround)
{
	//X轴移动
	if (x > 0) {
		if (Speed.x + x < MAX_PLAYER_SPEED_X) {
			Speed.x += x;
		}
		else {
			Speed.x = MAX_PLAYER_SPEED_X;
		}
	}
	else if (x < 0) {
		if (Speed.x + x > -MAX_PLAYER_SPEED_X) {
			Speed.x += x;
		}
		else {
			Speed.x = -MAX_PLAYER_SPEED_X;
		}
	}
	else {
		//惯性设置，分两个状态
		if (Speed.x > 0) {
			if (isGround) {
				//地面
				if (Speed.x < SLOW_DOWN_X) {
					Speed.x = 0;
				}
				Speed.x -= SLOW_DOWN_X;
			}
			else {
				//空中惯性更大点
				if (Speed.x < SLOW_DOWN_AIR) {
					Speed.x = 0;
				}
				Speed.x -= SLOW_DOWN_AIR;
			}
		}
		else if (Speed.x < 0) {
			//对称
			if (isGround) {
				if (Speed.x > -SLOW_DOWN_X) {
					Speed.x = 0;
				}
				Speed.x += SLOW_DOWN_X;
			}
			else {
				if (Speed.x >- SLOW_DOWN_AIR) {
					Speed.x = 0;
				}
				Speed.x += SLOW_DOWN_AIR;
			}
		}
		//else
			//dir = Dir::STOP;
	}
}

void Player::air(int step)
{
	switch (step)
	{
	case 1:
		// 一阶段，上升
		if (Speed.y + JUMP_ACCE > MAX_SPEED_JUMP) {
			Speed.y += JUMP_ACCE;
		}
		else {
			Speed.y = MAX_SPEED_JUMP;
		}
		break;
	case 3:
		// 三阶段，减速上升并滑一段
		if (Speed.y > -4) {
			Speed.y -=0.5; 
		}
		else if (Speed.y < -4) {
			Speed.y = 0;
		}
		break;
	case 4:
		// 抓墙一阶段，不会下降
		Speed.y = 0;
		break;
	case 5:
		// 五阶段，下落
		if (Speed.y - FALL_ACCE > -MAX_SPEED_FALL) {
			Speed.y -= FALL_ACCE;
		}
		else {
			Speed.y =- MAX_SPEED_FALL;
		}
		break;
	case 6:
		// 滑墙或抓墙二阶段，缓慢下降
		if (Speed.y - SLIP_ACCE > -Max_SPEED_SLIP) {
			Speed.y -= SLIP_ACCE;
		}
		else {
			Speed.y = -Max_SPEED_SLIP;
		}
		break;
	case 7:
		// 七阶段，落地
		Speed.y = 0;
		break;
	default:
		break;
	}

}

Vec2 Player::getSpeed()
{
	return Speed;
}
