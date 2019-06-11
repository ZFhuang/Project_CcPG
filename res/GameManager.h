///�������һЩ��Ϸ���е��жϺ�һЩ���ݵĹ���

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"

USING_NS_CC;

#define GM() GameManager::getInstance()
extern int nowSceneIdx;

class GameManager : public cocos2d::Ref
{
public:
	GameManager();
	virtual ~GameManager();
	virtual bool init();
	static GameManager* getInstance();
	//// ��ȡ��������ڵ�ͼ������
	//static cocos2d::Vec2 getMapPosition(cocos2d::TMXTiledMap* map, cocos2d::Node* node);
	//// ��ȡ����ָ��λ�õ�ͼ����
	//static int getTileID(cocos2d::TMXTiledMap* map, cocos2d::Node* node);

	void nextScene();
	void restartScene();
	void startScene(int sceneIndex);

private:
	static GameManager* m_gameManager;
};

#endif