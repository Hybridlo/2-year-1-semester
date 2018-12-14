#include "Stage.h"

USING_NS_CC;

cocos2d::Scene* StageScene::createScene(bool p2, int p1l, int p2l, int level)
{
	auto scene = cocos2d::Scene::create();

	auto layer = StageScene::create();

	layer->level = level;
	layer->player1Lives = p1l;
	if (p2)
	{
		layer->player2Lives = p2l;
		layer->p2 = p2;
	}

	scene->addChild(layer);
	return scene;
}

void StageScene::setNewBody(Node *sprite, int xpos, int ypos, StageScene * set)
{
	sprite->setAnchorPoint(Vec2(0.0, 0.0));
	sprite->setPosition(xpos, ypos);
	set->addChild(sprite);
}

void StageScene::makeWallPiece(int xpos, int ypos, StageScene *set, bool armor)
{
	if (armor)
	{
		std::vector<Node*> wall(1);
		wall[0] = Sprite::create("armorWallPiece.png");
		setNewBody(wall[0], xpos, ypos, set);
		collision[(int)((xpos - xplay) / 16)][(int)((ypos - yplay) / 16)] = 254;
		collision[(int)((xpos - xplay) / 16)][(int)((ypos - yplay) / 16) + 1] = 254;
		collision[(int)((xpos - xplay) / 16) + 1][(int)((ypos - yplay) / 16) + 1] = 254;
		collision[(int)((xpos - xplay) / 16) + 1][(int)((ypos - yplay) / 16)] = 254;
		walls.push_back(wall);
	}
	else
	{
		std::vector<Node*> wall(4);
		wall[0] = Sprite::create("brickWallPiece3.png");
		setNewBody(wall[0], xpos, ypos, set);
		collision[(int)((xpos - xplay) / 16)][(int)((ypos - yplay) / 16)] = 255;

		wall[1] = Sprite::create("brickWallPiece1.png");
		setNewBody(wall[1], xpos, ypos + 16, set);
		collision[(int)((xpos - xplay) / 16)][(int)((ypos - yplay) / 16) + 1] = 255;

		wall[2] = Sprite::create("brickWallPiece2.png");
		setNewBody(wall[2], xpos + 16, ypos + 16, set);
		collision[(int)((xpos - xplay) / 16) + 1][(int)((ypos - yplay) / 16) + 1] = 255;

		wall[3] = Sprite::create("brickWallPiece4.png");
		setNewBody(wall[3], xpos + 16, ypos, set);
		collision[(int)((xpos - xplay) / 16) + 1][(int)((ypos - yplay) / 16)] = 255;

		walls.push_back(wall);
	}
}

void StageScene::makeWall(int xpos, int ypos, StageScene *set, bool armor)
{
	makeWallPiece(xpos, ypos, set, armor);
	makeWallPiece(xpos, ypos + 32, set, armor);
	makeWallPiece(xpos + 32, ypos + 32, set, armor);
	makeWallPiece(xpos + 32, ypos, set, armor);
}

void StageScene::makeBase()
{
	base = Sprite::create("base.png");
	setNewBody(base, xplay + 64 * 6, yplay, this);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			collision[24 + i][j] = 200;
}

void StageScene::buildMap(StageScene *set)
{
	mapBuilt = true;
	if (p2)
		respTime = (190 - level * 4 - 20) * 1000 / 60;
	else
		respTime = (190 - level * 4) * 1000 / 60;

	player1 = new Tank;
	player1->lastShoot = std::chrono::high_resolution_clock::now();
	player1->playerLock = std::chrono::high_resolution_clock::now() - std::chrono::hours(1);
	player1->spawn = std::chrono::high_resolution_clock::now() - std::chrono::hours(1);
	player1->name = "player1u1";
	player1->speed = 3;
	player1->enemyNumber = 1;
	if (p2)
	{
		player2 = new Tank;
		player2->lastShoot = std::chrono::high_resolution_clock::now();
		player2->playerLock = std::chrono::high_resolution_clock::now() - std::chrono::hours(1);
		player2->spawn = std::chrono::high_resolution_clock::now() - std::chrono::hours(1);
		player2->name = "player2u1";
		player2->speed = 3;
		player2->enemyNumber = 2;
	}
	std::ifstream fin("level" + std::to_string(level) + ".txt");
	if (fin.fail())
	{
		gameComplete();
		return;
	}
	makeBase();
	std::string line = "";
	getline(fin, line);
	while (line != "-----")
	{
		char size = '0', type = '0';
		int xpos, ypos;
		std::string temp;
		size = line[0];
		line.erase(line.begin(), line.begin() + 2);
		temp = line.substr(0, line.find(' '));
		xpos = std::stoi(temp);
		line.erase(line.begin(), line.begin() + line.find(' ') + 1);
		temp = line.substr(0, line.find(' '));
		ypos = std::stoi(temp);
		line.erase(line.begin(), line.begin() + line.find(' ') + 1);
		type = line[0];
		bool armor = (type == 't');
		if (size == 'b')
			makeWall(xplay + 32 * xpos, yplay + 32 * ypos, set, armor);
		if (size == 's')
			makeWallPiece(xplay + 32 * xpos, yplay + 32 * ypos, set, armor);
		getline(fin, line);
	}
	while (!fin.eof())
	{
		fin >> line;
		if (line == "e1")
		{
			Tank *enemy = new Tank;
			enemy->name = "enemy1u1";
			enemy->speed = 2;
			enemy->enemyNumber = enemies.size() + 3;
			enemies.push_back(enemy);
		}
		if (line == "e2")
		{
			Tank *enemy = new Tank;
			enemy->name = "enemy2u1";
			enemy->speed = 4;
			enemy->enemyNumber = enemies.size() + 3;
			enemies.push_back(enemy);
		}
		Node *enemyIcon = Sprite::create("enemyTank.png");
		if (enemiesLeft.size() % 2 == 0)
			enemyIcon->setPosition(xplay + 832 + 10, yplay + 800 - (int)(enemiesLeft.size() / 2) * 32);
		else
			enemyIcon->setPosition(xplay + 832 + 42, yplay + 800 - (int)(enemiesLeft.size() / 2) * 32);
		enemyIcon->setAnchorPoint(Vec2(0.0, 0.0));
		enemiesLeft.push_back(enemyIcon);
		this->addChild(enemyIcon);
	}
	fin.close();
	auto stat1 = Sprite::create("p1stat.png");
	stat1->setPosition(xplay + 832 + 10, yplay + 800 - 480);
	stat1->setAnchorPoint(Vec2(0.0, 0.0));
	this->addChild(stat1);
	p1l = Sprite::create(std::to_string(player1Lives) + ".png");
	p1l->setPosition(xplay + 832 + 42, yplay + 800 - 480);
	p1l->setAnchorPoint(Vec2(0.0, 0.0));
	this->addChild(p1l);
	if (p2)
	{
		auto stat2 = Sprite::create("p2stat.png");
		stat2->setPosition(xplay + 832 + 10, yplay + 800 - 480 - 64);
		stat2->setAnchorPoint(Vec2(0.0, 0.0));
		this->addChild(stat2);
		p2l = Sprite::create(std::to_string(player2Lives) + ".png");
		p2l->setPosition(xplay + 832 + 42, yplay + 800 - 480 - 64);
		p2l->setAnchorPoint(Vec2(0.0, 0.0));
		this->addChild(p2l);
	}
}

