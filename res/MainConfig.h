#pragma once
#include "proj.win32\res\Character\Player.h"
#include "proj.win32\res\GameManager.h"
#include "proj.win32\res\Snare\Needle.h"
#include "cocos2d.h"
#include <string>

//��ͼ��ͼ����
static const std::string BACKGROUND_LAYER = "Background";
static const std::string PLATFORM_LAYER ="Platform";
static const std::string SNARE_LAYER = "Snare";
static const std::string TARGET_LAYER = "Target";
static const std::string OBJECT_LAYER = "Object";

//��ͼtag,ͨ��tag�����ֶ���Ĳ�ͬ��ͼ
//���Ե�ͼ
static const int TEST_SCENE = 0;

//ͼ����
static const int NEEDLE = 27;
static const int BIRTH = 4;
static const int WIN = 21;