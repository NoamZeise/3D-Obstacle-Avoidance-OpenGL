#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include "GameObject.h"

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath)
	{
		//load the vetrex and fragment shader to a string from a passed filepath
		std::ifstream vStream(vertexPath), fStream(fragmentPath);
		std::string vShaderStringSource((std::istreambuf_iterator<char>(vStream)), std::istreambuf_iterator<char>()),
			fShaderStringSource((std::istreambuf_iterator<char>(fStream)), std::istreambuf_iterator<char>());
		const char* vShaderSource(vShaderStringSource.c_str());
		const char* fShaderSource(fShaderStringSource.c_str());
		//create the shaders and compile the source code for them
		unsigned int vShader(glCreateShader(GL_VERTEX_SHADER)), fShader(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(vShader, 1, &vShaderSource, NULL);
		glShaderSource(fShader, 1, &fShaderSource, NULL);
		int success;
		glCompileShader(vShader);
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[1024];
			glGetShaderInfoLog(vShader, 1024, NULL, infoLog);
			std::cout << "ERROR compiling vertex shader\n" << infoLog << std::endl;
		}
		glCompileShader(fShader);
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[1024];
			glGetShaderInfoLog(fShader, 1024, NULL, infoLog);
			std::cout << "ERROR compiling fragment shader\n" << infoLog << std::endl;
		}
		//attach shaders to shader program and delete them
		ID = glCreateProgram();
		glAttachShader(ID, vShader);
		glAttachShader(ID, fShader);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[1024];
			glGetProgramInfoLog(ID, 1024, NULL, infoLog);
			std::cout << "ERROR linking shader program\n" << infoLog << std::endl;
		}
		glDeleteShader(vShader);
		glDeleteShader(fShader);
	}

	
	void Use()
	{
		glUseProgram(ID);
	}
	void setMat4(const std::string& name, glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
	}
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	static void ConfigWithGameObject(Shader* shader, GameObject* gObj)
	{
		glm::mat4 matrix = gObj->getModelMatrix();
		shader->setMat4("model", matrix);
		shader->setVec4("shapeColour", gObj->getColour());
		shader->setFloat("scale", gObj->getScale());
	}
	static void ConfigWithGameObject(Shader* shader, GameObject* gObj, float scaleModifier)
	{
		glm::mat4 matrix = gObj->getModelMatrix();
		shader->setMat4("model", matrix);
		shader->setVec4("shapeColour", gObj->getColour());
		shader->setFloat("scale", gObj->getScale() * scaleModifier);
	}

	static void ConfigureShader(Shader* shader, glm::mat4 view, glm::mat4 projection)
	{
		shader->Use();
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
	}
};
#endif