bool StageScene::init()
{
	if (!LayerColor::initWithColor(Color4B(99, 99, 99, 255)))
	{
		return false;
	}

	initTime = std::chrono::high_resolution_clock::now();

	srand(std::chrono::duration_cast<std::chrono::seconds>
		(std::chrono::high_resolution_clock::now().time_since_epoch()).count());

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Size playfieldSize;
	playfieldSize.setSize(832, 832);

	Node *playfield = Sprite::create("Playfield.png");
	playfield->setAnchorPoint(Vec2(0.5, 0.5));
	playfield->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	this->addChild(playfield);

	auto eventListener = EventListenerKeyboard::create();

	Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) 
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_ENTER && !pause)
		{
			pauseLabel = Label::createWithSystemFont("Pause", "Arial", 60);
			pauseLabel->setTextColor(Color4B::ORANGE);;
			pauseLabel->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
			pause = true;
			this->addChild(pauseLabel);
			this->pauseSchedulerAndActions();
			this->getEventDispatcher()->resumeEventListenersForTarget(this);
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER && pause)
		{
			pauseLabel->removeFromParent();
			this->resumeSchedulerAndActions();
			pause = false;
		}
		else if (keys.find(keyCode) == keys.end()) 
			keys[keyCode] = std::chrono::high_resolution_clock::now();
	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		keys.erase(keyCode);
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	this->scheduleUpdate();
	return true;
}

bool StageScene::isKeyPressed(EventKeyboard::KeyCode code) 
{
	if (keys.find(code) != keys.end())
		return true;
	return false;
}

bool StageScene::isColliding(int dir, Sprite *moving)
{
	int xpos = moving->getPosition().x - xplay;
	int ypos = moving->getPosition().y - yplay;
	if (dir == 0 && (collision[(int)(xpos / 16)][(int)(ypos / 16 + 4)] != 0 ||
		collision[(int)(xpos / 16) + 1][(int)(ypos / 16 + 4)] != 0 ||
		collision[(int)(xpos / 16) + 2][(int)(ypos / 16 + 4)] != 0 ||
		collision[(int)(xpos / 16) + 3][(int)(ypos / 16 + 4)] != 0))
		return true;
	if (dir == 1 && (collision[(int)((xpos) / 16)][(int)(ypos / 16) - 1] != 0 ||
		collision[(int)(xpos / 16) + 1][(int)(ypos / 16 - 1)] != 0 ||
		collision[(int)(xpos / 16) + 2][(int)(ypos / 16 - 1)] != 0 ||
		collision[(int)(xpos / 16) + 3][(int)(ypos / 16 - 1)] != 0))
		return true;
	if (dir == 2 && (collision[(int)(xpos / 16 - 1)][(int)(ypos / 16)] != 0 ||
		collision[(int)(xpos / 16 - 1)][(int)(ypos / 16) + 1] != 0 ||
		collision[(int)(xpos / 16 - 1)][(int)(ypos / 16) + 2] != 0 ||
		collision[(int)(xpos / 16 - 1)][(int)(ypos / 16) + 3] != 0))
		return true;
	if (dir == 3 && (collision[(int)(xpos / 16 + 4)][(int)(ypos / 16)] != 0 ||
		collision[(int)(xpos / 16 + 4)][(int)(ypos / 16) + 1] != 0 ||
		collision[(int)(xpos / 16 + 4)][(int)(ypos / 16) + 2] != 0 ||
		collision[(int)(xpos / 16 + 4)][(int)(ypos / 16) + 3] != 0))
		return true;
	return false;
}

bool StageScene::isBulletColliding(int dir, cocos2d::Sprite *moving, int bulletNumber)
{
	int xpos = moving->getPosition().x - xplay;
	int ypos = moving->getPosition().y - yplay;
	if (dir == 0 && ((collision[(int)((xpos - 28) / 16) + 1][(int)(ypos / 16 + 1)] == bulletNumber &&
		collision[(int)((xpos - 28) / 16) + 2][(int)(ypos / 16 + 1)] == bulletNumber) ||
		(collision[(int)((xpos - 28) / 16) + 1][(int)(ypos / 16 + 1)] == bulletNumber - 100 &&
		collision[(int)((xpos - 28) / 16) + 2][(int)(ypos / 16 + 1)] == bulletNumber - 100)))
		return false;
	if (dir == 0 && (collision[(int)((xpos - 28) / 16) + 1][(int)(ypos / 16 + 1)] != 0 ||
		collision[(int)((xpos - 28) / 16) + 2][(int)(ypos / 16 + 1)] != 0 ))
		return true;
	if (dir == 1 && ((collision[(int)((xpos - 28) / 16) + 1][(int)(ypos / 16)] == bulletNumber &&
		collision[(int)((xpos - 28) / 16) + 2][(int)(ypos / 16)] == bulletNumber) ||
		(collision[(int)((xpos - 28) / 16) + 1][(int)(ypos / 16)] == bulletNumber - 100 &&
		collision[(int)((xpos - 28) / 16) + 2][(int)(ypos / 16)] == bulletNumber - 100)))
		return false;
	if (dir == 1 && (collision[(int)((xpos - 28) / 16) + 1][(int)(ypos / 16)] != 0 ||
		collision[(int)((xpos - 28) / 16) + 2][(int)(ypos / 16)] != 0))
		return true;
	if (dir == 2 && ((collision[(int)(xpos / 16)][(int)((ypos - 28) / 16) + 1] == bulletNumber &&
		collision[(int)(xpos / 16)][(int)((ypos - 28) / 16) + 2] == bulletNumber) ||
		(collision[(int)(xpos / 16)][(int)((ypos - 28) / 16) + 1] == bulletNumber - 100 &&
		collision[(int)(xpos / 16)][(int)((ypos - 28) / 16) + 2] == bulletNumber - 100)))
		return false;
	if (dir == 2 && (collision[(int)(xpos / 16)][(int)((ypos - 28) / 16) + 1] != 0 ||
		collision[(int)(xpos / 16)][(int)((ypos - 28) / 16) + 2] != 0))
		return true;
	if (dir == 3 && ((collision[(int)(xpos / 16 + 1)][(int)((ypos - 28) / 16) + 1] == bulletNumber &&
		collision[(int)(xpos / 16 + 1)][(int)((ypos - 28) / 16) + 2] == bulletNumber) ||
		(collision[(int)(xpos / 16 + 1)][(int)((ypos - 28) / 16) + 1] == bulletNumber - 100 &&
			collision[(int)(xpos / 16 + 1)][(int)((ypos - 28) / 16) + 2] == bulletNumber - 100)))
		return false;
	if (dir == 3 && (collision[(int)(xpos / 16 + 1)][(int)((ypos - 28) / 16) + 1] != 0 ||
		collision[(int)(xpos / 16 + 1)][(int)((ypos - 28) / 16) + 2] != 0))
		return true;
	return false;
}

