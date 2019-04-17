///��ҲٿصĽ�ɫ��

#pragma once

#include"proj.win32\res\GameManager.h"

using namespace std;

static const int PLAYER_TAG = 110;
static const int  PLAYER_WIDTH = 100;
static const int  PLAYER_HEIGHT = 100;
static const int  PLAYER_SPEEDX = 3;
static const int  PLAYER_SPEEDY = 3;

// ����֡����·������
static string PLAYER_IMG_PATH[4] = {
	"Character/player/player1.png",
	"Character/player/player2.png",
	"Character/player/player3.png",
	"Character/player/player4.png"
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
	void move(Vec2 speed);
	// ���ٶ���Ŀ�ĵ��ƶ�
	bool moveTo(Vec2 pos, Vec2 speed);
	// ���õ�ǰ����ķ���
	void setDir(Dir dir);
	// ���ش˾���
	Sprite* getSpite();
	// �����ٶ�
	Vec2 runSpeed = Vec2(PLAYER_SPEEDX, PLAYER_SPEEDY);

private:
	AniState nowAni = AniState::FALL;
	int life;
	int energy;
	bool isGround = false;;
	Dir dir = Dir::RIGHT;
	Animate* animate = nullptr;
	Sprite* center = nullptr;
};