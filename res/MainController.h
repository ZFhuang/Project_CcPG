///������ҿ��ƽ�������

#pragma once
#include "cocos2d.h"
#include "Character\Player.h"
USING_NS_CC;

static const string PLATFORM_LAYER = "Platform";
// �����ƶ��ļ��ٶ�
static const float RUNACCE = 4;
// ��Ծ��ʱ��ms
static const clock_t JUMPTIME = 400;
// �����ݴ�ʱ��ms
static const clock_t FAULT_FALLTIME = 40;
// ������Ծ�ݴ�ʱ��ms
static const clock_t FAULT_JUMPTIME = 40;
// ��Ծ����������ռ�ı���
static const float UPRATE = 0.7;
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
	static MainController* getInstance(Player* in,cocos2d::Layer* layer,int maptag);
	// ��ʼ��
	void init();
	// �������ڲ��Ļص�
	void update();
	// ��������
	void addKeyListener();
	// ����������Ӧ
	void keyClick(EventKeyboard::KeyCode code);
	// ������ס��Ӧ,����ѭ����ÿ��һ��ʱ����һ�αȽϺ�
	void keyPress();
	// �����ɿ���Ӧ
	void keyRelease(EventKeyboard::KeyCode code);
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
	//����ʵ�������ƶ��������า�ǵı���
	int clickDir = 0;
	// ���ذ����ٶȿ����ƶ�����λ��
	PlayerCol getNewPos(Vec2 speed);
	// �Ƿ��ڵ���
	bool isGround = false;
	// ��ס��ס��������һЩ�̶��ƶ�����
	bool lockPress = false;
	// ���㰴����Ծ��õļ�ʱ
	clock_t jumpStart = 0;
	// �����뿪�����õļ�ʱ����������Ծ�ݴ�
	clock_t fallStart = 0;
	// ������Ծ��ʱ��
	clock_t prejumpStart = 0;
};