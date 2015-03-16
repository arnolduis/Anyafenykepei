#pragma once
 
#include "cocos2d.h"

USING_NS_CC;

class CollisionPath {
public:
	std::list<Vec2> points;
	
	CollisionPath(Vec2[] points);
	float MinimumX (){ return points.begin().x; }
	float MaximumX (){ return points.end().x; }
	float InterpolateY(float x);
	static float InterpolateY(float x, Vec2 p0, Vec2 p1);
};