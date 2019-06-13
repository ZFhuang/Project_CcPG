#include "Key.h"
#include "proj.win32\res\Character\Player.h"

void Key::update(float dt)
{
	if (states == 0) {
		if (sprite->getBoundingBox().intersectsRect(player->getSprite()->getBoundingBox())) {
			//被玩家触发，载入跟踪
			states = 1;
			target = player->getLastKey(sprite);
		}
	}
	else if (states == 1) {
		if (target != nullptr) {
			//记录一个完整周期
			targetPoss[fcounter] = target->getPosition();
			fcounter++;
			if (fcounter >= FRAMES) {
				//记录完成，开始进入跟踪期
				states = 2;
				fcounter = 0;
			}
		}
	}
	else if (states == 2) {
		if (target != nullptr) {
			//开始跟踪
			sprite->setPosition(targetPoss[fcounter]);
			targetPoss[fcounter] = target->getPosition();
			fcounter++;
			if (fcounter >= FRAMES) {
				//一个周期结束，开始下个周期
				fcounter = 0;
			}
		}
	}
}

bool Key::init(Sprite *sprite, Player *player)
{
	this->sprite = sprite;
	this->player = player;
	//this->tag = tag;
	this->states = 0;
	this->fcounter = 0;
	this->scheduleUpdate();

	return true;
}
