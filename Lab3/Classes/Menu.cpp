#include "Menu.h"
#include "Game.h"

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

	chooseDifficulty = Label::createWithSystemFont("Choose difficulty", "Arial", 60);
	chooseDifficulty->setTextColor(Color4B::ORANGE);

	for (int i = 0; i < 10; i++)
	{
		diff[i] = Label::createWithSystemFont(std::to_string(i), "Arial", 45);
		diff[i]->setTextColor(Color4B::ORANGE);
	}

	auto listener = EventListenerMouse::create();
	listener->onMouseUp = CC_CALLBACK_1(MenuScene::startGameCallback, this);

	listener->onMouseMove = CC_CALLBACK_1(MenuScene::highlight, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	Size visibleSize = Director::getInstance()->getVisibleSize();

	chooseDifficulty->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 150);

	int offset = 0;

	for (int i = 0; i < 9; i++)
	{
		if (i != 0 && i % 3 == 0)
			offset++;
		diff[i]->setPosition(visibleSize.width / 2 - 60 + 60 * (i % 3), visibleSize.height / 2 + 60 - 100 * offset);
	}
	diff[9]->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 240);

	this->addChild(chooseDifficulty);

	for (int i = 0; i < 10; i++)
	this->addChild(diff[i]);

	return true;
}


void MenuScene::startGameCallback(Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	int xpos = mouseEvent->getLocationInView().x;
	int ypos = Director::getInstance()->getVisibleSize().height + mouseEvent->getLocationInView().y;
	for (int i = 0; i < 10; i++)
	{
		if (xpos > diff[i]->getPosition().x - diff[i]->getContentSize().width &&
			xpos < diff[i]->getPosition().x + diff[i]->getContentSize().width &&
			ypos > diff[i]->getPosition().y - diff[i]->getContentSize().height &&
			ypos < diff[i]->getPosition().y + diff[i]->getContentSize().height)
			Director::getInstance()->replaceScene(
				TransitionFade::create(0.5, GameScene::createScene(i), Color3B(255, 255, 255)));
	}
}

void MenuScene::highlight(cocos2d::Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	int xpos = mouseEvent->getLocationInView().x;
	int ypos = Director::getInstance()->getVisibleSize().height + mouseEvent->getLocationInView().y;
	for (int i = 0; i < 10; i++)
	{
		if (xpos > diff[i]->getPosition().x - diff[i]->getContentSize().width &&
			xpos < diff[i]->getPosition().x + diff[i]->getContentSize().width &&
			ypos > diff[i]->getPosition().y - diff[i]->getContentSize().height &&
			ypos < diff[i]->getPosition().y + diff[i]->getContentSize().height)
			diff[i]->setTextColor(Color4B::WHITE);
		else
			diff[i]->setTextColor(Color4B::ORANGE);
	}
}