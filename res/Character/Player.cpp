#include"Player.h"
//#include "proj.win32\res\GameManager.h"
//#include "proj.win32\res\MainController.h"
//#include "proj.win32\res\Snare\Needle.h"

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

// ����Ϊ��λ��
void Player::toNewPos(Vec2 pos)
{
	center->setPosition(pos);
}

Sprite* Player::getSprite()
{
	return center;
}

void Player::update(float dt)
{
	this->dt = dt;
	timer();
	if (Speed.x > 0) {
		isRight = true;
	}
	else if (Speed.x < 0) {
		isRight = false;
	}
}

Vec2 Player::getPos()
{
	return center->getPosition();
}

// ������Ծ
void Player::jump()
{
	if (isGround && !isJumping) {
		//��ͨ��
		isJumping = true;
		isGround = false;
		setSpeedY(JUMPSPEED);
		prejumpTimer = -1;
	}
	else if (isSliping && !isJumping) {
		//������
		isJumping = true;
		isSliping = false;
		setSpeedY(JUMPSPEED*0.75);
		setSpeedX(-MAX_RUN*backDir);
		prejumpTimer = -1;
		backjumpTimer = 0;
	}
	else {
		if (prejumpTimer < 0)
			prejumpTimer = 0;
	}
}

// �˷�����
void Player::dash(int dir)
{
	if (dashNum > 0 && (dashTimer<0 || dashTimer>TIMER_DASH / 2)) {
		switch (dir)
		{
		case 0:
			// dir 0 ��ǰˮƽ����
			if (isRight) {
				// dir 3 D
				setSpeedX(MAX_X*0.8);
				setSpeedY(0);
				isJumping = true;
				isGround = false;
			}
			else
			{
				// dir 7 A
				//Ҫ�չ˸���2����
				setSpeedX(-MAX_X*0.8);
				setSpeedY(0);
				isJumping = true;
				isGround = false;
			}
			break;
		case 1:
			// dir 1 W
			setSpeedX(0);
			setSpeedY(MAX_Y*0.8);
			isJumping = true;
			isGround = false;
			break;
		case 2:
			// dir 2 WD
			//Ҫ�չ˸���2����
			setSpeedX(MAX_X*0.8 / 1.4);
			setSpeedY(MAX_Y*0.8 / 1.4);
			isJumping = true;
			isGround = false;
			break;
		case 3:
			// dir 3 D
			setSpeedX(MAX_X*0.8);
			setSpeedY(0);
			isJumping = true;
			isGround = false;
			break;
		case 4:
			// dir 4 DS
			setSpeedX(MAX_X*0.8 / 1.4);
			setSpeedY(-MAX_Y*0.8 / 1.4);
			isJumping = true;
			isGround = false;
			break;
		case 5:
			// dir 5 S
			setSpeedX(0);
			setSpeedY(-MAX_Y*0.8);
			isJumping = true;
			isGround = false;
			break;
		case 6:
			// dir 6 SA
			//Ҫ�չ˸���2����
			setSpeedX(-MAX_X*0.8 / 1.4);
			setSpeedY(-MAX_Y*0.8 / 1.4);
			isJumping = true;
			isGround = false;
			break;
		case 7:
			// dir 7 A
			setSpeedX(-MAX_X*0.8);
			setSpeedY(0);
			isJumping = true;
			isGround = false;
			break;
		case 8:
			// dir 8 AW
			//Ҫ�չ˸���2����
			setSpeedX(-MAX_X*0.8 / 1.4);
			setSpeedY(MAX_Y*0.8 / 1.4);
			isJumping = true;
			isGround = false;
			break;
		default:
			break;
		}
		dashTimer = 0;
		dashNum--;
	}
}

// ��������
void Player::jumpend()
{
	if (isJumping) {
		//��ֹ��Ծ�߶ȹ���
		if (Speed.y > JUMPSPEED / 2) {
			setSpeedY(Speed.y / 1.5);
		}
		else if (Speed.y > 0) {
			//ʹ������Ȼ
			setSpeedY(Speed.y / 2);
		}
		isJumping = false;
	}
}

// �ܶ�
void Player::run(int dir)
{
	//����ץȡ�ͷ�����;�в������ƶ�
	if (!isHolding&&backjumpTimer < 0) {
		if (isGround || isSliping) {
			//������Ϊ�˼��ٿ�һ��
			addSpeedX(dir*MAX_RUN * 8);
			if (Speed.x > MAX_RUN) {
				setSpeedX(MAX_RUN);
			}
			else if (Speed.x < -MAX_RUN) {
				setSpeedX(-MAX_RUN);
			}
		}
		else {
			//���м�����һ��
			addSpeedX(dir*MAX_RUN * 3);
			if (Speed.x > MAX_RUN) {
				setSpeedX(MAX_RUN);
			}
			else if (Speed.x < -MAX_RUN) {
				setSpeedX(-MAX_RUN);
			}
		}
	}
}

