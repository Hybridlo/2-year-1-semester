#pragma once

class Bullet
{
public:
	int way = 0;
	Tank *owner;
	cocos2d::Sprite *explosion;
	cocos2d::Sprite *getSprite() { return sprite; };
	void setSprite(cocos2d::Sprite *newSprite) { sprite = newSprite; };
private:
	cocos2d::Sprite *sprite;
};