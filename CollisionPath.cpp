#include "CollisionPath.h"

float MinimumX { get { return points[0].X; } }
float MaximumX { get { return points[points.Count - 1].X; } }

void CollisionPath(Vector2[] points)
{
	points = new List<Vec2>();
	points.AddRange(points);
}

float InterpolateY(float x)
{
	for (int i = 0; i < points.Count - 1; i++)
	{
		if (x < points[i + 1].X)
		{
			return InterpolateY(x, points[i], points[i + 1]);
		}
	}
	return InterpolateY(x, points[points.Count - 2], points[points.Count - 1]); ;
}

static float InterpolateY(float x, Vector2 p0, Vector2 p1)
{
	return (x - p0.X) * ((p1.Y - p0.Y) / (p1.X - p0.X)) + p0.Y;
}