void StageScene::moveTank(Tank *tank, std::string move, float delta)
{
	Sprite *sprite = tank->getSprite();
	if (tank->ypos != (int)sprite->getPosition().y && tank->movingUp && sprite->getNumberOfRunningActions() == 1)
	{
		sprite->stopAllActions();
		tank->movingUp = false;
		for (int i = 0; i < 4; i++)
		{
			collision[(int)((sprite->getPosition().x - xplay) / 16) + i][(int)((sprite->getPosition().y - yplay) / 16) - 1] = 0;
		}
	}
	if (move == "up" && !tank->movingUp && sprite->getNumberOfRunningActions() == 0)
	{
		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(2);
		animFrames.pushBack(SpriteFrame::create(tank->name + "up1.png", Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height)));
		animFrames.pushBack(SpriteFrame::create(tank->name + "up2.png", Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height)));

		Animation *animation = Animation::createWithSpriteFrames(animFrames, delta * 4);
		Animate *animate = Animate::create(animation);

		auto action = MoveBy::create(delta * 16 / tank->speed, cocos2d::Vec2(0, 16));

		if ((int)(sprite->getPosition().y) + 1 > yplay + 768)
		{
			sprite->setPosition(sprite->getPosition().x, yplay + 768);
			sprite->stopAllActions();
			return;
		}

		if (!isColliding(0, sprite))
		{
			for (int i = 0; i < 4; i++)
			{
				collision[(int)((sprite->getPosition().x - xplay) / 16) + i][(int)((sprite->getPosition().y - yplay) / 16) + 4] = tank->enemyNumber;
			}
			sprite->runAction(RepeatForever::create(animate));
			cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
			actions.pushBack(action);
			actions.pushBack(DelayTime::create(0.02));
			auto sequence = Sequence::create(actions);
			sprite->runAction(sequence);
			tank->movingUp = true;
			tank->movingDown = false;
			tank->movingLeft = false;
			tank->movingRight = false;
			
		}
		else
			sprite->setTexture(tank->name + "up1.png");

		tank->way = 0;
		tank->ypos = (int)sprite->getPosition().y;

	}
	if (tank->ypos != (int)sprite->getPosition().y && tank->movingDown && sprite->getNumberOfRunningActions() == 1)
	{
		sprite->stopAllActions();
		tank->movingDown = false;
		for (int i = 0; i < 4; i++)
		{
			collision[(int)((sprite->getPosition().x - xplay) / 16) + i][(int)((sprite->getPosition().y - yplay) / 16) + 4] = 0;
		}
	}
	if (move == "down" && !tank->movingDown && sprite->getNumberOfRunningActions() == 0)
	{
		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(2);
		animFrames.pushBack(SpriteFrame::create(tank->name + "down1.png", Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height)));
		animFrames.pushBack(SpriteFrame::create(tank->name + "down2.png", Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height)));

		Animation *animation = Animation::createWithSpriteFrames(animFrames, delta * 4);
		Animate *animate = Animate::create(animation);

		if ((int)(sprite->getPosition().y) - 1 < yplay)
		{
			sprite->setPosition(sprite->getPosition().x, yplay);
			sprite->stopAllActions();
			return;
		}

		auto action = MoveBy::create(delta * 16 / tank->speed, cocos2d::Vec2(0, -16));

		if (!isColliding(1, sprite))
		{
			for (int i = 0; i < 4; i++)
			{
				collision[(int)((sprite->getPosition().x - xplay) / 16) + i][(int)((sprite->getPosition().y - yplay) / 16) - 1] = tank->enemyNumber;
			}
			sprite->runAction(RepeatForever::create(animate));
			cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
			actions.pushBack(action);
			actions.pushBack(DelayTime::create(0.02));
			auto sequence = Sequence::create(actions);
			sprite->runAction(sequence);
			tank->movingUp = false;
			tank->movingDown = true;
			tank->movingLeft = false;
			tank->movingRight = false;
		}
		else
			sprite->setTexture(tank->name + "down1.png");

		tank->way = 1;
		tank->ypos = (int)sprite->getPosition().y;

	}
	if (tank->xpos != (int)sprite->getPosition().x && tank->movingLeft && sprite->getNumberOfRunningActions() == 1)
	{
		sprite->stopAllActions();
		tank->movingLeft = false;
		for (int i = 0; i < 4; i++)
		{
			collision[(int)((sprite->getPosition().x - xplay) / 16) + 4][(int)((sprite->getPosition().y - yplay) / 16) + i] = 0;
		}
	}
	if (move == "left" && !tank->movingLeft && sprite->getNumberOfRunningActions() == 0)
	{
		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(2);
		animFrames.pushBack(SpriteFrame::create(tank->name + "left1.png", Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height)));
		animFrames.pushBack(SpriteFrame::create(tank->name + "left2.png", Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height)));

		Animation *animation = Animation::createWithSpriteFrames(animFrames, delta * 4);
		Animate *animate = Animate::create(animation);

		if ((int)(sprite->getPosition().x) - 1 < xplay)
		{
			sprite->setPosition(xplay, sprite->getPosition().y);
			sprite->stopAllActions();
			return;
		}

		auto action = MoveBy::create(delta * 16 / tank->speed, cocos2d::Vec2(-16, 0));

		if (!isColliding(2, sprite))
		{
			for (int i = 0; i < 4; i++)
			{
				collision[(int)((sprite->getPosition().x - xplay) / 16) - 1][(int)((sprite->getPosition().y - yplay) / 16) + i] = tank->enemyNumber;
			}
			sprite->runAction(RepeatForever::create(animate));
			cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
			actions.pushBack(action);
			actions.pushBack(DelayTime::create(0.02));
			auto sequence = Sequence::create(actions);
			sprite->runAction(sequence);
			tank->movingUp = false;
			tank->movingDown = false;
			tank->movingLeft = true;
			tank->movingRight = false;
		}
		else
		{
			sprite->setTexture(tank->name + "left1.png");
			CCLOG("col");
		}

		tank->way = 2;
		tank->xpos = (int)sprite->getPosition().x;

	}
	if (tank->xpos != (int)sprite->getPosition().x && tank->movingRight && sprite->getNumberOfRunningActions() == 1)
	{
		sprite->stopAllActions();
		tank->movingRight = false;
		for (int i = 0; i < 4; i++)
		{
			collision[(int)((sprite->getPosition().x - xplay) / 16) - 1][(int)((sprite->getPosition().y - yplay) / 16) + i] = 0;
		}
	}
	if (move == "right" && !tank->movingRight && sprite->getNumberOfRunningActions() == 0)
	{
		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(2);
		animFrames.pushBack(SpriteFrame::create(tank->name + "right1.png", Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height)));
		animFrames.pushBack(SpriteFrame::create(tank->name + "right2.png", Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height)));

		Animation *animation = Animation::createWithSpriteFrames(animFrames, delta * 4);
		Animate *animate = Animate::create(animation);

		auto action = MoveBy::create(delta * 16 / tank->speed, cocos2d::Vec2(16, 0));

		if ((int)(sprite->getPosition().x) + 1 > xplay + 768)
		{
			sprite->setPosition(xplay + 768, sprite->getPosition().y);
			sprite->stopAllActions();
			return;
		}

		if (!isColliding(3, sprite))
		{
			for (int i = 0; i < 4; i++)
			{
				collision[(int)((sprite->getPosition().x - xplay) / 16) + 4][(int)((sprite->getPosition().y - yplay) / 16) + i] = tank->enemyNumber;
			}
			sprite->runAction(RepeatForever::create(animate));
			cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
			actions.pushBack(action);
			actions.pushBack(DelayTime::create(0.02));
			auto sequence = Sequence::create(actions);
			sprite->runAction(sequence);
			tank->movingUp = false;
			tank->movingDown = false;
			tank->movingLeft = false;
			tank->movingRight = true;
		}
		else
			sprite->setTexture(tank->name + "right1.png");

		tank->way = 3;
		tank->xpos = (int)sprite->getPosition().x;

	}
}

