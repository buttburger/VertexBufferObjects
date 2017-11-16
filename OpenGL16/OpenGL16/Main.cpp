#define GLEW_STATIC
#define FREEGLUT_STATIC
#pragma comment (lib, "freeglut_static.lib")
#pragma comment (lib, "libglew32.lib")
#pragma comment (lib, "glm_static.lib")
//#pragma comment (lib, "libpng16.lib")
#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\glm.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
//#include <png.h>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <math.h>

#include "LoadShaders.h"
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

float objtexture[] =
{
	0,1,2,
	3,4,5,
	5,6,7,
	8,9,10,
	11,12,13,
	7,3,5,
	0,14,1,
	3,15,4,
	5,16,6,
	8,17,9,
	11,18,12,
	7,19,3,
};

unsigned int index, color;
float r = 0.0f;
int offset = 0;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	r += 0.1f;
	glUseProgram(prog_hdlr);
	GLuint uniformModel = glGetUniformLocation(prog_hdlr, "model");
	GLuint uniformView = glGetUniformLocation(prog_hdlr, "view");
	GLuint uniformProjection = glGetUniformLocation(prog_hdlr, "projection");
	
	glm::mat4 projection = glm::perspective(70.0f, 1.0f, 0.1f, 50.0f);
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	
	glm::vec3 eye = glm::vec3(sin(r*0.33)*5.0f, 5, cos(r*0.33)*5.0f);
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::mat4 view = glm::lookAt(eye, center, up);
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)); //PLAY AROUND WITH THESE VALUES HEEHEE
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
void process_keys(unsigned char key, int x, int y)
{
	if(key == 27)exit(0);
}
void reshape(GLsizei w, GLsizei h)
{
	if(h == 0) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, 1.0*(GLfloat)w/(GLfloat)h, 0.1f, 50.0f);
	glMatrixMode(GL_MODELVIEW);
}
void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(refreshMS, timer, 0);
}
int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("GLSL tutorial");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(process_keys);
	//glutIdleFunc(display);
	//
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glewInit();
	if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
	{
		printf("Ready for GLSL - vertex, fragment, and geometry units\n");
		setShaders(prog_hdlr, "shaders/vert_shader.glsl", "shaders/frag_shader.glsl");
		location_attribute_0 = glGetAttribLocation(prog_hdlr, "R"); //radius
		location_viewport = glGetUniformLocation(prog_hdlr, "viewport"); //viewport
		//
		glGenBuffers(1, &program);
		glGenBuffers(1, &index);
		glBindBuffer(GL_ARRAY_BUFFER, program);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		//
		glBufferData(GL_ARRAY_BUFFER, sizeof(f) + sizeof(normals), 0, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(f), f);
		offset+=sizeof(f);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(normals), normals);
		offset+=sizeof(normals);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indarr), indarr, GL_STATIC_DRAW);
	}
	else
	{
		printf("No GLSL support\n");
	}
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}