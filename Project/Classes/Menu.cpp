#include "Menu.h"
#include "Stage.h"

USING_NS_CC;

Scene *MenuScene::createScene() {

	auto scene = Scene::create();

	MenuScene* layer = MenuScene::create();

	scene->addChild(layer);

	return scene;
}



bool MenuScene::init() 
{
	if (!Layer::init()) 
	{
		return false;
	}

	startGameLabel1 = Label::createWithSystemFont("1 player", "Arial", 60);
	startGameLabel1->setTextColor(Color4B::ORANGE);

	startGameLabel2 = Label::createWithSystemFont("2 player", "Arial", 60);
	startGameLabel2->setTextColor(Color4B::ORANGE);

	auto listener = EventListenerMouse::create();
	listener->onMouseUp = CC_CALLBACK_1(MenuScene::startGameCallback, this);

	listener->onMouseMove = CC_CALLBACK_1(MenuScene::highlight, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	Size visibleSize = Director::getInstance()->getVisibleSize();

	startGameLabel1->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 100);

	startGameLabel2->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 100);

	this->addChild(startGameLabel1);

	this->addChild(startGameLabel2);

	return true;
}


void MenuScene::startGameCallback(Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	int xpos = mouseEvent->getLocationInView().x;
	int ypos = Director::getInstance()->getVisibleSize().height + mouseEvent->getLocationInView().y;
	if (xpos > startGameLabel1->getPosition().x - startGameLabel1->getContentSize().width &&
		xpos < startGameLabel1->getPosition().x + startGameLabel1->getContentSize().width &&
		ypos > startGameLabel1->getPosition().y - startGameLabel1->getContentSize().height &&
		ypos < startGameLabel1->getPosition().y + startGameLabel1->getContentSize().height)
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5, StageScene::createScene(false, 4, 4, 1), Color3B(255, 255, 255)));
	if (xpos > startGameLabel2->getPosition().x - startGameLabel2->getContentSize().width &&
		xpos < startGameLabel2->getPosition().x + startGameLabel2->getContentSize().width &&
		ypos > startGameLabel2->getPosition().y - startGameLabel2->getContentSize().height &&
		ypos < startGameLabel2->getPosition().y + startGameLabel2->getContentSize().height)
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5, StageScene::createScene(true, 4, 4, 1), Color3B(255, 255, 255)));
}

void MenuScene::highlight(cocos2d::Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	int xpos = mouseEvent->getLocationInView().x;
	int ypos = Director::getInstance()->getVisibleSize().height + mouseEvent->getLocationInView().y;
	if (xpos > startGameLabel1->getPosition().x - startGameLabel1->getContentSize().width &&
		xpos < startGameLabel1->getPosition().x + startGameLabel1->getContentSize().width &&
		ypos > startGameLabel1->getPosition().y - startGameLabel1->getContentSize().height &&
		ypos < startGameLabel1->getPosition().y + startGameLabel1->getContentSize().height)
		startGameLabel1->setTextColor(Color4B::WHITE);
	else
		startGameLabel1->setTextColor(Color4B::ORANGE);
	if (xpos > startGameLabel2->getPosition().x - startGameLabel2->getContentSize().width &&
		xpos < startGameLabel2->getPosition().x + startGameLabel2->getContentSize().width &&
		ypos > startGameLabel2->getPosition().y - startGameLabel2->getContentSize().height &&
		ypos < startGameLabel2->getPosition().y + startGameLabel2->getContentSize().height)
		startGameLabel2->setTextColor(Color4B::WHITE);
	else
		startGameLabel2->setTextColor(Color4B::ORANGE);
}