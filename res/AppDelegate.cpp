#include "AppDelegate.h"
#include "Scene\TestScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

// 启动函数
bool AppDelegate::applicationDidFinishLaunching() {
	// 初始化导演对象
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		// 修改窗口,16：9最好
		glview = GLViewImpl::createWithRect("Protype", Rect(0, 0, 1366, 768));
		//glview = GLViewImpl::createWithFullScreen("Protype");
		director->setOpenGLView(glview);
	}

	// 开启调试信息显示
	director->setDisplayStats(true);

	// 设置回调次数(在性能过剩时可以视为帧数)
	director->setAnimationInterval(1.0 / 60);

	// 载入初始场景
	auto scene = TestScene::createScene();
		 
	// 启动
	director->runWithScene(scene);	

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
