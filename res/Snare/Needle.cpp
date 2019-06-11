#include "Needle.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\Character\Player.h"

bool Needle::hit = false;

void Needle::update(float dt)
{
	if (box->intersectsRect(player->getSprite()->getBoundingBox())) {
		if (Needle::hit == false) {
			// ×²µ½Õë
			gameManager->restartScene();
			Needle::hit = true;
		}
	}
}

bool Needle::init(Sprite *sprite, Player *player, GameManager *gameManager,int dir)
{
	this->sprite = sprite;
	this->player = player;
	this->gameManager = gameManager;
	Needle::hit = false;

	// ¼ÆËãÅö×²ºÐ
	this->box = new Rect();
	float maxx, maxy, minx, miny;
	switch (dir)
	{
	case 1:
		//ÉÏ
		minx = this->sprite->getBoundingBox().getMinX();
		miny = this->sprite->getBoundingBox().getMinY();
		maxx = this->sprite->getBoundingBox().getMaxX();
		maxy = this->sprite->getBoundingBox().getMidY();
		break;
	case 2:
		//ÏÂ
		minx = this->sprite->getBoundingBox().getMinX();
		miny = this->sprite->getBoundingBox().getMidY();
		maxx = this->sprite->getBoundingBox().getMaxX();
		maxy = this->sprite->getBoundingBox().getMaxY();
		break;
	case 3:
		//×ó
		minx = this->sprite->getBoundingBox().getMidX();
		miny = this->sprite->getBoundingBox().getMinY();
		maxx = this->sprite->getBoundingBox().getMaxX();
		maxy = this->sprite->getBoundingBox().getMaxY();
		break;
	case 4:
		//ÓÒ
		minx = this->sprite->getBoundingBox().getMinX();
		miny = this->sprite->getBoundingBox().getMinY();
		maxx = this->sprite->getBoundingBox().getMidX();
		maxy = this->sprite->getBoundingBox().getMaxY();
		break;
	default:
		break;
	}
	this->box->setRect(minx,miny,maxx-minx,maxy-miny);

	this->scheduleUpdate();

	return true;
}
