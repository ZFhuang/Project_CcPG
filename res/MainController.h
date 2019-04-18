///������ҿ��ƽ�������

#pragma once
#include "cocos2d.h"
#include "Character\Player.h"
USING_NS_CC;

static const string PLATFORM_LAYER = "Platform";
static const float RUNACCE = 2;

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
	// ��ײ����
	void addCollideListener();
	// ����������Ӧ
	void keyClick(EventKeyboard::KeyCode code);
	// ������ס��Ӧ,����ѭ����ÿ��һ��ʱ����һ�αȽϺ�
	void keyPress();
	// �����ɿ���Ӧ
	void keyRelease(EventKeyboard::KeyCode code);
	// ����map
	std::map<EventKeyboard::KeyCode, bool> keymap;
	// ������ײ
	bool onContactBegin(const PhysicsContact &contact);

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
	// �����ƶ��ļ��ٶ�
	int acceSpeed = RUNACCE;
};