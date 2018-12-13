#pragma once

#include "cocos2d.h"

class MenuScene : public cocos2d::Layer
{

public:
	virtual bool init();

	static cocos2d::Scene *createScene();

	cocos2d::Label *chooseDifficulty;
	cocos2d::Label *diff[10];


	void startGameCallback(cocos2d::Event* event);
	void highlight(cocos2d::Event* event);

	CREATE_FUNC(MenuScene);

};