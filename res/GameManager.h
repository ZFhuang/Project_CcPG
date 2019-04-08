///�������һЩ��Ϸ���е��жϺ�һЩ���ݵĹ���

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

#define GM() GameManager::getInstance()

enum Dir
{
	STOP = 0,
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	RIGHT = 4
};

class GameManager : public Ref
{
public:
	GameManager();
	virtual ~GameManager();
	virtual bool init();
	static GameManager* getInstance();
	// ��ȡ��������ڵ�ͼ������
	Vec2 getMapPosition(TMXTiledMap* map, Node* node);
	// ��ȡ����ָ��λ�õ�ͼ����
	int getTileID(TMXTiledMap* map, Node* node);

private:
	static GameManager* m_gameManager;
};

#endif