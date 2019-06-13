#pragma once
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\Snare\Needle.h"
#include "cocos2d.h"
#include <string>

//地图的图层名
static const std::string BACKGROUND_LAYER = "Background";
static const std::string PLATFORM_LAYER ="Platform";
static const std::string SNARE_LAYER = "Snare";
static const std::string TARGET_LAYER = "Target";
static const std::string OBJECT_LAYER = "Object";

//测试地图
// 地图的xml文件位置
static const std::string MAP_TEST = "Map/testPlatform.tmx";
static const std::string MAP_01 = "Map/Level1.tmx";
static const std::string MAP_02 = "Map/Level2.tmx";

//图块标号
static const int NEEDLE_LEFT = 27;
static const int NEEDLE_RIGHT = 27;
static const int NEEDLE_UP = 27;
static const int NEEDLE_DOWN = 27;
static const int BIRTH = 4;
static const int WIN = 21;
static const int BATTERY = 11;
static const int KEY = 3;