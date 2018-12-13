#pragma once

#include "cocos2d.h"
#include "tank.h"
#include "bullet.h"
#include "Menu.h"
#include <vector>
#include <fstream>

class StageScene : public cocos2d::LayerColor
{
public:
	bool pause = false;
	bool overFlag = false;
	bool completeFlag = false;
	bool gameCompleteFlag = false;
	std::chrono::high_resolution_clock::time_point initTime;
	cocos2d::Label *pauseLabel;
	int xplay = 84;
	int yplay = 64;
	int respTime;
	int level;
	Tank *player1;
	cocos2d::Sprite *p1l;
	int player1Lives;
	bool p2;
	Tank *player2;
	int player2Lives;
	cocos2d::Sprite *p2l;
	bool mapBuilt = false;
	int spawnPoint = 2;
	std::vector<Bullet*> bullets;
	std::vector<cocos2d::Sprite*> explosions;
	std::vector<std::vector<Node*>> walls;
	cocos2d::Sprite *base;
	std::vector<Tank*> enemies;
	std::vector<Tank*> enemiesOut;
	std::vector<Node*> enemiesLeft;
	byte collision[52][52] = { {0,}, };
	cocos2d::Sprite *collis[52][52];
	static cocos2d::Scene* createScene(bool p2, int p1l, int p2l, int level);
	void setNewBody(Node *sprite, int xpos, int ypos, StageScene *set);
	void makeWallPiece(int xpos, int ypos, StageScene *set, bool armor);
	void makeWall(int xpos, int ypos, StageScene *set, bool armor);
	void makeBase();
	void buildMap(StageScene *set);
	virtual bool init() override;
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
	bool isColliding(int dir, cocos2d::Sprite *moving);
	bool isBulletColliding(int dir, cocos2d::Sprite *moving, int bulletNumber);
	void moveTank(Tank *tank, std::string move, float delta);
	void shoot(Tank *tank, float delta);
	void removeExploded();
	void itemExplode(cocos2d::Sprite *explode, bool type, int xpos, int ypos);
	bool bulletDestroy(Bullet *bullet);
	void bulletCollision();
	void movePlayers(float delta);
	void gameOver();
	void stageComplete();
	void gameComplete();
	bool spawnTank(Tank *tank, float delta);
	void AIMovement(Tank *enemy);
	void moveEnemies(float delta);
	void debugCollision();
	virtual void update(float delta) override;
	CREATE_FUNC(StageScene);

private:
	static std::map<cocos2d::EventKeyboard::KeyCode,
		std::chrono::high_resolution_clock::time_point> keys;
};