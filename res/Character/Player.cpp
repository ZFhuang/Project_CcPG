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
		// ������
		center = Sprite::create(PLAYER_IMG_PATH[0]);
		// ǿ�����ô�С
		//center->setContentSize(Size(PLAYER_WIDTH, PLAYER_HEIGHT));
		// ����tag
		center->setTag(PLAYER_TAG);

		// ע��Ҫ����ê�㷽�����������ת�Ȳ���
		//center->setAnchorPoint(Vec2(0.5, 0.5));
		// ˢ�´�С
		//PLAYER_WIDTH = center->getContentSize().width;
		//PLAYER_HEIGHT = center->getContentSize().height;

		// ���������ײ��
		auto size = center->getContentSize();
		auto body = PhysicsBody::createBox(center->getContentSize());
		center->setPhysicsBody(body);
		// ��ʼλ��
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

	// ֡��������
	Vector<SpriteFrame*> frameVector;
	Animation* animation = nullptr;

	// ���ֶ���������
	switch (nowAni)
	{
	case IDLE:
	{
		// ��ҳ�̬����ռλ
		frameVector.pushBack(SpriteFrame::create(PLAYER_IMG_PATH[0], Rect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT)));
		animation = Animation::createWithSpriteFrames(frameVector);
		animation->setDelayPerUnit(1.0f);
		break;
	}
	case RUN:
	{
		// ����ܶ�����ռλ
		for (int i = 0; i < 4; i++)
		{
			auto spriteFrame = SpriteFrame::create(PLAYER_IMG_PATH[i], Rect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));
			frameVector.pushBack(spriteFrame);
		}
		animation = Animation::createWithSpriteFrames(frameVector);
		// ���ò��Ŷ�������֡���ʱ��
		animation->setDelayPerUnit(1.0f / 4.0f);
		break;
	}
	default:
	{
		break;
	}
	}

	// ��������
	if (animation != nullptr) {
		// ��ֹͣ����ִ�е����ж���
		center->stopAllActions();
		animate = Animate::create(animation);
		// �����µĶ���
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
	// �����λ�ò�ֵȻ���ټ�������
	// ��λ�����丽��ʱ���᲻���ƶ�,����true��ʾ����
	Vec2 delta = pos - center->getPosition();
	// ������һ������
	Vec2 dir = delta.getNormalized();
	// x�Ƿ��ڸ���
	if (delta.x - dir.x*speed.x<1 && delta.x - dir.x*speed.x>-1) {
		speed.x = 0;
	}
	else {
		speed.x *= dir.x;
	}
	// y�Ƿ��ڸ���
	if (delta.y - dir.y*speed.y<1 && delta.y - dir.y*speed.y>-1) {
		speed.y = 0;
	}
	else {
		speed.y *= dir.y;
	}
	// ���ڸ�������true
	if (speed.x == 0 && speed.y == 0) {
		return true;
	}
	//�����ƶ�
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

	// ���������ת����bug
	// ��ת��Ҫ��flip����Ϊflip�ķ�ת���޹�ê���
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
	//һ��body��CategoryBitmask����һ��body��ContactTestBitmask���߼���Ľ��������0ʱ���Ӵ��¼��������������򲻷��͡� 
	//һ��body��CategoryBitmask����һ��body��CollisionBitmask���߼�����������0ʱ�����ǽ���ײ��������ײ

	auto size = center->getContentSize();

	//����triggerX
	Node *tx = new Node();
	tx->setPosition(size.width / 2, size.height / 2);
	center->addChild(tx);
	triggerX = PhysicsBody::createEdgeBox(size);
	triggerX->setCategoryBitmask(0x00000001);
	triggerX->setContactTestBitmask(0x00000010);
	triggerX->setCollisionBitmask(0);
	triggerX->setTag(1);
	tx->setPhysicsBody(triggerX);

	//����triggerY
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
	//�����ײ���
	auto listenerX = EventListenerPhysicsContact::create();//������ײ����
	listenerX->onContactBegin = CC_CALLBACK_1(Player::onXCollisionBegin, this);//�ص�����
	listenerX->onContactSeparate = CC_CALLBACK_1(Player::onXCollisionSeperate, this);//�ص�����

	auto listenerY = EventListenerPhysicsContact::create();//������ײ����
	listenerY->onContactBegin = CC_CALLBACK_1(Player::onYCollisionBegin, this);//�ص�����
	listenerY->onContactSeparate = CC_CALLBACK_1(Player::onYCollisionSeperate, this);//�ص�����

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listenerX, triggerX->getNode()); //�����¼�����
	dispatcher->addEventListenerWithSceneGraphPriority(listenerY, triggerY->getNode()); //�����¼�����
}
