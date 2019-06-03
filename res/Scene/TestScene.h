///�����õĳ�����

#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Player;
class GameManager;
class MainController;

//��ͼ��xml�ļ�λ��
static const std::string MAP_TMX_FILE_PATH = "Map/testPlatform.tmx";

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
	// ��Ҷ���
	Player* player;
	// ��������ָ��
	cocos2d::PhysicsWorld* pw;
	Vec2 birthPlace;

	// ���ص�ͼ
	void loadMap(std::string mapPath);
	// ���ؽ�ɫ����ֻ����ң�
	void loadCharacter();
};