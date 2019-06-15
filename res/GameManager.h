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

	void nextScene();
	void restartScene();
	void startScene(int sceneIndex);

private:
	static GameManager* m_gameManager;
};

#endif