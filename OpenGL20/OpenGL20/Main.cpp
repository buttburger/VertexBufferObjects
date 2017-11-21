#define _CRT_SECURE_NO_WARNINGS
#define GLEW_STATIC
#define FREEGLUT_STATIC
#pragma comment (lib, "freeglut_static.lib")
#pragma comment (lib, "libglew32.lib")
#pragma comment (lib, "libpng16.lib")
#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <png.h>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <math.h>
#include "LoadShaders.h"
using namespace std;

int refreshMS = 15;
GLuint prog_hdlr, vao, vbo, ind, texture;
GLint location_attribute_0, location_viewport;
//const float light0_position[4] = {1,1,1,0};

//GLubyte*textureImage;

int loadPngImage(char*name, int&outWidth, int&outHeight, bool&outHasAlpha, GLubyte**outData)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	int color_type, interlace_type;
	FILE *fp;
	if((fp = fopen(name, "rb")) == NULL)return false;
	/* Create and initialize the png_struct with the desired error handler functions.
	*If you want to use the default stderr and longjump method, you can supply NULL for the last three parameters.
	*We also supply the the compiler header file version,
	so that we know if the application was compiled with a compatible version of the library. REQUIRED */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL)
	{
		fclose(fp);
		return false;
	}
	//Allocate/initialize the memory for image information.  REQUIRED.
	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}
	/* Set error handling if you are using the setjmp/longjmp method(this is the normal method of doing things with libpng).
	* REQUIRED unless you  set up your own error handlers in the png_create_read_struct() earlier.*/
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		/* Free all of the memory associated with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		/* If we get here, we had a problem reading the file */
		return false;
	}
	/* Set up the output control if you are using standard C streams */
	png_init_io(png_ptr, fp);
	/* If we have already read some of the signature */
	png_set_sig_bytes(png_ptr, sig_read);
	/*If you have enough memory to read in the entire image at once, and you need to specify only transforms that can be controlled
	* with one of the PNG_TRANSFORM_* bits (this presently excludes dithering, filling, setting background, and doing gamma
	* adjustment), then you can read the entire image (including pixels) into the info structure with this call
	*
	* PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING  forces 8 bit PNG_TRANSFORM_EXPAND forces to  expand a palette into RGB*/
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
	
	png_uint_32 width, height;
	int bit_depth;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
	//
	//outHasAlpha = (color_type & PNG_COLOR_MASK_ALPHA);
	outHasAlpha = (color_type == PNG_COLOR_TYPE_RGBA);
	//
	outWidth = width;
	outHeight = height;
	
	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	*outData = (unsigned char*) malloc(row_bytes * outHeight);
	
	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
	for(int i = 0; i < outHeight; i++)
	{
		//note that png is ordered top to bottom, but OpenGL expect it bottom to top so the order or swapped
		memcpy(*outData+(row_bytes*(outHeight-1-i)), row_pointers[i], row_bytes);
	}
	/* Clean up after the read, and free any memory allocated */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);
	return true;
}

GLfloat vdata[] =
{
	-0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //RED
	 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //GREEN
	 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, //BLUE
};

GLfloat tex[] =
{
	0.0f,0.0f,
	0.0f,1.0f,
	1.0f,-1.0f,
};

unsigned indarr[] = {0,1,2};

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glUseProgram(prog_hdlr);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

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
	gluPerspective(60.0f, 1.0*(GLfloat)w/(GLfloat)h, 0.1f, 50.0f);
	glMatrixMode(GL_MODELVIEW);
}
void timer(int value)
{
	glutPostRedisplay(); //Post re-paint request to activate display()
	glutTimerFunc(refreshMS, timer, 0); //next timer call milliseconds later
}
void initGL()
{
	glClearColor(0.7f, 0.7f, 0.3f, 1.0f);

	/*
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	*/

	/*
	int width, height;
	bool hasAlpha;
	char filename[] = "test.png";
	object.id_texture = loadPngImage(filename, width, height, hasAlpha, &textureImage);
	if(object.id_texture == -1)
	{
		printf("FAIL TO LOAD!\n");
	}
	printf("TEST: %d\n", object.id_texture);
	//cout<<"Image load "<<"Alpha "<<hasAlpha<<endl;
	printf("Image load Alpha %d\n", hasAlpha);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, width, height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureImage);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	*/

	//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glewInit();
	if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
	{
		printf("Ready for GLSL - vertex, fragment, and geometry units\n");
		setShaders(prog_hdlr, "shaders/vert_shader.glsl", "shaders/frag_shader.glsl");
		location_attribute_0 = glGetAttribLocation(prog_hdlr, "R"); //radius
		location_viewport = glGetUniformLocation(prog_hdlr, "viewport"); //viewport
		
		glGenBuffers(1, &vao);
		glBindBuffer(GL_ARRAY_BUFFER, vao);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vdata), vdata, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (char*)(sizeof(GLfloat)*3));
		
		/*
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(indarr), indarr, GL_STATIC_DRAW);
		*/
	}
	else
	{
		printf("No GLSL support\n");
	}
}
int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("GLSL tutorial");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(process_keys);
	//glutIdleFunc(display);
	initGL();
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}