void StageScene::shoot(Tank *tank, float delta)
{
	tank->lastShoot = std::chrono::high_resolution_clock::now();
	Sprite *sprite = tank->getSprite();
	Bullet *bullet = new Bullet;
	bullet->owner = tank;
	bullet->way = tank->way;
	MoveBy *action;
	if (bullet->way == 0)
	{
		bullet->setSprite(Sprite::create("bulletUp.png"));
		bullet->getSprite()->setPosition(sprite->getPosition().x + 28, sprite->getPosition().y + 56);
		action = MoveBy::create(delta, Vec2(0, 8 * tank->bulletSpeed));
	}
	else if (bullet->way == 1)
	{
		bullet->setSprite(Sprite::create("bulletDown.png"));
		bullet->getSprite()->setPosition(sprite->getPosition().x + 28, sprite->getPosition().y + 8);
		action = MoveBy::create(delta, Vec2(0, -8 * tank->bulletSpeed));
	}
	else if (bullet->way == 2)
	{
		bullet->setSprite(Sprite::create("bulletLeft.png"));
		bullet->getSprite()->setPosition(sprite->getPosition().x + 8, sprite->getPosition().y + 28);
		action = MoveBy::create(delta, Vec2(-8 * tank->bulletSpeed, 0));
	}
	else if (bullet->way == 3)
	{
		bullet->setSprite(Sprite::create("bulletRight.png"));
		bullet->getSprite()->setPosition(sprite->getPosition().x + 56, sprite->getPosition().y + 28);
		action = MoveBy::create(delta, Vec2(8 * tank->bulletSpeed, 0));
	}
	bullet->getSprite()->setAnchorPoint(Vec2(0.0, 0.0));
	bullet->getSprite()->runAction(RepeatForever::create(action));
	tank->bullet = true;
	bullets.push_back(bullet);
	this->addChild(bullet->getSprite());
}

void StageScene::removeExploded()
{
	for (int i = 0; i < explosions.size(); i++)
		if (explosions[i]->getNumberOfRunningActions() == 0)
		{
			explosions[i]->removeFromParent();
			explosions.erase(explosions.begin() + i);
		}
}

void StageScene::itemExplode(Sprite *explode, bool type, int xpos, int ypos)
{
	explode = Sprite::create("explode1.png");
	if (!type)
		explode->setPosition(xpos, ypos);
	else
		explode->setPosition(xpos + 16, ypos + 16);

	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(3);
	animFrames.pushBack(SpriteFrame::create("explode1.png", Rect(0, 0, 64, 64)));
	animFrames.pushBack(SpriteFrame::create("explode2.png", Rect(0, 0, 64, 64)));
	animFrames.pushBack(SpriteFrame::create("explode3.png", Rect(0, 0, 64, 64)));
	if (type)
	{
		animFrames.pushBack(SpriteFrame::create("explode4.png", Rect(0, 0, 128, 128)));
		animFrames.pushBack(SpriteFrame::create("explode5.png", Rect(0, 0, 128, 128)));
	}

	Animation *animation = Animation::createWithSpriteFrames(animFrames, 0.02 * 3);
	Animate *animate = Animate::create(animation);
	explode->runAction(animate);
	this->addChild(explode);
	explosions.push_back(explode);
}

