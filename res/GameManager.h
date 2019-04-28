///�������һЩ��Ϸ���е��жϺ�һЩ���ݵĹ���

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

#define GM() GameManager::getInstance()

class GameManager : public Ref
{
public:
	GameManager();
	virtual ~GameManager();
	virtual bool init();
	static GameManager* getInstance();
	// ��ȡ��������ڵ�ͼ������
	static Vec2 getMapPosition(TMXTiledMap* map, Node* node);
	// ��ȡ����ָ��λ�õ�ͼ����
	static int getTileID(TMXTiledMap* map, Node* node);

private:
	static GameManager* m_gameManager;
};

#endif