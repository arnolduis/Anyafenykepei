#pragma once

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    PhysicsWorld *sceneWorld;
    virtual void update(float delta) override;
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
	bool wasKeyPressed(cocos2d::EventKeyboard::KeyCode);
	bool isMousePressed(int);
    double keyPressedDuration(cocos2d::EventKeyboard::KeyCode);

    CREATE_FUNC(HelloWorld);


private:
	Sprite *nyuszSprite;
	Sprite *ninjaSprite;

	PhysicsBody *nyuszBody;
	PhysicsBody *slopeBody;

	Animate *animRunningF;
	Animate *animRunningL;
	Animate *animRunningR;

	Camera *camera;
	Vec3 cPos;
	Vec3 cRot;
	Vec3 cFoc;

	Vec2 prevMouseLoc;

	void addMouse();
	void addKeyboard();

    void setupAnimations();
    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; };

    std::map<cocos2d::EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> keys;
    std::map<cocos2d::EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> oldKeys;
    std::map<int, std::chrono::high_resolution_clock::time_point> mouseState;
    Label * label;
};