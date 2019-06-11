#include "Win.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\Character\Player.h"

extern int nowSceneIdx;

void Win::update(float dt)
{
	if (sprite->getBoundingBox().intersectsRect(player->getSprite()->getBoundingBox())) {
		//µ½´ïÖÕµã
		if (nowSceneIdx == initSceneIdx) {
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
