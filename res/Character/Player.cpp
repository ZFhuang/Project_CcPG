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
		center = Sprite::create(PLAYER_IMG);
		// ǿ�����ô�С
		center->setContentSize(Size(PLAYER_WIDTH, PLAYER_HEIGHT));
		// ����tag
		center->setTag(PLAYER_TAG);

		// ע��Ҫ����ê�㷽�����������ת�Ȳ���
		center->setAnchorPoint(Vec2(0.5, 0.5));
		// ˢ�´�С
		PLAYER_WIDTH = center->getContentSize().width;
		PLAYER_HEIGHT = center->getContentSize().height;
		keyNum = 0;
		lastKey = nullptr;

		// ���������ײ��
		auto size = center->getContentSize();
		auto body = PhysicsBody::createBox(center->getContentSize());
		body->setGravityEnable(false);
		// ��ʼλ��
		center->setPosition(pos);

		cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo(PLAYER_ANIFILE_INFO);
		m_armature = cocostudio::Armature::create("ProtagonistAnimation");
		//У׼һ�³�ʼֵ
		m_armature->setPosition(Vec2(PLAYER_WIDTH/2, PLAYER_HEIGHT/2));
		m_armature->setScaleX(SCALEX);
		m_armature->setScaleY(SCALEY);
		center->addChild(m_armature);
		m_armature->getAnimation()->play("jump");
		nowAni = JUMP;
	}
}