bool StageScene::bulletDestroy(Bullet *bullet)
{
	int x = bullet->getSprite()->getPosition().x - xplay, y = bullet->getSprite()->getPosition().y - yplay;
	int x1 = -1, x2 = -1, x3 = -1, x4 = -1, y1 = -1, y2 = -1, y3 = -1, y4 = -1;
	bool wallDestroyed = false;
	if (bullet->way == 0)
	{
		x -= 26;
			x = x - x % 16;
			y = y - y % 16;
		x1 = x;
		x2 = x + 16;
		x3 = x + 32;
		x4 = x + 48;
		y1 = y + 16;

	}
	else if (bullet->way == 1)
	{
		x -= 26;
			x = x - x % 16;
			y = y + 16 - y % 16;
		x1 = x;
		x2 = x + 16;
		x3 = x + 32;
		x4 = x + 48;
		y1 = y - 16;
	}
	else if (bullet->way == 2)
	{
		y -= 26;
			x = x + 16 - x % 16;
			y = y - y % 16;
		x1 = x - 16;
		y1 = y;
		y2 = y + 16;
		y3 = y + 32;
		y4 = y + 48;
	}
	else if (bullet->way == 3)
	{
		y -= 26;
			x = x - x % 16;
			y = y - y % 16;
		x1 = x + 16;
		y1 = y;
		y2 = y + 16;
		y3 = y + 32;
		y4 = y + 48;
	}
	if ((bullet->owner != player1 && bullet->owner != player2) && player1->spawned)
	{
		for (int i = -1; i < 5; i++)
		{
			for (int j = -1; j < 5; j++)
			{
				int xpos = (int)(((player1->getSprite()->getPosition().x - xplay) / 16) + i) * 16;
				int ypos = (int)(((player1->getSprite()->getPosition().y - yplay) / 16) + j) * 16;
				if (collision[(int)(xpos / 16)][(int)(ypos / 16)] == player1->enemyNumber &&
					(xpos == x1 || xpos == x2 || xpos == x3 || xpos == x4) && (ypos == y1 || ypos == y2 || ypos == y3 || ypos == y4) && !wallDestroyed)
				{
					itemExplode(player1->explosion, true, player1->getSprite()->getPosition().x, player1->getSprite()->getPosition().y);
					for (int k = -1; k < 5; k++)
						for (int l = -1; l < 5; l++)
							if ((int)(xpos / 16 - i) + k >= 0 && (int)(ypos / 16 - j) + l >= 0 &&
								(int)(xpos / 16 - i) + k < 52 && (int)(ypos / 16 - j) + l < 52)
								if (collision[(int)(xpos / 16 - i) + k][(int)(ypos / 16 - j) + l] == player1->enemyNumber)
									collision[(int)(xpos / 16 - i) + k][(int)(ypos / 16 - j) + l] = 0;
					player1->getSprite()->removeFromParent();
					player1->spawned = false;
					player1->way = -1;
					player1->spawn = std::chrono::high_resolution_clock::now();
					bullet->owner->bullet = false;
					bullet->getSprite()->removeFromParent();
					return true;
				}
			}
		}
	}
	else if (bullet->owner == player2 && player1->spawned)
	{
		for (int i = -1; i < 5; i++)
		{
			for (int j = -1; j < 5; j++)
			{
				int xpos = (int)(((player1->getSprite()->getPosition().x - xplay) / 16) + i) * 16;
				int ypos = (int)(((player1->getSprite()->getPosition().y - yplay) / 16) + j) * 16;
				if (collision[(int)(xpos / 16)][(int)(ypos / 16)] == player1->enemyNumber &&
					(xpos == x1 || xpos == x2 || xpos == x3 || xpos == x4) && (ypos == y1 || ypos == y2 || ypos == y3 || ypos == y4) && !wallDestroyed)
				{
					player1->playerLock = std::chrono::high_resolution_clock::now();
					bullet->owner->bullet = false;
					bullet->getSprite()->removeFromParent();
					return true;
				}
			}
		}
	}
	if (p2)
		if ((bullet->owner != player1 && bullet->owner != player2) && player2->spawned)
		{
			for (int i = -1; i < 5; i++)
			{
				for (int j = -1; j < 5; j++)
				{
					int xpos = (int)(((player2->getSprite()->getPosition().x - xplay) / 16) + i) * 16;
					int ypos = (int)(((player2->getSprite()->getPosition().y - yplay) / 16) + j) * 16;
					if (collision[(int)(xpos / 16)][(int)(ypos / 16)] == player2->enemyNumber &&
						(xpos == x1 || xpos == x2 || xpos == x3 || xpos == x4) && (ypos == y1 || ypos == y2 || ypos == y3 || ypos == y4) && !wallDestroyed)
					{
						itemExplode(player2->explosion, true, player2->getSprite()->getPosition().x, player2->getSprite()->getPosition().y);
						for (int k = -1; k < 5; k++)
							for (int l = -1; l < 5; l++)
								if ((int)(xpos / 16 - i) + k >= 0 && (int)(ypos / 16 - j) + l >= 0 &&
									(int)(xpos / 16 - i) + k < 52 && (int)(ypos / 16 - j) + l < 52)
									if (collision[(int)(xpos / 16 - i) + k][(int)(ypos / 16 - j) + l] == player2->enemyNumber)
										collision[(int)(xpos / 16 - i) + k][(int)(ypos / 16 - j) + l] = 0;
						player2->getSprite()->removeFromParent();
						player2->spawned = false;
						player2->way = -1;
						player2->spawn = std::chrono::high_resolution_clock::now();
						bullet->owner->bullet = false;
						bullet->getSprite()->removeFromParent();
						return true;
					}
				}
			}
		}
		else if (bullet->owner == player1 && player2->spawned)
		{
			for (int i = -1; i < 5; i++)
			{
				for (int j = -1; j < 5; j++)
				{
					int xpos = (int)(((player2->getSprite()->getPosition().x - xplay) / 16) + i) * 16;
					int ypos = (int)(((player2->getSprite()->getPosition().y - yplay) / 16) + j) * 16;
					if (collision[(int)(xpos / 16)][(int)(ypos / 16)] == player2->enemyNumber &&
						(xpos == x1 || xpos == x2 || xpos == x3 || xpos == x4) && (ypos == y1 || ypos == y2 || ypos == y3 || ypos == y4) && !wallDestroyed)
					{
						player2->playerLock = std::chrono::high_resolution_clock::now();
						bullet->owner->bullet = false;
						bullet->getSprite()->removeFromParent();
						return true;
					}
				}
			}
		}
	if (bullet->owner == player1 || bullet->owner == player2)
	for (int m = 0; m < enemiesOut.size(); m++)
			for (int i = -1; i < 5; i++)
			{
				for (int j = -1; j < 5; j++)
				{
					int xpos = (int)(((enemiesOut[m]->getSprite()->getPosition().x - xplay) / 16) + i) * 16;
					int ypos = (int)(((enemiesOut[m]->getSprite()->getPosition().y - yplay) / 16) + j) * 16;
					if (collision[(int)(xpos / 16)][(int)(ypos / 16)] == enemiesOut[m]->enemyNumber &&
						(xpos == x1 || xpos == x2 || xpos == x3 || xpos == x4) && (ypos == y1 || ypos == y2 || ypos == y3 || ypos == y4) && !wallDestroyed)
					{
						itemExplode(enemiesOut[m]->explosion, true, enemiesOut[m]->getSprite()->getPosition().x, enemiesOut[m]->getSprite()->getPosition().y);
						for (int k = -1; k < 5; k++)
							for (int l = -1; l < 5; l++)
								if ((int)(xpos / 16 - i) + k >= 0 && (int)(ypos / 16 - j) + l >= 0 &&
									(int)(xpos / 16 - i) + k < 52 && (int)(ypos / 16 - j) + l < 52)
									if (collision[(int)(xpos / 16 - i) + k][(int)(ypos / 16 - j) + l] == enemiesOut[m]->enemyNumber)
										collision[(int)(xpos / 16 - i) + k][(int)(ypos / 16 - j) + l] = 0;
						for (int k = 0; k < 52; k++)
							for (int l = 0; l < 52; l++)
								if (collision[k][l] == enemiesOut[m]->enemyNumber + 100)
									collision[k][l] = 0;
						enemiesOut[m]->getSprite()->removeFromParent();
						enemiesOut[m]->setSprite(nullptr);
						enemiesOut.erase(enemiesOut.begin() + m);
						m--;
						bullet->owner->bullet = false;
						bullet->getSprite()->removeFromParent();
						return true;
					}
				}
			}
	for (int i = 0; i < walls.size(); i++)
		for (int j = 0; j < walls[i].size(); j++)
		{
			int xpos = walls[i][j]->getPosition().x - xplay;
			int ypos = walls[i][j]->getPosition().y - yplay;
			if (collision[(int)(xpos / 16)][(int)(ypos / 16)] != 0 && (xpos == x1 || xpos == x2 || xpos == x3 || xpos == x4) && (ypos == y1 || ypos == y2 || ypos == y3 || ypos == y4))
			{
				if (collision[(int)(xpos / 16)][(int)(ypos / 16)] == 254)
					continue;
				wallDestroyed = true;
				walls[i][j]->removeFromParent();
				collision[(int)(xpos / 16)][(int)(ypos / 16)] = 0;
				walls[i].erase(walls[i].begin() + j);
				j--;
				if (walls[i].size() == 0)
				{
					walls.erase(walls.begin() + i);
					if (i > 0)
					i--;
				}
			}
		}
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			int xpos = base->getPosition().x - xplay + i * 16;
			int ypos = base->getPosition().y - yplay + j * 16;
			if (collision[(int)(xpos / 16)][(int)(ypos / 16)] == 200 && (xpos == x1 || xpos == x2 || xpos == x3 || xpos == x4) && (ypos == y1 || ypos == y2 || ypos == y3 || ypos == y4))
			{
				base->setTexture("baseDestroyed.png");
				gameOver();
			}
		}
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i] == bullet)
			continue;
		if (bullet->way == 0)
			y1 += 16;
		else if (bullet->way == 1)
			y1 -= 16;
		else if (bullet->way == 2)
			x1 -= 16;
		else if (bullet->way == 3)
			x1 += 16;
		int xpos = bullets[i]->getSprite()->getPosition().x - xplay;
		int ypos = bullets[i]->getSprite()->getPosition().y - yplay;
		if (bullets[i]->way == 0)
		{
			xpos -= 12;
			ypos -= (ypos - yplay) % 16;
			y2 = y1 - 16;
			y3 = y1 + 16;
		}
		else if (bullets[i]->way == 1)
		{
			xpos -= 12;
			ypos += (ypos - yplay) % 16;
			y2 = y1 - 16;
			y3 = y1 + 16;
		}
		else if (bullets[i]->way == 2)
		{
			xpos -= (xpos - xplay) % 16;
			ypos -= 12;
			x2 = x1 - 16;
			x3 = x1 + 16;
		}
		else if (bullets[i]->way == 3)
		{
			xpos += (xpos - xplay) % 16;
			ypos -= 12;
			x2 = x1 - 16;
			x3 = x1 + 16;
		}
		xpos = (int)(xpos / 16);
		ypos = (int)(ypos / 16);
		if (collision[xpos][ypos] != 0 && (xpos * 16 == x1 || xpos * 16 == x2 || xpos * 16 == x3 || xpos * 16 == x4) &&
			(ypos * 16 == y1 || ypos * 16 == y2 || ypos * 16 == y3 || ypos * 16 == y4))
		{
			bullets[i]->getSprite()->removeFromParent();
			for (int k = -1; k < 3; k++)
				for (int j = -1; j < 3; j++)
					if ((int)(xpos / 16) + k >= 0 && (int)(xpos / 16) + k < 53 &&
						(int)(ypos / 16) + j >= 0 && (int)(ypos / 16) + j < 53)
						if (collision[xpos + k][ypos + j] == 100 + bullets[i]->owner->enemyNumber)
							collision[xpos + k][ypos + j] = 0;
			bullets[i]->owner->bullet = false;
			itemExplode(bullets[i]->explosion, false, xpos * 16 + xplay, ypos * 16 + yplay);
			bullets.erase(bullets.begin() + i);
		}
	}
	bullet->owner->bullet = false;
	bullet->getSprite()->removeFromParent();
	return true;
}

