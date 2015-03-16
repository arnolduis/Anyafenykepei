#pragma once
 
#include "cocos2d.h"
#include "collisionPath.h"

USING_NS_CC;

class Player : public Actor {
public:
	const float WALK_VELOCITY = 200f;
	const float JUMP_VELOCITY = -500.0f;

	Actor(Vec2 position, Vec2 dimensions);
	virtual ~Actor(void);
	virtual void update(float delta, std::vector<CollisionPath> collisionPaths);
};