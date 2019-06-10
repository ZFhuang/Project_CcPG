#include "Win.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\Character\Player.h"

void Win::update(float dt)
{
	if (sprite->getBoundingBox().intersectsRect(player->getSprite()->getBoundingBox())) {
		//µ½´ïÖÕµã
		gameManager->nextScene();
	}
}

bool Win::init(Sprite *sprite, Player *player, GameManager *gameManager)
{
	this->sprite = sprite;
	this->player = player;
	this->gameManager = gameManager;

	this->scheduleUpdate();

	return true;
}
