#pragma once
 
#include "cocos2d.h"
#include "collisionPath.h"

USING_NS_CC;

class Actor : public Object {
public:
	Vec2 position;
	Vec2 velocity;
	Vec2 dimensions;
	CollisionPath attachedPath;
	const float GRAVITY = 20.0f;

	virtual void update(float delta, std::vector<CollisionPath> collisionPaths);
	Actor(Vec2 position, Vec2 dimensions);
	virtual ~Actor(void);
};