// ����
void Player::climb(int dir)
{
	if (isHolding && (!isGround) && wallDir != 0) {
		if (climbTimer < -1) {
			climbTimer = 0;
		}
		if (climbTimer < TIMER_CLIMBING) {
			//����������������������
			setSpeedY(dir*MAX_CLIMB);
			//����3dt, ����2dt, ����1dt, ���ˢ��
			climbTimer += dt*(dir + 2);
		}
		else {
			isHolding = false;
		}
	}
}

void Player::setSpeedX(float x)
{
	// �ǳ��״̬�������ɸı�Speed
	if (dashTimer < 0)
		Speed.x = x;
}

void Player::addSpeedX(float x)
{
	setSpeedX(Speed.x + x*dt);
	if (Speed.x > MAX_X) {
		setSpeedX(MAX_X);
	}
	if (Speed.x < -MAX_X) {
		setSpeedX(-MAX_X);
	}
}

void Player::setSpeedY(float y)
{
	// �ǳ��״̬�������ɸı�Speed
	if (dashTimer < 0)
		Speed.y = y;
}

void Player::addSpeedY(float y)
{
	setSpeedY(Speed.y + y*dt);
	if (Speed.y > MAX_Y) {
		setSpeedY(MAX_Y);
	}
	if (Speed.y < -MAX_Y) {
		setSpeedY(-MAX_Y);
	}
}

// ��ʱ��ˢ��
void Player::timer()
{
	if (fallTimer >= 0) {
		fallTimer += dt;
		if (fallTimer >= TIMER_FALL) {
			isGround = false;
			fallTimer = -1;
		}
	}
	if (prejumpTimer >= 0) {
		prejumpTimer += dt;
		if (prejumpTimer >= TIMER_PREJUMP) {
			prejumpTimer = -1;
		}
		else {
			jump();
		}
	}
	if (outTimer >= 0) {
		outTimer += dt;
		if (outTimer >= TIMER_OUT) {
			isSliping = false;
			outTimer = -1;
		}
	}
	if (backjumpTimer >= 0) {
		backjumpTimer += dt;
		if (backjumpTimer >= TIMER_BACKJUMP) {
			backjumpTimer = -1;
		}
	}
	if (dashTimer >= 0) {
		dashTimer += dt;
		if (dashTimer >= TIMER_DASH) {
			dashTimer = -1;
			//ʹ��ֹͣ����̫ͻȻ
			setSpeedX(Speed.x / 3);
			setSpeedY(Speed.y / 3);
		}
	}
}

// ����
void Player::fall(float speed)
{
	if (!isHolding) {
		if (isGround&&fallTimer < 0) {
			fallTimer = 0;
		}
		if (isSliping) {
			addSpeedY(speed / 2);
			if (Speed.y > 0) {
				setSpeedY(Speed.y / 2);
			}
			if (Speed.y < -MAX_SLIP)
				setSpeedY(-MAX_SLIP);
		}
		else {
			addSpeedY(speed);
		}
	}
}

// ײ��ͷ
void Player::headCol()
{
	//ǿ������
	jumpend();
	setSpeedY(0);
}

// �»�
void Player::slip(int wallDir)
{
	//�»�
	if (this->wallDir != 0)
		backDir = this->wallDir;
	this->wallDir = wallDir;
	if (this->wallDir != 0) {
		isSliping = true;
		outTimer = -1;
	}
	else {
		if (isSliping == true && outTimer < 0)
			outTimer = 0;
	}
}

// ץǽ
void Player::hold(bool isHolding)
{
	if (isSliping&&wallDir != 0 && (climbTimer < TIMER_CLIMBING)) {
		//��ץǽ
		if ((!this->isHolding) && isHolding) {
			setSpeedY(0);
		}
		this->isHolding = isHolding;
		if (this->isHolding) {
			setSpeedX(wallDir * 70);
		}
	}
	else {
		//����ץǽ
		this->isHolding = false;
		this->isJumping = false;
	}
}

// ����
void Player::ground(bool isGround)
{
	if (isGround) {
		//���
		this->isGround = true;
		isJumping = false;
		isSliping = false;
		isHolding = false;
		fallTimer = -1;
		climbTimer = -1;
		dashNum = DASH_TIMES;
		setSpeedY(0);
	}
	else {
		if (fallTimer < 0) {
			this->isGround = false;
		}
	}
}

// ����ֹͣ
void Player::slow(float speed)
{
	if (!isHolding) {
		//��������Ӱ�쵽����
		if (isGround) {
			if (Speed.x > 0) {
				addSpeedX(speed);
				if (Speed.x < 0) {
					setSpeedX(0);
				}
			}
			else if (Speed.x < 0) {
				addSpeedX(-speed);
				if (Speed.x > 0) {
					setSpeedX(0);
				}
			}
		}
		else {
			//����������Сһ��
			if (Speed.x > 0) {
				addSpeedX(speed / 2);
				if (Speed.x < 0) {
					setSpeedX(0);
				}
			}
			else if (Speed.x < 0) {
				addSpeedX(-speed / 2);
				if (Speed.x > 0) {
					setSpeedX(0);
				}
			}
		}
	}
}

Vec2 Player::getSpeed()
{
	return Speed;
}
