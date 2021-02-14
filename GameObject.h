#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shape.h"
#include "Rectangle.h"

class GameObject
{
public:
	unsigned int VAO;
	int vertexCount;
	bool isRemoved = false;


	GameObject(unsigned int vao, int count, glm::vec3 pos, glm::vec4 colr, float scale, float size )
	{
		mPosition = pos;
		Initialposition = pos;
		vertexCount = count;
		mShapeSize = size;
		mColor = colr;
		VAO = vao;
		setScale(scale);
	}

	void UpdateObj(GLFWwindow* window, double deltaTime)
	{

	}

	glm::mat4 getModelMatrix() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, mPosition);
		return model;
	}
	glm::vec4 getColour()
	{
		return mColor;
	}
	void setZPos(float zPosition)
	{
		mPosition.z = zPosition;
	}
	void offsetXPos(float xOffset)
	{
		mPosition.x += xOffset;
	}
	void Reset()
	{
		mPosition = Initialposition;
	}
	Rectangle* getRect()
	{
		mRectangle.setPosition(mPosition.x, mPosition.z);
		return &mRectangle;
	}
	void setScale(float scale)
	{
		mScale = scale;
		mRectangle.setRect(mPosition, mShapeSize * mScale, mShapeSize * mScale);
	}
	float getScale()
	{
		return mScale;
	}
	
protected:
	glm::vec3 mPosition;
	float mScale;
	float mShapeSize;
private:
	glm::vec3 Initialposition;
	glm::vec4 mColor;
	Rectangle mRectangle;
};


#endif