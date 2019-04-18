///��ҲٿصĽ�ɫ��

#pragma once

#include"proj.win32\res\GameManager.h"

using namespace std;

static const int PLAYER_TAG = 110;
static int  PLAYER_WIDTH = 35;
static int  PLAYER_HEIGHT = 60;
static const int  MAX_PLAYER_SPEED_X = 10;
static const int  MAX_PLAYER_SPEED_JUMP = 20;
static const int  MAX_PLAYER_SPEED_FALL = 30;

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
	// �����ٶȷ����ƶ�
	void moveX(double speed);
	// ���ٶ���Ŀ�ĵ��ƶ�
	bool moveTo(Vec2 pos, Vec2 speed);
	// ���õ�ǰ����ķ���
	void setDir(Dir dir);
	// ���ش˾���
	Sprite* getSpite();
	

private:
	AniState nowAni = AniState::FALL;
	int life;
	int energy;
	bool isGround = false;;
	Dir dir = Dir::RIGHT;
	Animate* animate = nullptr;
	Sprite* center = nullptr;
	PhysicsBody *triggerX;
	PhysicsBody *triggerY;
	bool leftCol = false;
	bool rightCol = false;
	bool upCol = false;
	bool downCol = false;
	// ��ǰ�ٶ�
	Vec2 Speed = Vec2(0, 0);

	// ˢ���ƶ���ײ��
	void refreshTrigger();
	// ��ʼ����ײ������
	void initTrigger();
};