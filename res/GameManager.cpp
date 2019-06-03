#include "GameManager.h"
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\MainController.h"
#include "proj.win32\res\MainConfig.h"
#include "proj.win32\res\Snare\Needle.h"
#include "proj.win32\res\Scene\TestScene.h"

GameManager::GameManager()
{

}

GameManager::~GameManager()
{
	m_gameManager = NULL;
}

bool GameManager::init()
{
	return true;
}

GameManager* GameManager::m_gameManager = NULL;
GameManager* GameManager::getInstance()
{
	if (m_gameManager == NULL)
	{
		m_gameManager = new GameManager();
		m_gameManager->init();
	}
	return m_gameManager;
}

//Vec2 GameManager::getMapPosition(TMXTiledMap* map, Node* node)
//{
//	// 玩家相对于屏幕的坐标
//	float px = node->getPositionX();
//	float py = node->getPositionY() - node->getContentSize().height / 2;
//
//	// 地图相对于屏幕的x坐标
//	float mx = abs(map->getPositionX());
//	float my = 0;
//
//	// 玩家相对于地图的坐标
//	return Vec2(px + mx, py + my);
//}
//
//int GameManager::getTileID(TMXTiledMap* map, Node* node)
//{
//	// 获得图块的ID
//	Vec2 current = getMapPosition(map, node);
//	return map->getLayer(BACKGROUND_LAYER)->getTileGIDAt(Vec2((int)(current.x / map->getTileSize().width), (int)(map->getMapSize().height - 1 - current.y / map->getTileSize().height)));
//}

void GameManager::nowScene(int tag)
{
	if (nowSceneTag != tag) {
		nowSceneTag = tag;
	}
}

void GameManager::restartScene()
{
	startScene(nowSceneTag);
}

void GameManager::startScene(int sceneIndex)
{
	switch (sceneIndex)
	{
	case TEST_SCENE:
		nowScene(sceneIndex);
		CCDirector::sharedDirector()->replaceScene(TestScene::createScene());
		break;
	default:
		break;
	}
}
