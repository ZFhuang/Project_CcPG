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
		//center->setContentSize(Size(PLAYER_WIDTH, PLAYER_HEIGHT));
		// 设置tag
		center->setTag(PLAYER_TAG);

		// 注意要调好锚点方便接下来的旋转等操作
		//center->setAnchorPoint(Vec2(0.5, 0.5));
		// 刷新大小
		//PLAYER_WIDTH = center->getContentSize().width;
		//PLAYER_HEIGHT = center->getContentSize().height;

		// 添加物理碰撞盒
		auto size = center->getContentSize();
		auto body = PhysicsBody::createBox(center->getContentSize());
		
		center->setPhysicsBody(body);
		// 初始位置
		center->setPosition(pos);

		initTrigger();
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

void Player::moveX(double speed)
{
	if (speed > 0) {
		if (Speed.x + speed < MAX_PLAYER_SPEED_X) {
			Speed.x += speed;
		}
		else {
			Speed.x = MAX_PLAYER_SPEED_X;
		}
	}
	else if (speed < 0) {
		if (Speed.x + speed > -MAX_PLAYER_SPEED_X) {
			Speed.x += speed;
		}
		else {
			Speed.x = -MAX_PLAYER_SPEED_X;
		}
	}
	else {
		Speed.x = 0;
	}
	refreshTrigger();
	center->setPositionX(center->getPositionX() + Speed.x);
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

void Player::setDir(Dir dir)
{
	if (this->dir == dir) {
		return;
	}
	this->dir = dir;

	// 加了物理后翻转出现bug
	// 翻转不要用flip，因为flip的翻转是无关锚点的
	//if (this->dir == Dir::LEFT) {
	//	center->setRotationY(180);
	//}
	//else {
	//	center->setRotationY(0);
	//}
}

Sprite* Player::getSpite()
{
	return center;
}

void Player::refreshTrigger()
{
	//CCLOG("%f", Speed.x);
	triggerX->setPositionOffset(Vec2(Speed.x, 0));
	triggerY->setPositionOffset(Vec2(0, Speed.y));
}

void Player::initTrigger()
{
	auto size = center->getContentSize();

	//增加triggerX
	Node *tx = new Node();
	tx->setPosition(size.width / 2, size.height / 2);
	center->addChild(tx);
	triggerX = PhysicsBody::createEdgeBox(size);
	// CollisionBitmask与后结果为0表示关闭碰撞
	triggerX->setCollisionBitmask(0);
	tx->setPhysicsBody(triggerX);

	//增加triggerY
	Node *ty = new Node();
	ty->setPosition(size.width / 2, size.height / 2);
	center->addChild(ty);
	triggerY = PhysicsBody::createEdgeBox(size);
	// CollisionBitmask与后结果为0表示关闭碰撞
	triggerY->setCollisionBitmask(0);
	ty->setPhysicsBody(triggerY);
}
