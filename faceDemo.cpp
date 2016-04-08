// faceDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "faceDemo.h"

HeadScene scene;

faceDemo::faceDemo() {
};

faceDemo::~faceDemo() {
};

void faceDemo::Init() {
	//init glfw
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW!\n");
		exit(-1);
	}

	//create a glfw window
	window = glfwCreateWindow(800, 600, "faceDemo", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//init glew
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW!\n");
		exit(-1);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points  
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//mouse&keyboard function
	glfwSetKeyCallback(window, Keyboard);
	glfwSetMouseButtonCallback(window, MouseControl);

	scene.InitScene();
}

void faceDemo::MouseControl(GLFWwindow* window, int button, int action, int mods) {
	
}

void faceDemo::Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	scene.KeyboardFunction(key, action);
}

void faceDemo::Loop() {
	bool running = true;
	do
	{
		Render();
		running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
		glfwPollEvents();
		glfwSwapBuffers(window);
	} while (running);
};

void faceDemo::Render() {
	scene.RenderScene();
};

void faceDemo::Terminate() {
	//destroy window
	glfwDestroyWindow(window);
	
	//terminate glfw
	glfwTerminate();
}

int main()
{
	faceDemo* facedemo = new faceDemo();
	facedemo->Init();
	facedemo->Loop();
    return 0;
}

