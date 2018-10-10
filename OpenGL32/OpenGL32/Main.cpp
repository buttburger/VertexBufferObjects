#include "Common.h"

GLFWwindow*window;
int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

float horizontalAngle = 3.14f;//Initial horizontal angle : toward -Z
float verticalAngle = 0.0f;//Initial vertical angle : none
float mouseSpeed = 0.005f;
float speed = 3.0f;
float deltaTime;

float rotX = 0, rotY = 0, rotZ = 0;
float modelX = 0, modelY = 0, modelZ = 0;
float eyeX = 0, eyeY = 0, eyeZ = 25;//Initial position : on +Z
glm::vec3 eye = glm::vec3(eyeX, eyeY, eyeZ);

glm::vec3 target, right;
glm::mat4 projection, view, ModelMatrix, MVP, RotationMatrix, TranslationMatrix, ScalingMatrix;

GLuint MatrixID;

char loadmodel[] = "test1.fbx", loadmodel1[] = "maya.obj", loadmodel2[] = "cube.obj";
char loadtexture[] = "texture.png", loadtexture1[] = "texture1.png", loadtexture2[] = "texture2.png";

int dostuff = 0;
bool hasloaded = 0;


void wait(double seconds)
{
	double endwait;//clock_t
	endwait = clock() + seconds * CLOCKS_PER_SEC;
	while(clock() < endwait){}
}

int main()
{
	if(!glfwInit())//Initialise GLFW
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GLSL TEST!", NULL, NULL);//Open a window and create its OpenGL context
	if(window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowPos(window, 5, 25);//glfwSetWindowPos(window, (mode->width - 200) / 2, (mode->height - 200) / 2);
	
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);//Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//GLFW_CURSOR_DISABLED);
	glfwPollEvents();
	//glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);//Set the mouse at the center of the screen
	glewExperimental = true;//Needed for core profile
	if(glewInit() != GLEW_OK)//Initialize GLEW
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	//Get version info
	const GLubyte*renderer = glGetString(GL_RENDERER);//get renderer string
	const GLubyte*version = glGetString(GL_VERSION);//version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);//Dark blue background
	glEnable(GL_DEPTH_TEST);//Enable depth test
	glDepthFunc(GL_LESS);//Accept fragment if it closer to the camera than the former one
	//glEnable(GL_CULL_FACE);//Cull triangles which normal is not towards the camera
	
	int frames = 0;
	double currentTime;
	double limitFPS = 1.0/45.0;
	double lastTime = glfwGetTime();
	double timer = lastTime;
	bool mousefix = 0;
		
	for(;;)
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);
		double xpos, ypos;
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			if(mousefix == 0)
			{
				xpos = float(SCREEN_WIDTH /2);
				ypos = float(SCREEN_HEIGHT/2);
				mousefix = 1;
				//printf("TEST XYPOS: %f - %f\n", xpos, ypos);
			}
			else
			{
				glfwGetCursorPos(window, &xpos, &ypos);
			}
			glfwGetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
			glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);//Reset mouse position for next frame
			//Compute new orientation
			horizontalAngle += mouseSpeed * float(SCREEN_WIDTH /2 - xpos);
			verticalAngle   += mouseSpeed * float(SCREEN_HEIGHT/2 - ypos);
			//printf("hA: %f vA: %f xPos: %f yPos: %f\n", horizontalAngle, verticalAngle, xpos, ypos);
		}
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//GLFW_CURSOR_DISABLED);
			mousefix = 0;
		}
		glViewport(0, 0, (GLsizei)SCREEN_WIDTH, (GLsizei)SCREEN_HEIGHT);
		//printf("SW: %d SH: %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
		projection = glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 1000.0f);
		//Direction : Spherical coordinates to Cartesian coordinates conversion
		target = glm::vec3(cos(verticalAngle) * sin(horizontalAngle), 
						sin(verticalAngle),
						cos(verticalAngle) * cos(horizontalAngle));
		//Right vector
		right = glm::vec3(sin(horizontalAngle - 3.14f/2.0f),
									0,
									cos(horizontalAngle - 3.14f/2.0f));
		
		glm::vec3 up = glm::cross(right, target);
		view = LookAt(eye, eye+target, up);
		
		keyPress();
		
		if(hasloaded == 0)
		{
			//initVBO(loadmodel, loadtexture);//LOAD FBX MODEL
			initVBO(loadmodel1, loadtexture1, 0);//LOAD OBJ MODEL
			initVBO(loadmodel2, loadtexture2, 1);//LOAD OBJ MODEL
			hasloaded = 1;
		}
		if(dostuff == 1)
		{
			printf("LOAD!\n");
			hasloaded = 0;
			dostuff = 0;
		}
		if(dostuff == 2)
		{
			printf("DELETE!\n");
			deleteVBO(0);
			deleteVBO(1);
			dostuff = 0;
		}
		if(dostuff == 3)
		{
			printf("I HAVE THE SHINIEST MEAT BICYCLE!\n");
			dostuff = 0;
		}
		loadVBO(0);
		loadVBO(1);
		
		glfwSwapInterval(0);//UNLOCKFPS?
		glfwSwapBuffers(window);
		glfwPollEvents();

		frames++;
		lastTime = currentTime;
		currentTime = glfwGetTime();
		deltaTime = (float)(currentTime - lastTime);
		wait(limitFPS - deltaTime);
		if(currentTime - timer > 1.0)//Reset after one second
		{
			timer++;
			//printf("FPS: %d - %fms\n", frames, 1000.0/double(frames));
			frames = 0;
		}
		if((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && glfwWindowShouldClose(window) == 0))break;
	}
	deleteVBOall();
	glfwTerminate();
	return 0;
}
