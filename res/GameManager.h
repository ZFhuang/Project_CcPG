///负责进行一些游戏固有的判断和一些数据的管理

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
	//// 获取精灵相对于地图的坐标
	//static cocos2d::Vec2 getMapPosition(cocos2d::TMXTiledMap* map, cocos2d::Node* node);
	//// 获取精灵指定位置的图块编号
	//static int getTileID(cocos2d::TMXTiledMap* map, cocos2d::Node* node);

	void nextScene();
	void restartScene();
	void startScene(int sceneIndex);

private:
	static GameManager* m_gameManager;
};

#endif