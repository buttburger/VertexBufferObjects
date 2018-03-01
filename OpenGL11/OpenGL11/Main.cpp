#define GLEW_STATIC
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#pragma comment(lib, "libpng16.lib")
#pragma comment (lib, "libglew32.lib")
#pragma comment(lib, "libglew32.lib")
#pragma comment(lib, "freeglut_static.lib")
#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <freeglut.h>
#include <glm\glm.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <png.h>
#include "load_shaders.h"
#include "load_textures.h"
#include "object.h"
using namespace std;

int refreshMS = 15;
GLuint  prog_hdlr, program;
GLint location_attribute_0, location_viewport;
const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

GLfloat f[] =
{
	1,-1,-1,
	1,-1,1,
	-1,-1,1,
	-1,-1,-1,
	1,1,-1,
	1,1,1,
	-1,1,1,
	-1,1,-1,
};
float normals[] =
{
	0.0000, -1.0000, -0.0000,
	0.0000, 1.0000, -0.0000,
	1.0000, 0.0000, 0.0000,
	-0.0000, -0.0000, 1.0000,
	-1.0000, -0.0000, -0.0000,
	0.0000, 0.0000, -1.0000,
};
unsigned indarr[] =
{
	1,3,0,
	7,5,4,
	4,1,0,
	5,2,1,
	2,7,3,
	0,7,4,
	1,2,3,
	7,6,5,
	4,5,1,
	5,6,2,
	2,6,7,
	0,3,7,
};

unsigned int index, color;
float r = 0.0f;
int offset = 0;


int testx, testy, testz;


int wkey=0, skey=0, akey=0, dkey=0;

float lastx, lasty;

float xrot = 0, yrot = 0, zrot = 0;
float camx = 0, camy = 0, camz = 0;

float tx=0, ty=0, tz=0;

int testcrap = 0;

