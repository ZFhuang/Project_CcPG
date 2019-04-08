///�����õĳ�����

#pragma once

#include "cocos2d.h"
#include "proj.win32\res\MainConfig.h"
#include "proj.win32\res\Character/Player.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\MainController.h"
USING_NS_CC;

//��ͼtag,ͨ��tag�����ֶ���Ĳ�ͬ��ͼ
#define MAP_TAG 111
//��ͼ��xml�ļ�λ��
#define MAP_TMX_FILE_PATH "Map/testPlatform.tmx"


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

	// ���ص�ͼ
	void loadMap(std::string mapPath);
	// ���ؽ�ɫ����ֻ����ң�
	void loadCharacter();
};