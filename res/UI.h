#pragma once
#include "cocos2d.h"
#include <string>
#include <fstream>

class MainController;

class UI
{
public:
	UI();
	~UI();
	bool init(MainController *controller);
	void loadTextFile(std::string path);
	bool nextLine();
	void update(float dt,cocos2d::Vec2 pos);
	cocos2d::Sprite *talker;
	cocos2d::Label *text;
private:
	MainController *controller;
	std::ifstream *fin;
};