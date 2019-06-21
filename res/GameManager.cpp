#include "GameManager.h"
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\MainController.h"
#include "proj.win32\res\MainConfig.h"
#include "proj.win32\res\Snare\Needle.h"
#include "proj.win32\res\Scene\MainScene.h"
#include "SimpleAudioEngine.h"

int nowSceneIdx = 0;
int nowStoryIdx = 0;
CocosDenshion::SimpleAudioEngine *audio = CocosDenshion::SimpleAudioEngine::getInstance();

GameManager::GameManager()
{

}

GameManager::~GameManager()
{
	m_gameManager = NULL;
}

bool GameManager::init()
{
	if (!audio->isBackgroundMusicPlaying()) {
		audio->playBackgroundMusic(MUSIC.c_str(),true);
		audio->setBackgroundMusicVolume(0.3);
	}
	
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
	nowSceneIdx++;
	startScene(nowSceneIdx);
}

void GameManager::restartScene()
{
	startScene(nowSceneIdx);
}

void GameManager::startScene(int sceneIndex)
{
	nowSceneIdx = sceneIndex;
	CCLOG("GM: %d", nowSceneIdx);
	auto scene = MainScene::createScene();
	//动态切换场景似乎要push和pop才有效
	Director::sharedDirector()->replaceScene(TransitionJumpZoom::create(0.5, scene));
}
