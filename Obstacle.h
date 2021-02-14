#pragma once
#ifndef OBSATCLE_H
#define OBSTACLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"

class Obstacle : public GameObject
{

public:
	float speed = 8.0f;
	Obstacle(unsigned int vao, int count, glm::vec3 pos, glm::vec4 colr, float scle, float size) : GameObject(vao, count, pos, colr, scle, size)
	{
	}


	void UpdateObj(GLFWwindow* window, double deltaTime)
	{
		mPosition.x -= speed * deltaTime;
		if (mPosition.x <= -4.0f)
			isRemoved = true;
	}
};

#endif
