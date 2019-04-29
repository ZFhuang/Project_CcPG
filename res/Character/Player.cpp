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
		center->setContentSize(Size(PLAYER_WIDTH, PLAYER_HEIGHT));
		// ����tag
		center->setTag(PLAYER_TAG);

		// ע��Ҫ����ê�㷽�����������ת�Ȳ���
		center->setAnchorPoint(Vec2(0.5, 0.5));
		// ˢ�´�С
		PLAYER_WIDTH = center->getContentSize().width;
		PLAYER_HEIGHT = center->getContentSize().height;

		// ���������ײ��
		auto size = center->getContentSize();
		auto body = PhysicsBody::createBox(center->getContentSize());
		body->setGravityEnable(false);
		//center->setPhysicsBody(body);
		// ��ʼλ��
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

Sprite* Player::getSpite()
{
	return center;
}

void Player::setAcceX(float x, bool isGround)
{
	//X���ƶ�
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
		//�������ã�������״̬
		if (Speed.x > 0) {
			if (isGround) {
				//����
				if (Speed.x < SLOW_DOWN_X) {
					Speed.x = 0;
				}
				Speed.x -= SLOW_DOWN_X;
			}
			else {
				//���й��Ը����
				if (Speed.x < SLOW_DOWN_AIR) {
					Speed.x = 0;
				}
				Speed.x -= SLOW_DOWN_AIR;
			}
		}
		else if (Speed.x < 0) {
			//�Գ�
			if (isGround) {
				if (Speed.x > -SLOW_DOWN_X) {
					Speed.x = 0;
				}
				Speed.x += SLOW_DOWN_X;
			}
			else {
				if (Speed.x > -SLOW_DOWN_AIR) {
					Speed.x = 0;
				}
				Speed.x += SLOW_DOWN_AIR;
			}
		}
		//else
			//dir = Dir::STOP;
	}
}

void Player::setAcceY(float y)
{
	//Y���ƶ�,���������ֲ�����û�й��Ե�
	if (y > 0) {
		if (Speed.y + y < MAX_PLAYER_SPEED_Y) {
			Speed.y += y;
		}
		else {
			Speed.y = MAX_PLAYER_SPEED_Y;
		}
	}
	else if (y < 0) {
		if (Speed.y + y > -MAX_PLAYER_SPEED_Y) {
			Speed.y += y;
		}
		else {
			Speed.y = -MAX_PLAYER_SPEED_Y;
		}
	}
	else {
		Speed.y = 0;
	}
}

void Player::sysBackjump(float x)
{
	//X���ƶ�
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
}

void Player::setAir(int step)
{
	switch (step)
	{
	case 1:
		// һ�׶Σ�����
		if (Speed.y + JUMP_ACCE > MAX_SPEED_JUMP) {
			Speed.y += JUMP_ACCE;
		}
		else {
			Speed.y = MAX_SPEED_JUMP;
		}
		break;
	case 3:
		// ���׶Σ�������������һ��
		if (Speed.y > -4) {
			Speed.y -= 0.5;
		}
		else if (Speed.y < -4) {
			Speed.y = 0;
		}
		break;
	case 4:
		// ץǽһ�׶Σ������½�
		Speed.y = 0;
		break;
	case 5:
		// ��׶Σ�����
		if (Speed.y - FALL_ACCE > -MAX_SPEED_FALL) {
			Speed.y -= FALL_ACCE;
		}
		else {
			Speed.y = -MAX_SPEED_FALL;
		}
		break;
	case 6:
		// ��ǽ��ץǽ���׶Σ������½�
		if (Speed.y - SLIP_ACCE > -MAX_SPEED_SLIP) {
			Speed.y -= SLIP_ACCE;
		}
		else {
			Speed.y = -MAX_SPEED_SLIP;
		}
		break;
	case 7:
		// �߽׶Σ����
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

bool Player::useEnergy(float used)
{
	if (used == -1) {
		// ���ʱ�����ָ�
		energy = MAX_ENERGY;
		return true;
	}
	else {
		energy -= used;
	}
	if (energy <= 0) {
		// �����ľ�
		return false;
	}
	else {
		return true;
	}
}
