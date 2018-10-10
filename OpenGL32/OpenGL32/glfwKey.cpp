#include "Common.h"

extern GLFWwindow*window;

extern glm::vec3 eye, right, target;

extern float deltaTime, speed;
extern float rotX, rotY, rotZ;
extern float modelX, modelY, modelZ;

extern bool hasloaded;
extern int dostuff;
int keyB = 0, keyN = 0, keyM = 0;

bool keydown(int key)
{
	if(glfwGetKey(window, key) == GLFW_PRESS)return 1;
}

int keylift(int key, int oldState, int function)
{
	//printf("oldState: %d\n", oldState);
	int newState = glfwGetKey(window, key);
	if(newState == GLFW_RELEASE && oldState == GLFW_PRESS)
	{
		printf("%d - %d - %d\n", oldState, newState, key);
		dostuff = function;
	}
	oldState = newState;
	return oldState;
}

void keyPress()
{
	//MOVE KEYS
	if(keydown(GLFW_KEY_UP))   eye += target * deltaTime * speed;//Move forward
	if(keydown(GLFW_KEY_DOWN)) eye -= target * deltaTime * speed;//Move backward
	if(keydown(GLFW_KEY_RIGHT))eye +=  right * deltaTime * speed;//Strafe right
	if(keydown(GLFW_KEY_LEFT)) eye -=  right * deltaTime * speed;//Strafe left
	//EULER X TEST
	if(keydown(GLFW_KEY_1))rotX += deltaTime * 100;
	if(keydown(GLFW_KEY_2))rotX -= deltaTime * 100;
	//EULER Y TEST
	if(keydown(GLFW_KEY_3))rotY += deltaTime * 100;
	if(keydown(GLFW_KEY_4))rotY -= deltaTime * 100;
	//EULER Z TEST
	if(keydown(GLFW_KEY_5))rotZ += deltaTime * 100;
	if(keydown(GLFW_KEY_6))rotZ -= deltaTime * 100;
	//X AXIS
	if(keydown(GLFW_KEY_I))modelX += deltaTime * 10;
	if(keydown(GLFW_KEY_J))modelX -= deltaTime * 10;
	//Y AXIS
	if(keydown(GLFW_KEY_O))modelY += deltaTime * 10;
	if(keydown(GLFW_KEY_K))modelY -= deltaTime * 10;
	//Z AXIS
	if(keydown(GLFW_KEY_P))modelZ += deltaTime * 10;
	if(keydown(GLFW_KEY_L))modelZ -= deltaTime * 10;
	//LOAD MODELS	
	keyB = keylift(GLFW_KEY_B, keyB, 1);
	keyN = keylift(GLFW_KEY_N, keyN, 2);
	keyM = keylift(GLFW_KEY_M, keyM, 3);
}
