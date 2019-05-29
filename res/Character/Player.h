///��ҲٿصĽ�ɫ��

#pragma once

#include"proj.win32\res\GameManager.h"

using namespace std;

//�ٶȼ�ÿ��ı������
//ÿ��ͼ����32*32��

static const int	PLAYER_TAG = 110;
static int  PLAYER_WIDTH = 40;
static int  PLAYER_HEIGHT = 50;
static const float	JUMP_ACCE = 1.2;
static const float	FALL_ACCE = 1.2;
static const float	SLIP_ACCE = 0.5;
static const float  MAX_PLAYER_SPEED_X = 200;
static const float	MAX_SPEED_SLIP = 50;
static const float	MAX_ENERGY = 3000;	// ���ץסǽ3s

// ����ֹͣ�Ĺ���
static const float  SLOW_DOWN_X = 400;
// ����ֹͣ�Ĺ���
static const float  SLOW_DOWN_AIR = 200;

// ����֡����·������
static string PLAYER_IMG_PATH[4] = {
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
	void init(Vec2 pos);
	// ���ý�Ҫ���ŵĶ���״̬
	void setAnimation(AniState state);
	// ���ٶ���Ŀ�ĵ��ƶ�
	bool moveTo(Vec2 pos, Vec2 speed);
	// �ƶ�
	void move(Vec2 pos);
	// ���ش˾���
	Sprite* getSpite();
	// �����ܲ��ȱ���ļ��ٶ�
	void setAcceX(float x, bool isGround);
	// �����������ļ��ٶ�
	void setAcceY(float y);
	// ������
	void sysBackjump(float x);
	// ����Y�ٶ�
	void setSpeedY(float y);
	// ����Y�ٶ�
	void addSpeedY(float y);
	// ���ص�ǰ�ٶ�
	Vec2 getSpeed();
	// ��������
	bool useEnergy(float used);
	// ��ǰˮƽ����ķ���
	bool isRight = false;

private:
	AniState nowAni = AniState::FALL;
	int life;
	clock_t energy = 0;
	Animate* animate = nullptr;
	Sprite* center = nullptr;
	PhysicsBody *triggerX;
	PhysicsBody *triggerY;

	// ��ǰ�ٶ�
	Vec2 Speed = Vec2(0, 0);
};