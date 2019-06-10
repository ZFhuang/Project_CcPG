///�����õĳ�����

#pragma once

#include "cocos2d.h"
#include<string>
USING_NS_CC;

class Player;
class GameManager;
class MainController;

// 1080Pʱ������Ŵ��ʣ������ֱ��ʰ��ձ�������,������16��9
static const float CAM_SCALE = 2;

class MainScene : public cocos2d::Layer
{
public:
	// ��ʼ��
	virtual bool init();
	// ��ѭ��
	void update(float dt);
	// ��������
	static cocos2d::Scene* createScene();
	// implement the "static create()" method manually
	CREATE_FUNC(MainScene);

private:
	// �ܿض���
	GameManager* gameManager;
	// ��������
	MainController* controller;
	// �����
	Camera* camera;
	// ����ƶ���Χ
	Rect* cameraRange;
	// ��Ҷ���
	Player* player;
	// ������
	Vec2 birthPlace;
	// ��ǰ��ͼ
	TMXTiledMap *map;

	// ѡ���ͼ
	TMXTiledMap* selectMap();
	// ��ʼ����ͼ����
	void initMap();
	// ��ȡ��ͼ
	void loadMap();
	// ���ؽ�ɫ����ֻ����ң�
	void loadCharacter();
	// ���������
	void loadCamera();
	// ������ڵ�ͼ��Χ�ڸ����ɫ
	void cameraFollow();
};