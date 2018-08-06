#define _CRT_SECURE_NO_WARNINGS
#define GLEW_STATIC
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#pragma comment(lib, "libfbxsdk-mt.lib")
#pragma comment(lib, "libpng16.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "freeglut_static.lib")
#pragma comment(lib, "glfw3.lib")
#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <freeglut.h>
#include <glfw3.h>
// Include GLM
#include <glm\glm.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include <Windows.h>
#include <conio.h>

#include <fbxsdk.h>
#include <png.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <math.h>

#include "Camera.h"
#include "load_shaders.h"
#include "load_textures.h"
#include "quaternion_utils.h"

#include "objloader.h"
#include "vboindexer.h"
#include "2dhud.h"

//#define my_sizeof(type) ((char *)(&type+1)-(char*)(&type))
//int size = my_sizeof(myVertices)/my_sizeof(myVertices[0]);

using namespace glm;
GLFWwindow*window;
int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
GLuint programID;

vec3 gOrientation1;
vec3 gPosition1(-1.5f, 0.0f, 0.0f);
vec3 gPosition2( 1.5f, 0.0f, 0.0f);
quat gOrientation2;

float eyeX = 0, eyeY = 0, eyeZ = 2;//Initial position : on +Z
glm::vec3 eye = glm::vec3(eyeX, eyeY, eyeZ);
float modelX = 0, modelY = 0, modelZ = 0;
float horizontalAngle = 3.14f; //Initial horizontal angle : toward -Z
float verticalAngle = 0.0f; //Initial vertical angle : none
float speed = 3.0f;//3 units/second
float mouseSpeed = 0.005f;


std::vector<glm::vec3> vectorvertices;
std::vector<glm::vec2> vectoruvs;
std::vector<glm::vec3> vectornormals; // Won't be used at the moment.
//int verticevectorsize;
std::vector<unsigned short> indices;
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::vec2> indexed_uvs;
std::vector<glm::vec3> indexed_normals;

int width, height;
bool hasAlpha;
char filename[] = "texture.png", filename1[] = "texture1.png", filename2[] = "texture2.png";

