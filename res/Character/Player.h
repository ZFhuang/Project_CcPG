///��ҲٿصĽ�ɫ��

#pragma once
#include<string>
#include"cocos2d.h"

//�ٶȼ�ÿ��ı������
//ÿ��ͼ����32*32��
using namespace cocos2d;

static const int PLAYER_TAG = 110;
static int PLAYER_WIDTH = 30;
static int PLAYER_HEIGHT = 45;

// ��ɫ���X�ƶ��ٶ�
static const float  MAX_RUN = 220;
// ��ɫ���Y�ƶ��ٶ�
static const float  MAX_CLIMB = 110;
// ��ɫ����»��ٶ�
static const float	MAX_SLIP = 300;
// X��������ٶ�
static const float  MAX_X = 800;
// Y��������ٶ�
static const float  MAX_Y = 800;
// ��Ծ��ʼ�ٶ�
static const float  JUMPSPEED = 500;
// ���ץסǽ6s, ����3dt, ����2dt, ����1dt, ���ˢ��
static const float	TIMER_CLIMBING = 6;
// ��������������0.3s
static const float	TIMER_BACKJUMP = 0.3;
// ��̲�������0.3s
static const float	TIMER_DASH = 0.2;
// ��Ե��Ծ�ݴ�0.1s
static const float	TIMER_FALL = 0.1;
// ��ǰ��Ծ�ݴ�0.1s
static const float	TIMER_PREJUMP = 0.1;
// ������Ծ�ݴ�0.1s
static const float	TIMER_OUT = 0.1;
// ����̴���
static const int	DASH_TIMES = 1;



// ����֡����·������
static std::string PLAYER_IMG_PATH[4] = {
	"Character/player/run1.png",
	"Character/player/run2.png",
	"Character/player/run3.png",
	"Character/player/run4.png"
};

// ����״̬��
enum AniState {
	IDLE = 0,	// ��̬
	RUN = 1,	// �ܲ�
	JUMP = 2,	// ��Ծ��
	FALL = 3,	// �½���
	WALL = 4	// ��ǽ��
};

class Player
{
public:
	Player();
	~Player();
	// ��ʼ��
	void init(cocos2d::Vec2 pos);
	// ���ý�Ҫ���ŵĶ���״̬
	void setAnimation(AniState state);
	// ���ٶ���Ŀ�ĵ��ƶ�
	bool moveTo(cocos2d::Vec2 pos, cocos2d::Vec2 speed);
	// �ƶ�
	void toNewPos(cocos2d::Vec2 pos);
	// ���ش˾���
	cocos2d::Sprite* getSprite();
	// ʱ�ӵ���
	void update(float dt);
	// ����λ��
	Vec2 getPos();

	// ��Ծ
	void jump();
	// ���
	void dash(int dir);
	// ������Ծ
	void jumpend();
	// �ܲ�
	void run(int dir);
	// ��ǽ
	void climb(int dir);
	// ����
	void fall(float speed);
	// ײ��ͷ
	void headCol();
	// ��ǽ
	void slip(int wallDir);
	// ץǽ
	void hold(bool isHolding);
	// ���
	void ground(bool isGround);
	// �ƶ�����
	void slow(float speed);
	// ����
	//void wind(float speed);
	// ���ص�ǰ�ٶ�
	cocos2d::Vec2 getSpeed();

private:
	AniState nowAni = AniState::FALL;
	int life;
	cocos2d::Animate* animate = nullptr;
	cocos2d::Sprite* center = nullptr;

	// ��ǰ�ٶ�
	cocos2d::Vec2 Speed = cocos2d::Vec2(0, 0);
	// �Ƿ��ڵ���
	bool isGround = false;
	// �Ƿ�����Ծ
	bool isJumping = false;
	// �Ƿ�ǽ
	bool isSliping = false;
	// �Ƿ�ץǽ
	bool isHolding = false;
	// ǽ�ķ���
	int wallDir = 0;
	// �����·������ĳ���
	int backDir = 0;
	// �ɳ�̵Ĵ���
	int dashNum = 1;
	// ��ǰˮƽ����ķ���
	bool isRight = true;

	// ����֡�ļ��
	float dt=0;
	// �뿪�����ʱ������������Ե��Ծ�ݴ�
	float fallTimer = -1;
	// �뿪ǽ���ʱ�����������������ݴ�
	float outTimer = -1;
	// ������Ծ��ʱ���������������Ծ�ݴ�
	float prejumpTimer = -1;
	// ��������ʱ������һС��ʱ���ڲ��������ƶ�
	float backjumpTimer = -1;
	// ��̼�ʱ������һС��ʱ���ڲ��������ƶ�
	float dashTimer = -1;
	// ������ʱ����������Ϊ����
	float climbTimer = -1;

	// ����X�ٶ�
	void setSpeedX(float x);
	// ����X�ٶ�
	void addSpeedX(float x);
	// ����Y�ٶ�
	void setSpeedY(float y);
	// ����Y�ٶ�
	void addSpeedY(float y);
	// ��ʱ��ˢ��
	void timer();
};