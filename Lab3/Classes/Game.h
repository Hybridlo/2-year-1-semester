#pragma once

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{

public:
	virtual bool init();

	static cocos2d::Scene *createScene(int diff);
	int diff;

	cocos2d::Sprite *great;
	cocos2d::Sprite *good;
	cocos2d::Sprite *ok;
	cocos2d::Sprite *arrow;
	bool arrowPressed = false;
	int arrowWay;
	std::chrono::high_resolution_clock::time_point lastArrow;
	cocos2d::Sprite *lastArrowScore;
	int score = 0;
	cocos2d::Label *scoreLabel;


	void startGameCallback(cocos2d::Event* event);
	void highlight(cocos2d::Event* event);

	virtual void update(float delta) override;
	CREATE_FUNC(GameScene);

};