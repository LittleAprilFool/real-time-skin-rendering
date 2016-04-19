// faceDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "faceDemo.h"

HeadScene scene;
//TestScene scene;
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

	width = 800;
	height = 800;

	//create a glfw window
	window = glfwCreateWindow(width, height, "faceDemo", NULL, NULL);
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

	//create a window for light
	light = glfwCreateWindow(width, height, "lightView", NULL, NULL);

	if (!light)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//mouse&keyboard function
	glfwSetKeyCallback(window, Keyboard);
	glfwSetMouseButtonCallback(window, MouseControl);
	glfwSetCursorPosCallback(window, CursorPosition);

	glfwSetKeyCallback(light, Keyboard);
	glfwSetMouseButtonCallback(light, MouseControl);

	glfwMakeContextCurrent(window);
	scene.InitScene(width, height);

	glfwMakeContextCurrent(light);
	scene.InitScene(width, height);
}

void faceDemo::CursorPosition(GLFWwindow* window, double xpos, double ypos) {
	scene.CursorPosition(xpos, ypos);
}

void faceDemo::MouseControl(GLFWwindow* window, int button, int action, int mods) {
	scene.MouseControl(button, action, mods);
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
		glfwSwapBuffers(light);
	} while (running);
};

void faceDemo::Render() {
	glfwMakeContextCurrent(window);
	scene.RenderScene();
	glfwMakeContextCurrent(light);
	scene.RenderLight();
};

void faceDemo::Terminate() {
	//destroy window
	glfwDestroyWindow(window);
	glfwDestroyWindow(light);
	
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