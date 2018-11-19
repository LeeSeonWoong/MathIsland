#include "AppDelegate.h"
#include "Default.h"

#define AppWidth 1280
#define AppHeight 720
AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate()
{
}

void AppDelegate::initGLContextAttrs()
{
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
	GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();

	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("MathIsland", cocos2d::Rect(0, 0, AppWidth, AppHeight));
#else
		glview = GLViewImpl::create("MathIsland");
#endif
		director->setOpenGLView(glview);
	}
	director->setAnimationInterval(1.0 / 60);
	glview->setDesignResolutionSize(AppWidth, AppHeight, ResolutionPolicy::EXACT_FIT);
	register_all_packages();

	auto scene = TitleScene::createScene();

	// run
	director->runWithScene(scene);
	return true;
}

void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
}
