#include "Battery.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\MainConfig.h"
#include "SimpleAudioEngine.h"

extern CocosDenshion::SimpleAudioEngine *audio;

Battery::Battery()
{
}

bool Battery::init(cocos2d::Sprite * sprite, Player * player)
{
	this->sprite = sprite;
	this->player = player;
	timer = -1;

	this->scheduleUpdate();

	return true;
}

void Battery::update(float dt)
{
	if (timer > 0) {
		timer -= dt;
	}
	else {
		timer = -1;
		sprite->setVisible(true);
	}
	//可以使用时
	if (timer == -1) {
		if (sprite->getBoundingBox().intersectsRect(player->getSprite()->getBoundingBox())) {
			//接到电池
			if (player->getBattery() == true) {
				//接到电池时
				timer = TIMER;
				sprite->setVisible(false);
				audio->playEffect(SOUND_BATTERY.c_str());
			}
			else {
				//无需电池时
				;
			}
		}
	}
}

Battery::~Battery()
{
}
