///��ҲٿصĽ�ɫ��

#pragma once

#include"proj.win32\res\GameManager.h"

using namespace std;

static const int PLAYER_TAG = 110;
static int  PLAYER_WIDTH = 40;
static int  PLAYER_HEIGHT = 50;
static const float JUMP_ACCE = 1.2;
static const float FALL_ACCE = 1.2;
static const float  MAX_PLAYER_SPEED_X = 10;
static const float  MAX_SPEED_JUMP = 7;
static const float  MAX_SPEED_FALL = 7;
// ֹͣ�Ĺ���
static const float  SLOW_DOWN_X = 5;

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
	// ���õ�ǰ����ķ���
	void setDir(Dir dir);
	// ���ش˾���
	Sprite* getSpite();
	// ���ü��ٶ�
	void setAcceX(float x);
	// ���ÿ����ٶȿ���
	void air(int step);
	// �����ٶ�
	Vec2 getSpeed();

private:
	AniState nowAni = AniState::FALL;
	int life;
	int energy;
	bool isGround = false;;
	Dir dir = Dir::STOP;
	Animate* animate = nullptr;
	Sprite* center = nullptr;
	PhysicsBody *triggerX;
	PhysicsBody *triggerY;

	// ��ǰ�ٶ�
	Vec2 Speed = Vec2(0, 0);
};