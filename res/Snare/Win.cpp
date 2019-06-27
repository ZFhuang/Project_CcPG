#include "Win.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\MainConfig.h"
#include "SimpleAudioEngine.h"

extern CocosDenshion::SimpleAudioEngine *audio;

extern int nowSceneIdx;

void Win::update(float dt)
{
	//允许少收集一个钥匙
	if (player->getKeyNum()<=1 && sprite->getBoundingBox().intersectsRect(player->getSprite()->getBoundingBox())) {
		//到达终点
		if (nowSceneIdx == initSceneIdx) {
			audio->playEffect(SOUND_PASS.c_str());
			gameManager->nextScene();
			CCLOG("Win: %d", nowSceneIdx);
		}
	}
}

bool Win::init(Sprite *sprite, Player *player, GameManager *gameManager,int initSceneIdx)
{
	this->sprite = sprite;
	this->player = player;
	this->gameManager = gameManager;
	this->initSceneIdx = initSceneIdx;

	this->scheduleUpdate();

	return true;
}