void StageScene::bulletCollision()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		Bullet *bullet = bullets[i];
		if (!bullets[i]->owner->getSprite())
			bullets[i]->owner = player1;
		int xpos = bullets[i]->getSprite()->getPosition().x - xplay;
		int ypos = bullets[i]->getSprite()->getPosition().y - yplay;
		if (isBulletColliding(bullets[i]->way, bullets[i]->getSprite(), bullets[i]->owner->enemyNumber + 100))
		{
			if (bulletDestroy(bullets[i]))
			{
				if (i == bullets.size())
					i--;
				else if (bullets[i] != bullet)
					i--;
				for (int k = -1; k < 3; k++)
					for (int j = -1; j < 3; j++)
						if ((int)(xpos / 16) + k >= 0 && (int)(xpos / 16) + k < 53 &&
							(int)(ypos / 16) + j >= 0 && (int)(ypos / 16) + j < 53)
						{
							if (i >= bullets.size())
								i--;
							if (collision[(int)(xpos / 16) + k][(int)(ypos / 16) + j] == 100 + bullets[i]->owner->enemyNumber)
								collision[(int)(xpos / 16) + k][(int)(ypos / 16) + j] = 0;
						}
				itemExplode(bullets[i]->explosion, false, xpos + xplay, ypos + yplay);
				bullets.erase(bullets.begin() + i);
				i--;
			}
			continue;
		}
		if (bullets[i]->getSprite()->getPosition().x < xplay ||
			bullets[i]->getSprite()->getPosition().x > xplay + 816 ||
			bullets[i]->getSprite()->getPosition().y < yplay ||
			bullets[i]->getSprite()->getPosition().y > yplay + 816)
		{
			for (int k = -1; k < 3; k++)
				for (int j = -1; j < 3; j++)
					if ((int)(xpos / 16) + k >= 0 && (int)(xpos / 16) + k < 52 &&
						(int)(ypos / 16) + j >= 0 && (int)(ypos / 16) + j < 52)
						if (collision[(int)(xpos / 16) + k][(int)(ypos / 16) + j] == 100 + bullets[i]->owner->enemyNumber)
							collision[(int)(xpos / 16) + k][(int)(ypos / 16) + j] = 0;
			bullets[i]->owner->bullet = false;
			bullets[i]->getSprite()->removeFromParent();
			itemExplode(bullets[i]->explosion, false, xpos + xplay, ypos + yplay);
			bullets.erase(bullets.begin() + i);
			i--;
			continue;
		}
		if (xpos % 16 != 0)
			xpos = xpos - xpos % 16;
		if (ypos % 16 != 0)
			ypos = ypos - ypos % 16;
		for (int k = 0; k < 2; k++)
			for (int j = 0; j < 2; j++)
				if (collision[(int)(xpos / 16) + k][(int)(ypos / 16) + j] == 0)
					collision[(int)(xpos / 16) + k][(int)(ypos / 16) + j] = 100 + bullets[i]->owner->enemyNumber;
		for (int k = 0; k < 2; k++)
			for (int j = 0; j < 2; j++)
			{
				if ((int)(xpos / 16) + k + 2 < 53 && bullets[i]->way == 2 && collision[(int)(xpos / 16) + k + 2][(int)(ypos / 16) + j] == 100 + bullets[i]->owner->enemyNumber)
					collision[(int)(xpos / 16) + k + 2][(int)(ypos / 16) + j] = 0;
				else if ((int)(ypos / 16) + j + 2 < 53 && bullets[i]->way == 1 && collision[(int)(xpos / 16) + k][(int)(ypos / 16) + j + 2] == 100 + bullets[i]->owner->enemyNumber)
					collision[(int)(xpos / 16) + k][(int)(ypos / 16) + j + 2] = 0;
				else if ((xpos / 16) + k - 2 > 0 && bullets[i]->way == 3 && collision[(int)(xpos / 16) + k - 2][(int)(ypos / 16) + j] == 100 + bullets[i]->owner->enemyNumber)
					collision[(int)(xpos / 16) + k - 2][(int)(ypos / 16) + j] = 0;
				else if ((int)(ypos / 16) + j - 2 > 0 && bullets[i]->way == 0 && collision[(int)(xpos / 16) + k][(int)(ypos / 16) + j - 2] == 100 + bullets[i]->owner->enemyNumber)
					collision[(int)(xpos / 16) + k][(int)(ypos / 16) + j - 2] = 0;
			}

	}
}