void Player::setAnimation()
{
	if (dashTimer == -1) {
		if (isGround) {
			if (Speed.x == 0 && nowAni != IDLE) {
				m_armature->getAnimation()->play("common");
				nowAni = IDLE;
			}
		}
	}
	if (!isSliping && !isHolding) {
		if (Speed.x > 0) {
			if (m_armature->getScaleX() != 1 * SCALEX)
			{
				m_armature->setScaleX(1 * SCALEX);
			}
		}
		else if (Speed.x < 0) {
			if (m_armature->getScaleX() != -1 * SCALEX)
			{
				m_armature->setScaleX(-1 * SCALEX);
			}
		}
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
	// ˢ�¶���
	setAnimation();
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
		if (nowAni != JUMP) {
			m_armature->getAnimation()->play("jump");
			nowAni = JUMP;
		}
	}
	else if (isSliping && !isJumping) {
		//������
		isJumping = true;
		isSliping = false;
		setSpeedY(JUMPSPEED*0.75);
		setSpeedX(-MAX_RUN*backDir);
		prejumpTimer = -1;
		backjumpTimer = 0;
		if (nowAni != JUMP) {
			m_armature->getAnimation()->play("jump");
			nowAni = JUMP;
		}
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
				if (nowAni != DASH) {
					m_armature->getAnimation()->play("rightRush");
					nowAni = DASH;
				}
			}
			else
			{
				// dir 7 A
				//Ҫ�չ˸���2����
				setSpeedX(-MAX_X*0.8);
				setSpeedY(0);
				isJumping = true;
				isGround = false;
				if (nowAni != DASH) {
					m_armature->getAnimation()->play("rightRush");
					nowAni = DASH;
				}
			}
			break;
		case 1:
			// dir 1 W
			setSpeedX(0);
			setSpeedY(MAX_Y*0.8);
			isJumping = true;
			isGround = false;
			if (nowAni != DASH) {
				m_armature->getAnimation()->play("topRush");
				nowAni = DASH;
			}
			break;
		case 2:
			// dir 2 WD
			//Ҫ�չ˸���2����
			setSpeedX(MAX_X*0.8 / 1.4);
			setSpeedY(MAX_Y*0.8 / 1.4);
			isJumping = true;
			isGround = false;
			if (nowAni != DASH) {
				m_armature->getAnimation()->play("top-rightRush");
				nowAni = DASH;
			}
			break;
		case 3:
			// dir 3 D
			setSpeedX(MAX_X*0.8);
			setSpeedY(0);
			isJumping = true;
			isGround = false;
			if (nowAni != DASH) {
				m_armature->getAnimation()->play("rightRush");
				nowAni = DASH;
			}
			break;
		case 4:
			// dir 4 DS
			setSpeedX(MAX_X*0.8 / 1.4);
			setSpeedY(-MAX_Y*0.8 / 1.4);
			isJumping = true;
			isGround = false;
			if (nowAni != DASH) {
				m_armature->getAnimation()->play("bottom-rightRush");
				nowAni = DASH;
			}
			break;
		case 5:
			// dir 5 S
			setSpeedX(0);
			setSpeedY(-MAX_Y*0.8);
			isJumping = true;
			isGround = false;
			if (nowAni != DASH) {
				m_armature->getAnimation()->play("bottomRush");
				nowAni = DASH;
			}
			break;
		case 6:
			// dir 6 SA
			//Ҫ�չ˸���2����
			setSpeedX(-MAX_X*0.8 / 1.4);
			setSpeedY(-MAX_Y*0.8 / 1.4);
			isJumping = true;
			isGround = false;
			if (nowAni != DASH) {
				m_armature->getAnimation()->play("bottom-rightRush");
				nowAni = DASH;
			}
			break;
		case 7:
			// dir 7 A
			setSpeedX(-MAX_X*0.8);
			setSpeedY(0);
			isJumping = true;
			isGround = false;
			if (nowAni != DASH) {
				m_armature->getAnimation()->play("rightRush");
				nowAni = DASH;
			}
			break;
		case 8:
			// dir 8 AW
			//Ҫ�չ˸���2����
			setSpeedX(-MAX_X*0.8 / 1.4);
			setSpeedY(MAX_Y*0.8 / 1.4);
			isJumping = true;
			isGround = false;
			if (nowAni != DASH) {
				m_armature->getAnimation()->play("top-rightRush");
				nowAni = DASH;
			}
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
			if (isGround) {
				if (Speed.x != 0 && nowAni != RUN) {
					m_armature->getAnimation()->play("run");
					nowAni = RUN;
				}
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
			if (dir == 0) {
				if (nowAni == CLIMB) {
					m_armature->getAnimation()->pause();
					m_armature->setScaleX(wallDir*SCALEX);
					nowAni = CLIMB;
				}
			}
			else {
				if (nowAni != CLIMB) {
					m_armature->getAnimation()->play("climb");
					nowAni = CLIMB;
					m_armature->setScaleX(wallDir*SCALEX);
				}
				else {
					m_armature->getAnimation()->resume();
					m_armature->setScaleX(wallDir*SCALEX);
				}
			}
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
		if (!isHolding&&nowAni != DASH&&nowAni != CLIMB) {
			m_armature->getAnimation()->play("rightRush");
			nowAni = DASH;
			m_armature->setScaleX(-wallDir*SCALEX);
		}
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
			if (!isHolding &&!isSliping&& nowAni != JUMP) {
				m_armature->getAnimation()->play("jump");
				nowAni = JUMP;
			}
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

void Player::initKeyNum(int num)
{
	keyNum = num;
}

Sprite * Player::getLastKey(Sprite * newKey)
{
	keyNum -= 1;
	if (lastKey == nullptr) {
		lastKey = newKey;
		return center;
	}
	else {
		Sprite * temp = lastKey;
		lastKey = newKey;
		return temp;
	}
}

int Player::getKeyNum()
{
	return keyNum;
}

// �ָ���̴���
bool Player::getBattery()
{
	//����Ҳ����ȥ������������ȤЩ���Ϳ�����Щ���ƿ����������Ĺؿ�
	if (dashNum < DASH_TIMES) {
		dashNum++;
		return true;
	}
	else {
		return false;
	}
}

Vec2 Player::getSpeed()
{
	return Speed;
}
