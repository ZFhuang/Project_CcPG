///������ҿ��ƽ�������

#pragma once
#include "cocos2d.h"
USING_NS_CC;

class Player;

// Ħ����
static const float SCENE_X = -1000;

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
	// �������ڲ��Ļص�
	void update(float dt);
	// ��ʼ��
	void init();
	// ϵͳ������ʱ��,���ڵ���ʱ
	float sysTimer = 0;

private:
	// �����Ƶ�����ָ��
	Player* player;
	// �����ƵĲ�ָ��
	cocos2d::Layer* layer;
	// ��ײҪ�õ���tiledmap
	TMXTiledMap* map;
	// ����ָ��
	static MainController* controller;
	// ����map
	std::map<EventKeyboard::KeyCode, bool> keymap;
	// ��ǰ֡���
	float dt;

	// ����ʵ�������ƶ��������า�ǵı���
	int clickDirX = 0;
	// ����ʵ�������ƶ��������า�ǵı���
	int clickDirY = 0;
	// ���ذ����ٶȿ����ƶ�����λ��
	PlayerCol getNewPos(Vec2 speed);
	// ��ס����������ϵͳ����
	bool isSysMode = false;

	// �Ƿ�������Y�ٶ�
	bool openY = true;
	// �Ƿ�������X�ٶ�
	bool openX = true;
	// ����
	float SCENE_Y = -1000;

	// ��������
	void addKeyListener();
	// ����������Ӧ
	void keyClick(EventKeyboard::KeyCode code);
	// ������ס��Ӧ,����ѭ����ÿ��һ��ʱ����һ�αȽϺ�
	void keyPress();
	// ��ס����ʱ��ϵͳ�׶�
	void sysMode();
	// ��������ı������ٶ�
	void environment();
	// �����ɿ���Ӧ
	void keyRelease(EventKeyboard::KeyCode code);
	// ״̬����
	void condition();
	// ��ʱ��ˢ��
	void timer();
};