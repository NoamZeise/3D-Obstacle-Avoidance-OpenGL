#pragma once
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <glm/glm.hpp>

class Rectangle
{
public:
	Rectangle(glm::vec3 Position, float width, float height)
	{
		w = width;
		h = height;
		setPosition(Position.x, Position.z);
	}
	Rectangle()
	{
		w = 0;
		h = 0;
		x = 0;
		z = 0;
	}

	static bool Colliding(Rectangle* rect1, Rectangle* rect2)
	{
		return 
			(rect1->x < rect2->x + rect2->w) &&
			(rect1->x + rect1->w > rect2->x) &&
			(rect1->z < rect2->z + rect2->h) &&
			(rect1->z + rect1->h > rect2->z);
	}
	void setRect(float x, float z, float width, float height)
	{
		w = width;
		h = height;
		setPosition(x, z);
	}
	void setRect(glm::vec3 Position, float width, float height)
	{
		w = width;
		h = height;
		setPosition(Position.x, Position.z);
	}
	void setPosition(float xPos, float zPos)
	{
		x = xPos -(w / 2);
		z = zPos -(h / 2);
	}
//private:
	float x;
	float z;
	float w;
	float h;
};


#endif