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

		//initTrigger();
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
	//refreshTrigger();

	if (!leftCol&&Speed.x < 0) {
		center->setPositionX(center->getPositionX() + Speed.x);
	}
	else if (!rightCol&&Speed.x > 0) {
		center->setPositionX(center->getPositionX() + Speed.x);
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

bool Player::onXCollisionBegin(const PhysicsContact & contact)
{
	auto a = contact.getShapeA()->getBody()->getTag();
	auto b = contact.getShapeB()->getBody()->getTag();
	if ((a == 3&&b==1) || (a==1&&b == 3)) {
		if (dir == Dir::RIGHT) {
			rightCol = true;
			return true;
		}
		else if (dir == Dir::LEFT) {
			leftCol = true;
			return true;
		}
	}
	return false;
}

bool Player::onXCollisionSeperate(const PhysicsContact & contact)
{
	auto a = contact.getShapeA()->getBody()->getTag();
	auto b = contact.getShapeB()->getBody()->getTag();
	if ((a == 3 && b == 1) || (a == 1 && b == 3)) {
		if (rightCol) {
			rightCol = false;
			return true;
		}
		else if (leftCol) {
			leftCol = false;
			return true;
		}
	}
	return false;
}

bool Player::onYCollisionBegin(const PhysicsContact & contact)
{
	auto a= contact.getShapeA()->getBody()->getTag();
	auto b = contact.getShapeB()->getBody()->getTag();
	CCLOG("%d %d", a, b);
	return true;
}

bool Player::onYCollisionSeperate(const PhysicsContact & contact)
{
	return true;
}

void Player::refreshTrigger()
{
	//CCLOG("%f", Speed.x);
	triggerX->setPositionOffset(Vec2(Speed.x, 0));
	triggerY->setPositionOffset(Vec2(0, Speed.y));
}

void Player::initTrigger()
{
	//一个body的CategoryBitmask和另一个body的ContactTestBitmask的逻辑与的结果不等于0时，接触事件将被发出，否则不发送。 
	//一个body的CategoryBitmask和另一个body的CollisionBitmask的逻辑与结果不等于0时，他们将碰撞，否则不碰撞

	auto size = center->getContentSize();

	//增加triggerX
	Node *tx = new Node();
	tx->setPosition(size.width / 2, size.height / 2);
	center->addChild(tx);
	triggerX = PhysicsBody::createEdgeBox(size);
	triggerX->setCategoryBitmask(0x00000001);
	triggerX->setContactTestBitmask(0x00000010);
	triggerX->setCollisionBitmask(0);
	triggerX->setTag(1);
	tx->setPhysicsBody(triggerX);

	//增加triggerY
	Node *ty = new Node();
	ty->setPosition(size.width / 2, size.height / 2);
	center->addChild(ty);
	triggerY = PhysicsBody::createEdgeBox(size);
	triggerY->setCategoryBitmask(0x00000002);
	triggerY->setContactTestBitmask(0x00000020);
	triggerY->setCollisionBitmask(0);
	triggerY->setTag(2);
	ty->setPhysicsBody(triggerY);

	addCollideListener();
}

void Player::addCollideListener()
{
	//添加碰撞检测
	auto listenerX = EventListenerPhysicsContact::create();//创建碰撞监听
	listenerX->onContactBegin = CC_CALLBACK_1(Player::onXCollisionBegin, this);//回调函数
	listenerX->onContactSeparate = CC_CALLBACK_1(Player::onXCollisionSeperate, this);//回调函数

	auto listenerY = EventListenerPhysicsContact::create();//创建碰撞监听
	listenerY->onContactBegin = CC_CALLBACK_1(Player::onYCollisionBegin, this);//回调函数
	listenerY->onContactSeparate = CC_CALLBACK_1(Player::onYCollisionSeperate, this);//回调函数

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listenerX, triggerX->getNode()); //加入事件监听
	dispatcher->addEventListenerWithSceneGraphPriority(listenerY, triggerY->getNode()); //加入事件监听
}
