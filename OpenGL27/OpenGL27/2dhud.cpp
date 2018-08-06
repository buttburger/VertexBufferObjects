#define _CRT_SECURE_NO_WARNINGS
#define GLEW_STATIC
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
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

#include <vector>

#include "2dhud.h"
#include "load_shaders.h"
#include "load_textures.h"

GLuint program2DshaderID;
GLuint program2DuniformID;
GLuint hud2dVertexBufferID;
GLuint hud2dUVBufferID;

GLuint Texture1;

extern int width, height;
extern bool hasAlpha;
extern char filename[], filename1[], filename2[];

/*
-0.25f,-0.25f,
0.25f,0.25f,
-0.25f,0.25f,
0.25f,-0.25f,

0.0f,0.0f,
1.0f,1.0f,
0.0f,1.0f,
1.0f,0.0f,

0,1,2,
0,3,1,
*/

float myVT[] =
{
	-0.25f, -0.25f, 0.0f,
	 0.25f,  0.25f, 0.0f,
	-0.25f,  0.25f, 0.0f,

	//-0.25f, -0.25f, 0.0f,
	 0.25f, -0.25f, 0.0f,
	//0.25f,  0.25f, 0.0f,
};

float myUV[] =
{
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	//0.0f, 0.0f,
	1.0f, 0.0f,
	//1.0f, 1.0f,
};

unsigned int ind[] =
{
	0,1,2,
	0,3,1,
};

void init2dhud()
{
	Texture1 = loadPngImage(filename1, width, height, hasAlpha, &textureImage);
	glGenBuffers(1, &hud2dVertexBufferID);
	glGenBuffers(1, &hud2dUVBufferID);

	loadShaders(program2DshaderID, "shaders/vert_2dhud.glsl", "shaders/frag_2dhud.glsl");
	program2DuniformID = glGetUniformLocation(program2DuniformID, "myTextureSampler");
}
void draw2dhud()
{
	glUseProgram(program2DshaderID);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(myVT), myVT, GL_STATIC_DRAW);
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(myUV), myUV, GL_STATIC_DRAW);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture1);

	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(program2DuniformID, 0);

	//1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glDrawElements(GL_TRIANGLES, sizeof(ind), GL_UNSIGNED_INT, (void*)0);
	//glDrawArrays(GL_TRIANGLES, 0, sizeof(myVT));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}