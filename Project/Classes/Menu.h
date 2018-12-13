#pragma once

#include "cocos2d.h"

class MenuScene : public cocos2d::Layer 
{

public:
	virtual bool init();

	static cocos2d::Scene *createScene();

	cocos2d::Label *startGameLabel1;
	cocos2d::Label *startGameLabel2;

	void startGameCallback(cocos2d::Event* event);
	void highlight(cocos2d::Event* event);

	CREATE_FUNC(MenuScene);

};