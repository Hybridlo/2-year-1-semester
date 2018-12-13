#include "Game.h"

USING_NS_CC;

Scene *GameScene::createScene(int diff) {

	auto scene = Scene::create();

	GameScene* layer = GameScene::create();

	layer->diff = diff;

	scene->addChild(layer);

	return scene;
}



bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	srand(std::chrono::duration_cast<std::chrono::seconds>
		(std::chrono::high_resolution_clock::now().time_since_epoch()).count());

	Size visibleSize = Director::getInstance()->getVisibleSize();

	ok = Sprite::create("ok.png");
	good = Sprite::create("good.png");
	great = Sprite::create("great.png");
	ok->setPosition(visibleSize.width / 4, visibleSize.height / 4 * 3);
	good->setPosition(visibleSize.width / 4, visibleSize.height / 4 * 3);
	great->setPosition(visibleSize.width / 4, visibleSize.height / 4 * 3);
	this->addChild(ok);
	this->addChild(good);
	this->addChild(great);

	scoreLabel = Label::createWithSystemFont("0", "Arial", 40);
	scoreLabel->setPosition(visibleSize.width - 150, visibleSize.height - 70);
	this->addChild(scoreLabel);

	lastArrowScore = Sprite::create("greatScore.png");
	lastArrowScore->setPosition(visibleSize.width - 150, visibleSize.height - 150);
	this->addChild(lastArrowScore);

	lastArrow = std::chrono::high_resolution_clock::now();

	auto eventListener = EventListenerKeyboard::create();

	Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (!arrowPressed && ((keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW && arrowWay == 0) ||
			(keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW && arrowWay == 1) ||
			(keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW && arrowWay == 2) ||
			(keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW && arrowWay == 3)))
		{
			if (arrow->getPosition().y - arrow->getContentSize().height / 2 > ok->getPosition().y - ok->getContentSize().height / 2 &&
				arrow->getPosition().y + arrow->getContentSize().height / 2 < ok->getPosition().y + ok->getContentSize().height / 2)
			{
				score += 50;
				lastArrowScore->setTexture("okScore.png");
				if (arrow->getPosition().y - arrow->getContentSize().height / 2 > good->getPosition().y - good->getContentSize().height / 2 &&
					arrow->getPosition().y + arrow->getContentSize().height / 2 < good->getPosition().y + good->getContentSize().height / 2)
				{
					score += 50;
					lastArrowScore->setTexture("goodScore.png");
					if (arrow->getPosition().y - arrow->getContentSize().height / 2 > great->getPosition().y - great->getContentSize().height / 2 &&
						arrow->getPosition().y + arrow->getContentSize().height / 2 < great->getPosition().y + great->getContentSize().height / 2)
					{
						lastArrowScore->setTexture("greatScore.png");
						score += 200;
					}
				}
			}
			else
			{
				score -= 50;
				lastArrowScore->setTexture("failScore.png");
			}
			arrowPressed = true;
		}
		scoreLabel->setString(std::to_string(score));
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	this->scheduleUpdate();

	return true;
}


void GameScene::update(float delta)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Node::update(delta);
	if (std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - lastArrow).count() > 1000 * 1000 / (500 + 40 * diff))
	{
		if (arrow)
			arrow->removeFromParent();
		int way = rand() % 4;
		if (way == 0)
			arrow = Sprite::create("arrowUp.png");
		if (way == 1)
			arrow = Sprite::create("arrowDown.png");
		if (way == 2)
			arrow = Sprite::create("arrowLeft.png");
		if (way == 3)
			arrow = Sprite::create("arrowRight.png");
		arrowWay = way;
		arrow->setPosition(visibleSize.width / 4, arrow->getContentSize().height / 2);
		auto action = MoveBy::create(1, Vec2(0, 500 + 40 * diff));
		arrow->runAction(RepeatForever::create(action));
		this->addChild(arrow);
		lastArrow = std::chrono::high_resolution_clock::now();
		arrowPressed = false;
	}
	if (arrow)
	if (!arrowPressed && arrow->getPosition().y + arrow->getContentSize().height / 2 > ok->getPosition().y + ok->getContentSize().height / 2)
	{
		score -= 50;
		scoreLabel->setString(std::to_string(score));
		lastArrowScore->setTexture("failScore.png");
		arrowPressed = true;
	}
}