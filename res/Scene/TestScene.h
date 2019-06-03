///�����õĳ�����

#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Player;
class GameManager;
class MainController;

//��ͼ��xml�ļ�λ��
static const std::string MAP_TMX_FILE_PATH = "Map/testPlatform.tmx";
// ����Ŵ���
static const float CAM_SCALE = 2;

class TestScene : public cocos2d::Layer
{
public:
	// ��ʼ��
	virtual bool init();
	// ��ѭ��
	void update(float dt);
	// ��������
	static cocos2d::Scene* createScene();
	// implement the "static create()" method manually
	CREATE_FUNC(TestScene);

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
	// �����
	Node* followPoint;
	// ������
	Vec2 birthPlace;
	// ��ǰ��ͼ
	TMXTiledMap *map;

	// ���ص�ͼ
	void loadMap(std::string mapPath);
	// ���ؽ�ɫ����ֻ����ң�
	void loadCharacter();
	// ���������
	void loadCamera();
	// ������ڵ�ͼ��Χ�ڸ����ɫ
	void cameraFollow();
};