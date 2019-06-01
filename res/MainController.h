///������ҿ��ƽ�������

#pragma once
#include "cocos2d.h"
#include "Character\Player.h"
USING_NS_CC;

//�ٶȼ�ÿ��ı������
//ÿ��ͼ����32*32��

static const string PLATFORM_LAYER = "Platform";
// ������������
static const float SCENE_Y = -20;
// ������������
static const float SCENE_Y = -20;
// �����������
static const float SCENE_X_GROUND = -40;
// ���к�������
static const float SCENE_X_AIR = -20;
// ��������ٶ�
static const float MAX_Y = 450;
// �ܲ��ٶ�
static const float RUNSPEED = 200;
// �������ļ��ٶ�
static const float CLIMBACCE = 2;
// ʹ���������ٶ�
static const float	ENERGYACCE = 1000/1;
// ��Ծ��ʱ��ms
static const clock_t JUMPTIME = 200;
// ��Ծ��ʼ�ٶ�
static const float JUMPSPEED = 200;
// ����»��ٶ�
static const float SLIPSPEED = -300;
// ��������ʱ��ms
static const clock_t BACKJUMPTIME = 200;
// �����ݴ�ʱ��ms
static const clock_t FAULT_FALLTIME = 40;
// ������Ծ�ݴ�ʱ��ms
static const clock_t FAULT_JUMPTIME = 40;
// ��Ծ����������ռ�ı���
static const float UPRATE = 0.8;
struct PlayerCol {
	Vec2 newPos;
	int xCol = 0;
	int yCol = 0;
};

class MainController
{
public:
	MainController();
	virtual ~MainController();
	// ������������Ҫ�����Ƶ����弰�����ƵĲ㴫��
	static MainController* getInstance(Player* in, cocos2d::Layer* layer, int maptag);
	// ��ʼ��
	void init();
	// �������ڲ��Ļص�
	void update(float dt);
	// ��������
	void addKeyListener();
	// ����������Ӧ
	void keyClick(EventKeyboard::KeyCode code);
	// ������ס��Ӧ,����ѭ����ÿ��һ��ʱ����һ�αȽϺ�
	void keyPress();
	// ��ס����ʱ��ϵͳ�׶�
	void sysCtrl();
	// ��������ı������ٶ�
	void environment();
	// �����ɿ���Ӧ
	void keyRelease(EventKeyboard::KeyCode code);
	// ״̬����
	void condition();
	// ����map
	std::map<EventKeyboard::KeyCode, bool> keymap;

private:
	// �����Ƶ�����ָ��
	Player* player;
	// �����ƵĲ�ָ��
	cocos2d::Layer* layer;
	// ��ײҪ�õ���tiledmap
	TMXTiledMap* map;
	// ����ָ��
	static MainController* controller;
	// ��ǰ֡���
	float dt;

	// ����ʵ�������ƶ��������า�ǵı���
	int clickDirX = 0;
	// ����ʵ�������ƶ��������า�ǵı���
	int clickDirY = 0;
	// ���ذ����ٶȿ����ƶ�����λ��
	PlayerCol getNewPos(Vec2 speed);

	// ��ס��ס����������ϵͳ�ƶ�����
	bool sysMove = false;
	// ǽ�ڷ���,0������ǽ
	int wallDir = 0;
	// �Ƿ�ץǽ
	bool isHold = false;


	// �Ƿ�������Y�ٶ�
	bool openY = true;
	// �Ƿ�������X�ٶ�
	bool openX = true;



};