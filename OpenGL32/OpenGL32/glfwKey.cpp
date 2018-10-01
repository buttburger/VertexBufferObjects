#include "Common.h"

extern GLFWwindow*window;

extern glm::vec3 eye, right, target;

extern float deltaTime, speed;
extern float rotX, rotY, rotZ;
extern float modelX, modelY, modelZ;

extern int oldState = GLFW_RELEASE;
extern int dostuff;
extern bool hasloaded;

void keyPress()
{
	//MOVE KEYS
	if(glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS)eye += target * deltaTime * speed;//Move forward
	if(glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS)eye -= target * deltaTime * speed;//Move backward
	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)eye +=  right * deltaTime * speed;//Strafe right
	if(glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS)eye -=  right * deltaTime * speed;//Strafe left
	//EULER X TEST
	if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)rotX += deltaTime * 100;
	if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)rotX -= deltaTime * 100;
	//EULER Y TEST
	if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)rotY += deltaTime * 100;
	if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)rotY -= deltaTime * 100;
	//EULER Z TEST
	if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)rotZ += deltaTime * 100;
	if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)rotZ -= deltaTime * 100;
	//X AXIS
	if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)modelX += deltaTime * 10;
	if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)modelX -= deltaTime * 10;
	//Y AXIS
	if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)modelY += deltaTime * 10;
	if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)modelY -= deltaTime * 10;
	//Z AXIS
	if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)modelZ += deltaTime * 10;
	if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)modelZ -= deltaTime * 10;
	//LOAD MODELS
	if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)dostuff = 1;
	if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)dostuff = 2;
	if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)dostuff = 3;

	int newState = glfwGetKey(window, GLFW_KEY_E);
	if(newState == GLFW_RELEASE && oldState == GLFW_PRESS)
	{
		printf("%d - %d BOOP!\n", oldState, newState);
	}
	oldState = newState;
}