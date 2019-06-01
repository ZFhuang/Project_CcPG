///��ҲٿصĽ�ɫ��

#pragma once

#include"proj.win32\res\GameManager.h"

using namespace std;

//�ٶȼ�ÿ��ı������
//ÿ��ͼ����32*32��

static const int PLAYER_TAG = 110;
static int PLAYER_WIDTH = 40;
static int PLAYER_HEIGHT = 50;

// ��ɫ���X�ƶ��ٶ�
static const float  MAX_RUN = 200;
// ��ɫ����»��ٶ�
static const float	MAX_SLIP = 70;
// X��������ٶ�
static const float  MAX_X = 1000;
// Y��������ٶ�
static const float  MAX_Y = 1000;
// Y������С�ٶ�
static const float  MIN_Y = -1000;
// ���ץסǽ3s
static const float	TIMER_HOLDING = 3;
// �����Ծ1s
static const float	TIMER_JUMPING = 1;



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
	void toNewPos(Vec2 pos);
	// ���ش˾���
	Sprite* getSpite();
	// ʱ�ӵ���
	void update(float dt);

	// �����ܲ��ȱ���ļ��ٶ�
	//void setAcceX(float x, bool isGround);
	// �����������ļ��ٶ�
	//void setAcceY(float y);
	// ������
	//void sysBackjump(float x);
	// ��Ծ
	void jump();
	// �ܲ�
	void run(int dir);
	// ����
	void fall(float speed);
	// ���
	void ground();
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
	//PhysicsBody *triggerX;
	//PhysicsBody *triggerY;
	// ��ǰ�ٶ�
	Vec2 Speed = Vec2(0, 0);

	// �Ƿ��ڵ���
	bool isGround = false;
	// �Ƿ�����Ծ
	bool isJumping = false;
	// �Ƿ�ǽ
	bool isSliping = false;

	// ��֡�ļ��
	float dt=0;
	// ���㰴����Ծ��õļ�ʱ
	float jumpStart = 0;
	// ���㰴�·�������õļ�ʱ
	float backjumpStart = 0;
	// �����뿪�����õļ�ʱ����������Ե��Ծ�ݴ�
	float fallStart = 0;
	// �����뿪ǽ���õļ�ʱ���������������ݴ�
	float outStart = 0;
	// ������Ծ��ʱ���������������Ծ�ݴ�
	float prejumpStart = 0;

	// ����X�ٶ�
	void setSpeedX(float x);
	// ����X�ٶ�
	void addSpeedX(float x);
	// ����Y�ٶ�
	void setSpeedY(float y);
	// ����Y�ٶ�
	void addSpeedY(float y);
};