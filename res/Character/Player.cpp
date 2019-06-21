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
		center = Sprite::create(PLAYER_IMG);
		// 强制设置大小
		center->setContentSize(Size(PLAYER_WIDTH, PLAYER_HEIGHT));
		// 设置tag
		center->setTag(PLAYER_TAG);

		// 注意要调好锚点方便接下来的旋转等操作
		center->setAnchorPoint(Vec2(0.5, 0.5));
		// 刷新大小
		PLAYER_WIDTH = center->getContentSize().width;
		PLAYER_HEIGHT = center->getContentSize().height;
		keyNum = 0;
		lastKey = nullptr;

		// 添加物理碰撞盒
		auto size = center->getContentSize();
		auto body = PhysicsBody::createBox(center->getContentSize());
		body->setGravityEnable(false);
		// 初始位置
		center->setPosition(pos);

		cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo(PLAYER_ANIFILE_INFO);
		m_armature = cocostudio::Armature::create("ProtagonistAnimation");
		//校准一下初始值
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

// 设置为新位置
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
	// 刷新动画
	setAnimation();
}

Vec2 Player::getPos()
{
	return center->getPosition();
}

// 进行跳跃
void Player::jump()
{
	if (isGround && !isJumping) {
		//普通跳
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
		//反身跳
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

// 八方向冲刺
void Player::dash(int dir)
{
	if (dashNum > 0 && (dashTimer<0 || dashTimer>TIMER_DASH / 2)) {
		switch (dir)
		{
		case 0:
			// dir 0 当前水平方向
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
				//要照顾根号2距离
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
			//要照顾根号2距离
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
			//要照顾根号2距离
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
			//要照顾根号2距离
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

// 结束下落
void Player::jumpend()
{
	if (isJumping) {
		//防止跳跃高度过低
		if (Speed.y > JUMPSPEED / 2) {
			setSpeedY(Speed.y / 1.5);
		}
		else if (Speed.y > 0) {
			//使下落自然
			setSpeedY(Speed.y / 2);
		}
		isJumping = false;
	}
}

// 跑动
void Player::run(int dir)
{
	//限制抓取和反身跳途中不可以移动
	if (!isHolding&&backjumpTimer < 0) {
		if (isGround || isSliping) {
			//乘三是为了加速快一点
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
			//空中加速慢一点
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

// 攀爬
void Player::climb(int dir)
{
	if (isHolding && (!isGround) && wallDir != 0) {
		if (climbTimer < -1) {
			climbTimer = 0;
		}
		if (climbTimer < TIMER_CLIMBING) {
			//由于是爬行所以无需阻力
			setSpeedY(dir*MAX_CLIMB);
			//上爬3dt, 不动2dt, 下爬1dt, 落地刷新
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
	// 非冲刺状态才能自由改变Speed
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
	// 非冲刺状态才能自由改变Speed
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

// 计时器刷新
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
			//使得停止不会太突然
			setSpeedX(Speed.x / 3);
			setSpeedY(Speed.y / 3);
		}
	}
}

// 下落
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

// 撞到头
void Player::headCol()
{
	//强制下落
	jumpend();
	setSpeedY(0);
}

// 下滑
void Player::slip(int wallDir)
{
	//下滑
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

// 抓墙
void Player::hold(bool isHolding)
{
	if (isSliping&&wallDir != 0 && (climbTimer < TIMER_CLIMBING)) {
		//可抓墙
		if ((!this->isHolding) && isHolding) {
			setSpeedY(0);
		}
		this->isHolding = isHolding;
		if (this->isHolding) {
			setSpeedX(wallDir * 70);
		}
	}
	else {
		//不可抓墙
		this->isHolding = false;
		this->isJumping = false;
	}
}

// 地面
void Player::ground(bool isGround)
{
	if (isGround) {
		//落地
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

// 缓速停止
void Player::slow(float speed)
{
	if (!isHolding) {
		//阻力不能影响到反向
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
			//空中阻力稍小一点
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

// 恢复冲刺次数
bool Player::getBattery()
{
	//这行也可以去掉，这样更有趣些，就可以做些类似空中攒能量的关卡
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
