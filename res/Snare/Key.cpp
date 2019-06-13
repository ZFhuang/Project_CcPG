#include "Key.h"
#include "proj.win32\res\Character\Player.h"

void Key::update(float dt)
{
	if (states == 0) {
		if (sprite->getBoundingBox().intersectsRect(player->getSprite()->getBoundingBox())) {
			//����Ҵ������������
			states = 1;
			target = player->getLastKey(sprite);
		}
	}
	else if (states == 1) {
		if (target != nullptr) {
			//��¼һ����������
			targetPoss[fcounter] = target->getPosition();
			fcounter++;
			if (fcounter >= FRAMES) {
				//��¼��ɣ���ʼ���������
				states = 2;
				fcounter = 0;
			}
		}
	}
	else if (states == 2) {
		if (target != nullptr) {
			//��ʼ����
			sprite->setPosition(targetPoss[fcounter]);
			targetPoss[fcounter] = target->getPosition();
			fcounter++;
			if (fcounter >= FRAMES) {
				//һ�����ڽ�������ʼ�¸�����
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
