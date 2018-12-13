#pragma once

class Tank
{
public:
	bool movingUp = false;
	bool movingDown = false;
	bool movingLeft = false;
	bool movingRight = false;
	bool bullet = false;
	bool spawned = false;
	std::chrono::high_resolution_clock::time_point playerLock;
	cocos2d::Sprite *explosion;
	std::string name;
	std::chrono::high_resolution_clock::time_point lastShoot;
	std::chrono::high_resolution_clock::time_point spawn;
	int speed;
	int bulletSpeed = 1;
	int way = -1;
	int enemyNumber;
	int xpos; //for moving and spawn
	int ypos;
	cocos2d::Sprite *spawnSprite = nullptr;
	cocos2d::Sprite *getSprite() { return sprite; };
	void setSprite(cocos2d::Sprite *newSprite) { sprite = newSprite; };
private:
	cocos2d::Sprite *sprite;
};
