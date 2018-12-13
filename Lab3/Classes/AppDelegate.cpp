#include "AppDelegate.h"
#include "Menu.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{

}

bool AppDelegate::applicationDidFinishLaunching()
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview)
	{
		glview = GLViewImpl::createWithRect("Hello World", Rect(0, 0, 700, 1000));
		glview->setFrameSize(700, 1000);
		director->setOpenGLView(glview);
	}

	auto scene = MenuScene::createScene();
	director->setDisplayStats(true);
	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground()
{

}

void AppDelegate::applicationWillEnterForeground()
{

}