void StageScene::movePlayers(float delta)
{
	if (player1Lives > 0 && std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - player1->playerLock).count() > 3000)
	{
		if (!player1->spawned)
		{
			if (player1Lives == 1)
			{
				player1Lives--;
				p1l->setTexture(std::to_string(player1Lives) + ".png");
				return;
			}
			spawnTank(player1, delta);
		}
		else
		{
			std::string move = "";
			if (isKeyPressed(EventKeyboard::KeyCode::KEY_W))
				move = "up";
			else if (isKeyPressed(EventKeyboard::KeyCode::KEY_S))
				move = "down";
			else if (isKeyPressed(EventKeyboard::KeyCode::KEY_A))
				move = "left";
			else if (isKeyPressed(EventKeyboard::KeyCode::KEY_D))
				move = "right";
			if (isKeyPressed(EventKeyboard::KeyCode::KEY_CTRL) && !player1->bullet &&
				std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::high_resolution_clock::now() - player1->lastShoot).count() > 500)
				shoot(player1, delta);
			moveTank(player1, move, delta);
		}
	}
	if (player2Lives > 0 && std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - player2->playerLock).count() > 3000)
	{
		if (!player2->spawned)
		{
			if (player2Lives == 1)
			{
				player2Lives--;
				p2l->setTexture(std::to_string(player2Lives) + ".png");
				return;
			}
			spawnTank(player2, delta);
		}
		else
		{
			std::string move = "";
			if (isKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW))
				move = "up";
			else if (isKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW))
				move = "down";
			else if (isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW))
				move = "left";
			else if (isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW))
				move = "right";
			if (isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_CTRL) && !player2->bullet &&
				std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::high_resolution_clock::now() - player2->lastShoot).count() > 500)
				shoot(player2, delta);
			moveTank(player2, move, delta);
		}
	}
	if (player1Lives == 0)
		if (p2)
		{
			if (player2Lives == 0)
				gameOver();
		}
		else
			gameOver();
}

void StageScene::gameOver()
{
	Label *over = Label::createWithSystemFont("Game over", "Arial", 60);
	over->setTextColor(Color4B::ORANGE);
	over->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
	this->addChild(over);
	overFlag = true;
}

void StageScene::stageComplete()
{
	Label *over = Label::createWithSystemFont("Stage complete", "Arial", 60);
	over->setTextColor(Color4B::ORANGE);
	over->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
	this->addChild(over);
	completeFlag = true;
}

void StageScene::gameComplete()
{
	Label *over = Label::createWithSystemFont("Game complete", "Arial", 60);
	over->setTextColor(Color4B::ORANGE);
	over->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
	this->addChild(over);
	gameCompleteFlag = true;
}

bool StageScene::spawnTank(Tank *tank, float delta)
{
	int xpos, ypos;
	if (tank->way == -1)
	if (tank == player1)
	{
		xpos = 4 * 64 + xplay;
		ypos = yplay;
		tank->xpos = xpos;
		tank->ypos = ypos;
		tank->way = 0;
	}
	else if (tank == player2)
	{
		xpos = 8 * 64 + xplay;
		ypos = yplay;
		tank->xpos = xpos;
		tank->ypos = ypos;
		tank->way = 0;
	}
	else
	{
		tank->way = 1;
		if (spawnPoint == 1)
		{
			xpos = xplay;
			ypos = 12 * 64 + yplay;
			spawnPoint++;
		}
		else if (spawnPoint == 2)
		{
			xpos = 6 * 64 + xplay;
			ypos = 12 * 64 + yplay;
			spawnPoint++;
		}
		else if (spawnPoint == 3)
		{
			xpos = 12 * 64 + xplay;
			ypos = 12 * 64 + yplay;
			spawnPoint = 1;
		}
		tank->xpos = xpos;
		tank->ypos = ypos;
	}
	if (!tank->spawn.time_since_epoch().count())
		tank->spawn = std::chrono::high_resolution_clock::now();
	else if (std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - tank->spawn).count() > respTime
		&& !tank->spawnSprite && !tank->spawned)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (collision[(int)((tank->xpos - xplay) / 16) + i][(int)((tank->ypos - yplay) / 16) + j] == 0)
					collision[(int)((tank->xpos - xplay) / 16) + i][(int)((tank->ypos - yplay) / 16) + j] = tank->enemyNumber;
				else
				{
					for (int i = 0; i < 4; i++)
						for (int j = 0; j < 4; j++)
							if (collision[(int)((tank->xpos - xplay) / 16) + i][(int)((tank->ypos - yplay) / 16) + j] == tank->enemyNumber)
								collision[(int)((tank->xpos - xplay) / 16) + i][(int)((tank->ypos - yplay) / 16) + j] = 0;
					return false;
				}
		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(13);
		animFrames.pushBack(SpriteFrame::create("spawn3.png", Rect(0, 0, 16 * 4, 16 * 4)));
		animFrames.pushBack(SpriteFrame::create("spawn2.png", Rect(0, 0, 16 * 4, 16 * 4)));
		animFrames.pushBack(SpriteFrame::create("spawn1.png", Rect(0, 0, 16 * 4, 16 * 4)));
		animFrames.pushBack(SpriteFrame::create("spawn2.png", Rect(0, 0, 16 * 4, 16 * 4)));
		animFrames.pushBack(SpriteFrame::create("spawn3.png", Rect(0, 0, 16 * 4, 16 * 4)));
		animFrames.pushBack(SpriteFrame::create("spawn4.png", Rect(0, 0, 16 * 4, 16 * 4)));
		animFrames.pushBack(SpriteFrame::create("spawn3.png", Rect(0, 0, 16 * 4, 16 * 4)));
		animFrames.pushBack(SpriteFrame::create("spawn2.png", Rect(0, 0, 16 * 4, 16 * 4)));
		animFrames.pushBack(SpriteFrame::create("spawn1.png", Rect(0, 0, 16 * 4, 16 * 4)));
		animFrames.pushBack(SpriteFrame::create("spawn2.png", Rect(0, 0, 16 * 4, 16 * 4)));
		animFrames.pushBack(SpriteFrame::create("spawn3.png", Rect(0, 0, 16 * 4, 16 * 4)));
		animFrames.pushBack(SpriteFrame::create("spawn4.png", Rect(0, 0, 16 * 4, 16 * 4)));

		Animation *animation = Animation::createWithSpriteFrames(animFrames, 0.017 * 10);
		Animate *animate = Animate::create(animation);

		tank->spawnSprite = Sprite::create("spawn4.png");
		setNewBody(tank->spawnSprite, tank->xpos, tank->ypos, this);
		tank->spawnSprite->runAction(animate);
	}
	if (tank->spawnSprite && tank->spawnSprite->getNumberOfRunningActions() == 0)
	{
		tank->spawnSprite->removeFromParent();
		tank->spawnSprite = nullptr;
		tank->spawned = true;
		if (tank == player1 || tank == player2)
			tank->setSprite(Sprite::create(tank->name + "up1.png"));
		else
			tank->setSprite(Sprite::create(tank->name + "down1.png"));
		setNewBody(tank->getSprite(), tank->xpos, tank->ypos, this);
		if (tank != player1 && tank != player2)
		{
			enemiesOut.push_back(tank);
			if (enemiesLeft.size() > 0)
			{
				enemiesLeft[enemiesLeft.size() - 1]->removeFromParent();
				enemiesLeft.pop_back();
			}
		}
		else if (tank == player1)
		{
			player1Lives--;
			if (player1Lives >= 0 && !gameCompleteFlag)
			p1l->setTexture(std::to_string(player1Lives) + ".png");
		}
		else if (tank == player2)
		{
			player2Lives--;
			if (player2Lives >= 0 && !gameCompleteFlag)
				p2l->setTexture(std::to_string(player2Lives) + ".png");
		}
		tank->spawn = std::chrono::high_resolution_clock::now();
		return true;
	}
	return false;
}