float xtest,ztest;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if(testcrap == 1)
	{
		GLuint test = 3;
		setShaders(test, 1, 2);
		//setShaders(prog_hdlr, 1, 2);
		testcrap = 0;
	}
	if(testcrap == 2)
	{
		GLuint test1 = 6;
		setShaders(test1, 4, 5);
		//setShaders(prog_hdlr, 4, 5);
		testcrap = 0;
	}

	if(wkey == 1)
	{
		float xrotrad, yrotrad, zrotrad;
		xrotrad = (xrot/180*3.141592654f);
		yrotrad = (yrot/180*3.141592654f);
		zrotrad = (zrot/180*3.141592654f);
		camx -= float(sin(zrotrad));
		camy += float(cos(zrotrad));
		//camz -= float(cos(xrotrad));
	}
	if(skey == 1)
	{
		float xrotrad, yrotrad, zrotrad;
		xrotrad = (xrot/180*3.141592654f);
		yrotrad = (yrot/180*3.141592654f);
		zrotrad = (zrot/180*3.141592654f);
		camx += float(sin(zrotrad));
		camy -= float(cos(zrotrad));
		//camz += float(cos(xrotrad));
	}
	if(akey == 1)
	{
		float zrotrad;
		zrotrad = (zrot/180*3.141592654f);
		camx -= float(cos(zrotrad))*(float)0.75;
		camy -= float(sin(zrotrad))*(float)0.75;
	}
	if(dkey == 1)
	{
		float zrotrad;
		zrotrad = (zrot/180*3.141592654f);
		camx += float(cos(zrotrad))*(float)0.75;
		camy += float(sin(zrotrad))*(float)0.75;
	}


	
	//glUseProgram(prog_hdlr);
	GLuint uniformModel = glGetUniformLocation(prog_hdlr, "model");
	GLuint uniformView = glGetUniformLocation(prog_hdlr, "view");
	GLuint uniformProjection = glGetUniformLocation(prog_hdlr, "projection");
	//PROJECTION!
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	//r += 0.1f;
	//glm::vec3 eye = glm::vec3(sin(r*0.33)*5.0f, -5, cos(r*0.33)*5.0f);
	glm::vec3 eye = glm::vec3(camx, camy - 3, camz);	
	glm::vec3 target = glm::vec3(camx + tx, camy + ty, camz + tz);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::mat4 view = glm::lookAt(eye, target, up);
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(testx, testy, testz));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


	glBindBuffer(GL_ARRAY_BUFFER, program);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	glNormalPointer(GL_FLOAT, 0, (GLvoid*)(sizeof(f)));
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	//glEnableVertexAttribArray(index);
	//glEnableVertexAttribArray(color);

	//glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
	//glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glDisableVertexAttribArray(index);
	//glDisableVertexAttribArray(color);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glutSwapBuffers();
}
void keyDown(unsigned char key, int x, int y)
{
	if(key == 'w')wkey = 1;
	if(key == 's')skey = 1;
	if(key == 'a')akey = 1;
	if(key == 'd')dkey = 1;

	if(key == 27)exit(0);

	if(key == 'i')testx++;
	if(key == 'j')testx--;

	if(key == 'o')testy++;
	if(key == 'k')testy--;

	if(key == 'p')testz++;
	if(key == 'l')testz--;

	if(key == 't')camx++;
	if(key == 'f')camx--;

	if(key == 'y')camy++;
	if(key == 'g')camy--;

	if(key == 'u')camz++;
	if(key == 'h')camz--;

	if(key == 'c')tx++;
	if(key == 'x')tx--;

	if(key == 'b')ty++;
	if(key == 'v')ty--;

	if(key == 'm')tz++;
	if(key == 'n')tz--;

	if(key == '1')testcrap = 1;
	if(key == '2')testcrap = 2;
}
void keyUp(unsigned char key, int x, int y)
{
	//printf("KeyUp %d\n", key);
	if(key == 'w')wkey = 0;
	if(key == 's')skey = 0;
	if(key == 'a')akey = 0;
	if(key == 'd')dkey = 0;
}
void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(refreshMS, timer, 0);
}
void mouseMovement(int x, int y)
{
	int diffx = x-(int)lastx;
	int diffy = y-(int)lasty;
	lastx = (float)x;
	lasty = (float)y;
	xrot -= (float)diffy; //UP AND DOWN
	//yrot += (float)diffx; //TILT LEFT AND RIGHT
	zrot -= (float)diffx; //LEFT AND RIGHT

	//ztest =	cos(xrot/180*3.141592654f);
	//xtest = sin(zrot/180*3.141592654f);
	//printf("xrot: %f zrot: %f\n", xtest, ztest);
}
void initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
	GLfloat LightAmbient[] = {0.25f, 0.25f, 0.25f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	GLfloat LightDiffuse[] = {0.75f, 0.75f, 0.75f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	GLfloat light0_position[4] = {1,1,1,0};
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	//
	GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
	GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
	//
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //WIREFRAME MODE
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //FILL OBJECT DRAW

	glewInit();
	if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
	{
		printf("Ready for GLSL - vertex, fragment, and geometry units\n");

		//LOAD SHADERS
		loadShaders(prog_hdlr, "shaders/vert_shader.glsl", "shaders/frag_shader.glsl");
		loadShaders(prog_hdlr, "shaders/vert_test.glsl", "shaders/frag_test.glsl");
		
		location_attribute_0 = glGetAttribLocation(prog_hdlr, "R"); //radius
		location_viewport = glGetUniformLocation(prog_hdlr, "viewport"); //viewport
		
		glGenBuffers(1, &program);
		glGenBuffers(1, &index);
		glBindBuffer(GL_ARRAY_BUFFER, program);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(f) + sizeof(normals), 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(f), f);
		offset+=sizeof(f);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(normals), normals);
		offset+=sizeof(normals);

		//glBufferData(GL_ARRAY_BUFFER, sizeof(f) + sizeof(normals), 0, GL_STATIC_DRAW);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(f), f);
		//glBufferSubData(GL_ARRAY_BUFFER, sizeof(f), sizeof(normals), normals);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indarr), indarr, GL_STATIC_DRAW);
	}
}
int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("GLSL tutorial");
	glutDisplayFunc(display);
	//glutIdleFunc(display);
	//glutReshapeFunc(reshape);
	//glutPassiveMotionFunc(mouseMovement);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(0, timer, 0);
	initGL();
	glutMainLoop();
	return 0;
}

/*
void reshape(GLsizei w, GLsizei h)
{
	if(h == 0)h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.0*(GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}
*/