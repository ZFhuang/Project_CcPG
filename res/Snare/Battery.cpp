#include "Battery.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\Character\Player.h"

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
	//����ʹ��ʱ
	if (timer == -1) {
		if (sprite->getBoundingBox().intersectsRect(player->getSprite()->getBoundingBox())) {
			//�ӵ����
			if (player->getBattery() == true) {
				//�ӵ����ʱ
				timer = TIMER;
				sprite->setVisible(false);
			}
			else {
				//������ʱ
				;
			}
		}
	}
}

Battery::~Battery()
{
}
