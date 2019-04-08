///负责进行一些游戏固有的判断和一些数据的管理

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
	// 获取精灵相对于地图的坐标
	Vec2 getMapPosition(TMXTiledMap* map, Node* node);
	// 获取精灵指定位置的图块编号
	int getTileID(TMXTiledMap* map, Node* node);

private:
	static GameManager* m_gameManager;
};

#endif