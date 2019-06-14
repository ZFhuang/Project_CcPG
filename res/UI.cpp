#include"UI.h"
#include <fstream>
#include"proj.win32\res\MainConfig.h"
#include"proj.win32\res\MainController.h"

//编码转换以显示中文
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

UI::UI()
{
}

UI::~UI()
{
}

bool UI::init(MainController * controller)
{
	this->controller = controller;
	talker = Sprite::create(TALKER_BACKGROUND);
	talker->setOpacity(230);
	talker->setScale(0.8);
	talker->setPosition(Vec2(0,0));
	talker->setVisible(false);
	text = cocos2d::Label::createWithSystemFont(" ", "GB2312", 24);
	text->setVisible(false);
	return true;
}

void UI::loadTextFile(std::string path)
{
	fin=new std::ifstream(path);
	talker->setVisible(true);
	text->setVisible(true);
	nextLine();
}

bool UI::nextLine()
{
	if (fin == nullptr) {
		return false;
	}
	std::string s;
	if (std::getline(*fin, s)) {
		text->setString(s);
		return true;
	}
	else {
		text->setString(" ");
		talker->setVisible(false);
		text->setVisible(false);
		fin->close();
		fin = nullptr;
		return false;
	}
}

void UI::update(float dt, cocos2d::Vec2 pos)
{
	talker->setPosition(Vec2(pos.x,pos.y-130));
	text->setPosition(Vec2(talker->getPositionX()+100, talker->getPositionY()));
}

