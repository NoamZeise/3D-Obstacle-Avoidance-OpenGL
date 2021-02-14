#include "main.h"

//window SETTINGS
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

//timing values
double deltaTime = 0.0;
double lastTime = 0.0;

//camera values
float fov = 55.0f;
float camPitch = -32.0f;
float camYaw = 0.0f;
glm::vec3 cameraPos = glm::vec3(-3.5f, 4.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -4.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

//cameraControl
float lastMouseX = 400, lastMouseY = 300;

//colours
glm::vec3 clearScreenColour = glm::vec3(0.1f, 0.2f, 0.5f);

//shader
Shader* texShader;
Shader* blackLineShader;
void inputHandler(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void drawObject(GameObject* gObj, Shader* shader)
{
	glm::mat4 model = gObj->getModelMatrix();
	shader->setMat4("model", model);
	shader->setVec4("shapeColour", gObj->getColour());
	shader->setFloat("scale", gObj->getScale());
	glBindVertexArray(gObj->VAO);
	glDrawArrays(GL_TRIANGLES, 0, gObj->vertexCount);
}

void drawObject(GameObject* gObj, Shader* shader, float extraScale)
{
	glm::mat4 model = gObj->getModelMatrix();
	shader->setMat4("model", model);
	shader->setVec4("shapeColour", gObj->getColour());
	shader->setFloat("scale", gObj->getScale() * extraScale);
	glBindVertexArray(gObj->VAO);
	glDrawArrays(GL_TRIANGLES, 0, gObj->vertexCount);
}

void setupShader(Shader* shader)
{
	shader->Use();
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
}


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "window could not be created\n" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR,  GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, mouse_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD could not be loaded :(" << std::endl;
		return -1;
	}
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	texShader = new Shader("vShader.vert", "fShader.frag");
	blackLineShader = new Shader("vShader.vert", "linefShader.frag");

	float cubeVerticies[] = {     //verticies of a cube
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f,  
	0.5f,  0.5f, -0.5f, 
	-0.5f,  0.5f, -0.5f,  
	-0.5f, -0.5f, -0.5f,  

	-0.5f, -0.5f,  0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f,  
	-0.5f, -0.5f,  0.5f,  

	-0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f, -0.5f,  
	-0.5f, -0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f,  
	-0.5f, -0.5f,  0.5f, 
	-0.5f,  0.5f,  0.5f,  

	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  

	-0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f, -0.5f,  0.5f,  
	-0.5f, -0.5f,  0.5f,  
	-0.5f, -0.5f, -0.5f,  

	-0.5f,  0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f,  
	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f, -0.5f, 
		}; //cube
	Shape cube(cubeVerticies);
	Player player(cube.VAO, cube.count, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(0.3f, 0.6f, 0.2f, 1.0f), 1.0f, cube.size);
	Obstacle obs(cube.VAO, cube.count, glm::vec3(55.0f, 0.0f, 0.0f), glm::vec4(0.7f, 0.2f, 0.1f, 1.0f), 1.0f, cube.size);

	glm::vec3 direction;
	direction.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
	direction.y = sin(glm::radians(camPitch));
	direction.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
	cameraFront = glm::normalize(direction);


	ObstacleManager obstacleManager(&obs, 10.0f);

	while (!glfwWindowShouldClose(window))
	{
		inputHandler(window);

		player.UpdateObj(window, deltaTime);
		obstacleManager.Update(window, deltaTime);

		if (obstacleManager.rectangleCollide(player.getRect()))
		{
			obstacleManager.Reset();
			player.Reset();
		}

		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		glClearColor(clearScreenColour.r, clearScreenColour.g, clearScreenColour.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		//setup normal shader
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		setupShader(texShader);
		//draw soild

		drawObject(&player, texShader);
		obstacleManager.Draw(texShader);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		setupShader(blackLineShader);
		//draw soild

		drawObject(&player, blackLineShader, 1.01f);
		obstacleManager.Draw(blackLineShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


void inputHandler(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	//move camera
	//change camera position when user presses wasd keys
	float cameraSpeed = 2.5f * deltaTime; // moves at 2.5 units per second
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//left and right movement is normalised so that the camera moves at a consistent speed

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	//note different subtraction orders
	float xOffset = xpos - lastMouseX;
	float yOffset = lastMouseY - ypos;
	lastMouseX = xpos;
	lastMouseY = ypos;

	const float sensitivity = 0.03f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	camYaw += xOffset;
	camPitch += yOffset;
	
	//constrain pitch so user does not rotate up or down too far
	if (camPitch > 89.0f)
		camPitch = 89.0f;
	if (camPitch < -89.0f)
		camPitch = -89.0f;

	
	
}

