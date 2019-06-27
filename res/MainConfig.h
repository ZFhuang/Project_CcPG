#pragma once
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\Snare\Needle.h"
#include "cocos2d.h"
#include <string>

//编码转换以显示中文
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

//对话背景图片位置
static const std::string TALKER_BACKGROUND = "../Resources/UI/Talker.png";

//剧情文本位置
static const std::string STORY_DEAD = "../Resources/Story/13.txt";
static const std::string STORY_0 = "../Resources/Story/0.txt";
static const std::string STORY_1 = "../Resources/Story/1.txt";
static const std::string STORY_2 = "../Resources/Story/2.txt";
static const std::string STORY_3 = "../Resources/Story/3.txt";
static const std::string STORY_4 = "../Resources/Story/4.txt";
static const std::string STORY_5 = "../Resources/Story/5.txt";
static const std::string STORY_6 = "../Resources/Story/6.txt";
static const std::string STORY_7 = "../Resources/Story/7.txt";
static const std::string STORY_8 = "../Resources/Story/8.txt";
static const std::string STORY_9 = "../Resources/Story/9.txt";
static const std::string STORY_10 = "../Resources/Story/10.txt";

//音乐
static const std::string MUSIC = "../Resources/Sounds/JohannSebastianBach.mp3";

//音效
static const std::string SOUND_JUMP = "../Resources/Sounds/jump.wav";
static const std::string SOUND_DEATH = "../Resources/Sounds/death.wav";
static const std::string SOUND_RUN = "../Resources/Sounds/run.wav";
static const std::string SOUND_RUSH = "../Resources/Sounds/rush.wav";
static const std::string SOUND_BATTERY = "../Resources/Sounds/battery.wav";
static const std::string SOUND_ROBOTBI = "../Resources/Sounds/RobotBi.wav";
static const std::string SOUND_PASS = "../Resources/Sounds/pass.wav";

//地图的图层名
static const std::string BACKGROUND_LAYER = "Background";
static const std::string PLATFORM_LAYER ="Platform";
static const std::string SNARE_LAYER = "Snare";
static const std::string TARGET_LAYER = "Target";
static const std::string OBJECT_LAYER = "Object";

// 地图的xml文件位置
static const std::string MAP_TEST = "../Resources/Map/testPlatform.tmx";
static const std::string MAP_00 = "../Resources/Map/Level0.tmx";
static const std::string MAP_01 = "../Resources/Map/Level1.tmx";
static const std::string MAP_02 = "../Resources/Map/Level2.tmx";
static const std::string MAP_03 = "../Resources/Map/Level3.tmx";
static const std::string MAP_04 = "../Resources/Map/Level4.tmx";
static const std::string MAP_05 = "../Resources/Map/Level5.tmx";
static const std::string MAP_06 = "../Resources/Map/Level6.tmx";
static const std::string MAP_07 = "../Resources/Map/Level7.tmx";
static const std::string MAP_08 = "../Resources/Map/Level8.tmx";
static const std::string MAP_09 = "../Resources/Map/Level9.tmx";
static const std::string MAP_10 = "../Resources/Map/Level10.tmx";

//图块标号
static const int NEEDLE_LEFT = 197;
static const int NEEDLE_RIGHT = 199;
static const int NEEDLE_UP = 196;
static const int NEEDLE_DOWN = 198;
static const int BIRTH = 106;
static const int WIN1 = 16;
static const int WIN2 = 17;
static const int WIN3 = 18;
static const int WIN4 = 31;
static const int WIN5 = 32;
static const int WIN6 = 33;
static const int WIN7 = 46;
static const int WIN8 = 47;
static const int WIN9 = 48;
static const int BATTERY = 138;
static const int KEY1 = 142;
static const int KEY2 = 144;
