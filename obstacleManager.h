#pragma once
#ifndef OBSTACLE_MANAGER_H
#define OBSTACLE_MANAGER_H

#include <vector>
#include <random>

#include "GameObject.h"
#include "shader.h"

class ObstacleManager
{
public:
	float scale = 0.5f;
	int currentScore = 0;
	int highScore = 0;
	ObstacleManager(Obstacle* templateObj, float fieldWidth)
	{
		mTemplateObject = templateObj;
		std::random_device rd{};
		std::mt19937 engine{ rd() };
		mRandomGen = engine;
		mZSpawnRange = std::uniform_real_distribution<float>(-(fieldWidth / 2), (fieldWidth / 2));
		mScaleRange = std::uniform_real_distribution<float>(0.8f, 3.0f);
		mXOffsetRange = std::uniform_real_distribution<float>(0.0f, 30.0f);
		Reset();
	}

	void Update(GLFWwindow* window, double deltaTime)
	{
		//std::cout << 1 << std::endl;
		for (int i = 0; i < mObstacles.size(); i++)
		{
			mObstacles.at(i)->UpdateObj(window, deltaTime);
			if (mObstacles.at(i)->isRemoved)
			{
				currentScore += 1;
				std::cout << "\ncurrent score: " << currentScore << std::endl;
				std::cout << "highscore:     " << highScore << std::endl;
				mObstacles.erase(mObstacles.begin() + i--);
			}
		}

		//spawn new obstacles
		if (mSpawnTimer >= mSpawnDelay)
		{
			mSpawnNewObsatcle();
			mSpawnNewObsatcle();
			mSpawnTimer = 0;
			if (mSpawnDelay > 0.8f)
				mSpawnDelay -= 0.05f;
			mSpeed += 0.3f;
		}
		mSpawnTimer += deltaTime;
	}

	void Draw(Shader* shader)
	{
		for (int i = 0; i < mObstacles.size(); i++)
		{
			glm::mat4 model = mObstacles.at(i)->getModelMatrix();
			shader->setMat4("model", model);
			shader->setVec4("shapeColour", mObstacles.at(i)->getColour());
			shader->setFloat("scale", mObstacles.at(i)->getScale());
			glBindVertexArray(mObstacles.at(i)->VAO);
			glDrawArrays(GL_TRIANGLES, 0, mObstacles.at(i)->vertexCount);
		}
	}

	void Reset()
	{
		if (currentScore > highScore)
			highScore = currentScore;
		currentScore = 0;
		mSpeed = 10.0f;
		mObstacles.clear();
		mSpawnDelay = 3.0f;
		mSpawnTimer = mSpawnDelay;
	}

	bool rectangleCollide(Rectangle* rect)
	{
		for (int i = 0; i < mObstacles.size(); i++)
			if (Rectangle::Colliding(rect, mObstacles.at(i)->getRect()))
				return true;
		return false;
	}

private:
	Obstacle* mTemplateObject;
	std::vector<Obstacle*> mObstacles;
	std::mt19937 mRandomGen;
	std::uniform_real_distribution<float> mZSpawnRange;
	std::uniform_real_distribution<float> mXOffsetRange;
	std::uniform_real_distribution<float> mScaleRange;
	double mSpawnTimer;
	double mSpawnDelay;
	float mSpeed;
	float mPreviousZPos = 0.0f;

	void mSpawnNewObsatcle()
	{
		Obstacle* newObj = new Obstacle(*mTemplateObject);
		newObj->setScale(mScaleRange(mRandomGen));
		float zPos = mZSpawnRange(mRandomGen);
		while (abs(zPos - mPreviousZPos) < 3.0f)
			zPos = mZSpawnRange(mRandomGen);
		mPreviousZPos = zPos;
		newObj->setZPos(zPos);
		newObj->speed = mSpeed;
		newObj->offsetXPos(mXOffsetRange(mRandomGen));
		mObstacles.push_back(newObj);
	}
};


#endif