void wait(double seconds)
{
	double endwait; //clock_t
	endwait = clock() + seconds * CLOCKS_PER_SEC;
	while(clock() < endwait){}
}
int main()
{
	if(!glfwInit()) //Initialise GLFW
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tutorial 09 - VBO Indexing", NULL, NULL);
	if(window == NULL)
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);//Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//GLFW_CURSOR_NORMAL);
	//Hide the mouse and enable unlimited movement
	glfwPollEvents();
	glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2); //Set the mouse at the center of the screen
	//Initialize GLEW
	glewExperimental = true; //Needed for core profile
	if(glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); //Dark blue background
	glEnable(GL_DEPTH_TEST); //Enable depth test
	glDepthFunc(GL_LESS); //Accept fragment if it closer to the camera than the former one
	//glEnable(GL_CULL_FACE); //Cull triangles which normal is not towards the camera

	// Create and compile our GLSL program from the shaders
	loadShaders(programID, "shaders/vert_shader.glsl", "shaders/frag_shader.glsl");

	// Load the texture
	GLuint Texture = //loadBMP_custom("test.bmp");
		loadPngImage(filename, width, height, hasAlpha, &textureImage);
		//LoadBMP("test.bmp");
		//loadDDS("uvmap.DDS");

	init2dhud();
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTexture");
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	bool res = loadOBJ("cube.obj", vectorvertices, vectoruvs, vectornormals);
	indexVBO(vectorvertices, vectoruvs, vectornormals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	/*
	for(int a = 0; a < 10; a++)
	{
		printf("indz: %d\n", indices[a]);
		printf("indV X: %f Y: %f Z: %f\n", indexed_vertices[a].x, indexed_vertices[a].y, indexed_vertices[a].z);
		printf("indUV U: %f V: %f\n", indexed_uvs[a].x, indexed_uvs[a].y);
	}
	_getch();
	*/

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	/*
	for(int asdf = 0; asdf < 10; asdf++)
	{
		printf("iVt X: %f Y: %f Z: %f\n", indexed_vertices[asdf].x, indexed_vertices[asdf].y, indexed_vertices[asdf].z);
		printf("iUV X: %f Y: %f Z: %f\n", indexed_uvs[asdf].x, indexed_uvs[asdf].y);
		printf("idz: %d\n", indices[asdf]);
	}

	_getch();
	*/

	glUseProgram(programID);
	// Get a handle for our "LightPosition" uniform
	//GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	
	int frames = 0;
	int nbFrames = 0;
	double currentTime;
	float deltaTime;
	double limitFPS = 1.0/35.0;
	double lastTime = glfwGetTime();
	double timer = lastTime;
	float rotX = 0, rotY = 0, rotZ = 0;

	for(;;)
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);

		double xpos, ypos; //Get mouse position
		glfwGetCursorPos(window, &xpos, &ypos);
		// Reset mouse position for next frame
		glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
		// Compute new orientation
		horizontalAngle += mouseSpeed * float(SCREEN_WIDTH /2 - xpos);
		verticalAngle   += mouseSpeed * float(SCREEN_HEIGHT/2 - ypos);
		//printf("hA: %f vA: %f xPos: %f yPos: %f\n", horizontalAngle, verticalAngle, xpos, ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 target(cos(verticalAngle) * sin(horizontalAngle), 
						sin(verticalAngle),
						cos(verticalAngle) * cos(horizontalAngle));
		// Right vector
		glm::vec3 right = glm::vec3(sin(horizontalAngle - 3.14f/2.0f), 
									0,
									cos(horizontalAngle - 3.14f/2.0f));
		glm::vec3 up = glm::cross(right, target); // Up vector

		if(glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
			eye += target * deltaTime * speed;} // Move forward
		if(glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
			eye -= target * deltaTime * speed;} // Move backward
		if(glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
			eye += right * deltaTime * speed;} // Strafe right
		if(glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
			eye -= right * deltaTime * speed;} // Strafe left

		//EULER X TEST
		if(glfwGetKey( window, GLFW_KEY_1 ) == GLFW_PRESS){
			rotX += deltaTime * 100;}
		if(glfwGetKey( window, GLFW_KEY_2 ) == GLFW_PRESS){
			rotX -= deltaTime * 100;}
		//EULER Y TEST
		if(glfwGetKey( window, GLFW_KEY_3 ) == GLFW_PRESS){
			rotY += deltaTime * 100;}
		if(glfwGetKey( window, GLFW_KEY_4 ) == GLFW_PRESS){
			rotY -= deltaTime * 100;}
		//EULER Z TEST
		if(glfwGetKey( window, GLFW_KEY_5 ) == GLFW_PRESS){
			rotZ += deltaTime * 100;}
		if(glfwGetKey( window, GLFW_KEY_6 ) == GLFW_PRESS){
			rotZ -= deltaTime * 100;}

		glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 1000.0f);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 view = LookAt(eye, eye+target, up);
		
		vec3 desiredDir = glm::vec3(rotX, rotY, rotZ); //gPosition1-gPosition2;
		vec3 desiredUp = vec3(0.0f, 1.0f, 0.0f); // +Y

		//EULER TO QUATERNION
		// Compute the desired orientation
		quat targetOrientation = normalize(qLookAt(desiredDir, desiredUp));
		// And interpolate
		gOrientation2 = RotateTowards(gOrientation2, targetOrientation, 1.0f*deltaTime);
		//printf("ROT X: %f Y: %f Z: %f W: %f\n", gOrientation2[0], gOrientation2[1], gOrientation2[2], gOrientation2[3]);
		glm::mat4 RotationMatrix = mat4_cast(gOrientation2);

		if(glfwGetKey( window, GLFW_KEY_I ) == GLFW_PRESS){
			modelX += deltaTime * 10;}
		if(glfwGetKey( window, GLFW_KEY_J ) == GLFW_PRESS){
			modelX -= deltaTime * 10;}

		if(glfwGetKey( window, GLFW_KEY_O ) == GLFW_PRESS){
			modelY += deltaTime * 10;}
		if(glfwGetKey( window, GLFW_KEY_K ) == GLFW_PRESS){
			modelY -= deltaTime * 10;}

		if(glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS){
			modelZ += deltaTime * 10;}
		if(glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS){
			modelZ -= deltaTime * 10;}

		//glm::mat4 RotationMatrix = eulerAngleYXZ(rotX, rotY, rotZ);
		glm::mat4 TranslationMatrix = translate(mat4(), glm::vec3(modelX, modelY, modelZ));
		glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));//scaling[0], scaling[1], scaling[2]));
		glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

		glm::mat4 MVP = projection * view * ModelMatrix;
		//Send our transformation to the currently bound shader in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		//glm::vec3 lightPos = glm::vec3(4,4,-4);
		//glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glDrawElements(GL_TRIANGLES, vectorvertices.size() * sizeof(glm::vec3), GL_UNSIGNED_SHORT, (void*)0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);


		draw2dhud();


		glfwSwapInterval(0); //UNLOCKFPS?
		glfwSwapBuffers(window);
		glfwPollEvents();

		frames++;
		lastTime = currentTime;
		currentTime = glfwGetTime();
		deltaTime = (float)(currentTime - lastTime);
		wait(limitFPS - deltaTime);
		if(currentTime - timer > 1.0) //Reset after one second
		{
			timer++;
			//printf("FPS: %d - %fms\n", frames, 1000.0/double(frames));
			frames = 0;
		}
		if((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && glfwWindowShouldClose(window) == 0))break;
	}
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	//glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();//Close OpenGL window and terminate GLFW
	return 0;
}