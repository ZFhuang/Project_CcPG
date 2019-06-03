#include "Needle.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\Character\Player.h"

void Needle::update(float dt)
{
	if (sprite->getBoundingBox().intersectsRect(player->getSpite()->getBoundingBox())) {
		//×²µ½Õë
		gameManager->restartScene();
	}
}

bool Needle::init(Sprite *sprite, Player *player, GameManager *gameManager)
{
	this->sprite = sprite;
	this->player = player;
	this->gameManager = gameManager;

	this->scheduleUpdate();

	return true;
}
