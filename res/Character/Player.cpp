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
		center = Sprite::create();
		center->setTag(PLAYER_TAG);
		// ע��Ҫ����ê�㷽�����������ת�Ȳ���
		center->setAnchorPoint(Vec2(0.2, 0.5));
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

void Player::move(Vec2 speed)
{
	center->setPosition(center->getPosition() + speed);
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

void Player::setDir(Dir dir)
{
	if (this->dir == dir) {
		return;
	}
	this->dir = dir;
	// ��ת��Ҫ��flip����Ϊflip�ķ�ת���޹�ê���
	if (this->dir == Dir::LEFT) {
		center->setRotationY(180);
	}
	else {
		center->setRotationY(0);
	}
}

Sprite* Player::getPlayer()
{
	return center;
}