void StageScene::AIMovement(Tank *enemy)
{
	int phaseTime = respTime * 8;

	if (std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - enemy->spawn).count() < phaseTime)
	{
		enemy->way = rand() % 4;
	}
	else if (std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - enemy->spawn).count() < phaseTime * 2)
	{
		if (p2)
		{
			if ((player1->spawned && !player2->spawned) || (player1->spawned && enemy->enemyNumber % 2 == 0))
			{
				if (enemy->getSprite()->getPosition().y > player1->getSprite()->getPosition().y)
					enemy->way = 1;
				else if (enemy->getSprite()->getPosition().y < player1->getSprite()->getPosition().y)
					enemy->way = 0;
				else if (enemy->getSprite()->getPosition().x > player1->getSprite()->getPosition().x)
					enemy->way = 2;
				else if (enemy->getSprite()->getPosition().x < player1->getSprite()->getPosition().x)
					enemy->way = 3;
			}
			else if (player2->spawned)
			{
				if (enemy->getSprite()->getPosition().y > player2->getSprite()->getPosition().y)
					enemy->way = 1;
				else if (enemy->getSprite()->getPosition().y < player2->getSprite()->getPosition().y)
					enemy->way = 0;
				else if (enemy->getSprite()->getPosition().x > player2->getSprite()->getPosition().x)
					enemy->way = 2;
				else if (enemy->getSprite()->getPosition().x < player2->getSprite()->getPosition().x)
					enemy->way = 3;
			}
		}
		else if (player1->spawned)
		{
			if (enemy->getSprite()->getPosition().y > player1->getSprite()->getPosition().y)
				enemy->way = 1;
			else if (enemy->getSprite()->getPosition().y < player1->getSprite()->getPosition().y)
				enemy->way = 0;
			else if (enemy->getSprite()->getPosition().x > player1->getSprite()->getPosition().x)
				enemy->way = 2;
			else if (enemy->getSprite()->getPosition().x < player1->getSprite()->getPosition().x)
				enemy->way = 3;
		}
	}
	else
	{
		if (enemy->getSprite()->getPosition().y > base->getPosition().y)
			enemy->way = 1;
		else if (enemy->getSprite()->getPosition().x > base->getPosition().x)
			enemy->way = 2;
		else if (enemy->getSprite()->getPosition().x < base->getPosition().x)
			enemy->way = 3;
	}
}

void StageScene::moveEnemies(float delta)
{
	if (enemies.size() > 0)
	if (enemiesOut.size() == 0)
	{
		if (spawnTank(enemies[0], delta))
			enemies.erase(enemies.begin());
	}
	else if (enemiesOut.size() < 6 && enemiesOut[enemiesOut.size() - 1]->spawned)
	{
		if (spawnTank(enemies[0], delta))
			enemies.erase(enemies.begin());
	}
	for (int i = 0; i < enemiesOut.size(); i++)
	{
		if (enemiesOut[i]->getSprite()->getNumberOfRunningActions() == 0)
		{
			if ((int)(enemiesOut[i]->getSprite()->getPosition().x - xplay) % 32 == 0 && (int)(enemiesOut[i]->getSprite()->getPosition().y - yplay) % 32 == 0 && rand() % 16 == 0)
			{
				AIMovement(enemiesOut[i]);
			}
			else if (((isColliding(enemiesOut[i]->way, enemiesOut[i]->getSprite()))) && rand() % 4 == 0)
			{
				if ((int)(enemiesOut[i]->getSprite()->getPosition().x - xplay) % 32 != 0 || (int)(enemiesOut[i]->getSprite()->getPosition().y - yplay) % 32 != 0)
				{
					if (enemiesOut[i]->way == 0)
						enemiesOut[i]->way = 1;
					else if (enemiesOut[i]->way == 1)
						enemiesOut[i]->way = 0;
					else if (enemiesOut[i]->way == 2)
						enemiesOut[i]->way = 3;
					else if (enemiesOut[i]->way == 3)
						enemiesOut[i]->way = 2;
				}
				else
				{
					if (rand() % 2 == 0)
						AIMovement(enemiesOut[i]);
					else
					{
						int random = rand() % 2;
						if (enemiesOut[i]->way == 0 && random == 0)
							enemiesOut[i]->way = 3;
						else if (enemiesOut[i]->way == 0 && random == 1)
							enemiesOut[i]->way = 2;
						else if (enemiesOut[i]->way == 1 && random == 0)
							enemiesOut[i]->way = 2;
						else if (enemiesOut[i]->way == 1 && random == 1)
							enemiesOut[i]->way = 3;
						else if (enemiesOut[i]->way == 2 && random == 0)
							enemiesOut[i]->way = 0;
						else if (enemiesOut[i]->way == 2 && random == 1)
							enemiesOut[i]->way = 1;
						else if (enemiesOut[i]->way == 3 && random == 0)
							enemiesOut[i]->way = 1;
						else if (enemiesOut[i]->way == 3 && random == 1)
							enemiesOut[i]->way = 0;
					}
				}
			}
		}
		std::string move = "";
		if (enemiesOut[i]->way == 0)
			move = "up";
		else if (enemiesOut[i]->way == 1)
			move = "down";
		else if (enemiesOut[i]->way == 2)
			move = "left";
		else if (enemiesOut[i]->way == 3)
			move = "right";
		if (rand() % 32 == 0 && !enemiesOut[i]->bullet &&
			std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::high_resolution_clock::now() - enemiesOut[i]->lastShoot).count() > 500)
			shoot(enemiesOut[i], delta);
		moveTank(enemiesOut[i], move, delta);
	}
}

void StageScene::debugCollision()
{
	for (int i = 0; i < 52; i++)
		for (int j = 0; j < 52; j++)
			if (collision[i][j] != 0 && !collis[i][j])
			{
				Sprite *coll = Sprite::create("collision.png");
				this->addChild(coll);
				coll->setPosition(xplay + i * 16, yplay + j * 16);
				coll->setAnchorPoint(Vec2(0.0, 0.0));
				collis[i][j] = coll;
			}
			else if (collision[i][j] == 0 && collis[i][j])
			{
				if (collis[i][j]->getPosition().x)
				collis[i][j]->removeFromParent();
				collis[i][j] = nullptr;
			}
}

void StageScene::update(float delta)
{
	if ((overFlag || gameCompleteFlag) && std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - initTime).count() > 1000)
	{
		this->pauseSchedulerAndActions();
		Sleep(1000); 
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5, MenuScene::createScene(), Color3B(255, 255, 255)));
	}
	if (completeFlag)
	{
		this->pauseSchedulerAndActions();
		Sleep(1000);
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5, StageScene::createScene(p2, player1Lives, player2Lives, level + 1), Color3B(255, 255, 255)));
	}
	if (!mapBuilt)
		buildMap(this);
	if (enemies.size() + enemiesOut.size() == 0 && !gameCompleteFlag)
		stageComplete();
	Node::update(delta);
	if (!gameCompleteFlag)
	{
		movePlayers(delta);
		moveEnemies(delta);
		bulletCollision();
		removeExploded();
	}
	//debugCollision();
}
std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> StageScene::keys;