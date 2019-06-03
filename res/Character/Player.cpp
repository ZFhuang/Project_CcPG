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
	}
	else if (isSliping && !isJumping) {
		//反身跳
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
			}
			else
			{
				// dir 7 A
				//要照顾根号2距离
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
			//要照顾根号2距离
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
			//要照顾根号2距离
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
			//要照顾根号2距离
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

Vec2 Player::getSpeed()
{
	return Speed;
}
