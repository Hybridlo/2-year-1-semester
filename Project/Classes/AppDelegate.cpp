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
		glview = GLViewImpl::createWithRect("Old Tanks game", Rect(0, 0, 1000, 960));
		glview->setFrameSize(1000, 960);
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