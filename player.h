#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(unsigned int vao, int count, glm::vec3 pos, glm::vec4 colr, float scle, float size) : GameObject(vao, count, pos, colr, scle, size)
	{
	}


	void UpdateObj(GLFWwindow* window, double deltaTime)
	{
		float speed = mSpeed * deltaTime;
		//if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		//	position.x += speed;
		//if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		//	position.x -= speed;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			mPosition.z -= speed;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			mPosition.z += speed;

		if (mPosition.z < -mLimit)
			mPosition.z = -mLimit;
		if (mPosition.z > mLimit)
			mPosition.z = mLimit;
		
	}
private:
	float mLimit = 4.0f;
	float mSpeed = 7.0f;
};

#endif
