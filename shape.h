#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Shape
{
public:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int count;
	float size = 0;

	template <size_t N>
	Shape(float (&verticies)[N]) //take verticies and make shape, in format pos.xyz
	{
		count = N / 3;
		size = abs(verticies[0] * 2);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
		glEnableVertexAttribArray(0);
	}

	~Shape()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
};



#endif
