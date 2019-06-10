#include "GameManager.h"
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\MainController.h"
#include "proj.win32\res\MainConfig.h"
#include "proj.win32\res\Snare\Needle.h"
#include "proj.win32\res\Scene\MainScene.h"

int nowSceneIdx = 0;
bool sceneIdxLock = true;

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

void GameManager::nextScene()
{
	if (sceneIdxLock) {
		nowSceneIdx++;
		startScene(nowSceneIdx);
		sceneIdxLock = false;
	}
}

void GameManager::restartScene()
{
	startScene(nowSceneIdx);
}

void GameManager::startScene(int sceneIndex)
{
	nowSceneIdx = sceneIndex;
	CCDirector::sharedDirector()->replaceScene(MainScene::createScene());
}
