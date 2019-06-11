#include "Needle.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\Character\Player.h"

bool Needle::hit = false;

void Needle::update(float dt)
{
	if (box->intersectsRect(player->getSprite()->getBoundingBox())) {
		if (Needle::hit == false) {
			// ײ����
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

	// ������ײ��
	this->box = new Rect();
	float maxx, maxy, minx, miny;
	switch (dir)
	{
	case 1:
		//��
		minx = this->sprite->getBoundingBox().getMinX();
		miny = this->sprite->getBoundingBox().getMinY();
		maxx = this->sprite->getBoundingBox().getMaxX();
		maxy = this->sprite->getBoundingBox().getMidY();
		break;
	case 2:
		//��
		minx = this->sprite->getBoundingBox().getMinX();
		miny = this->sprite->getBoundingBox().getMidY();
		maxx = this->sprite->getBoundingBox().getMaxX();
		maxy = this->sprite->getBoundingBox().getMaxY();
		break;
	case 3:
		//��
		minx = this->sprite->getBoundingBox().getMidX();
		miny = this->sprite->getBoundingBox().getMinY();
		maxx = this->sprite->getBoundingBox().getMaxX();
		maxy = this->sprite->getBoundingBox().getMaxY();
		break;
	case 4:
		//��
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
