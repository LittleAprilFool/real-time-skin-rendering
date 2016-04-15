#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include <vector>
//#include "myscene.h"
#include "TestScene.h"

class faceDemo {
public:
	faceDemo();
	~faceDemo();
	void Init();
	void Render();
	void Terminate();
	void Loop();
private:
	static void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseControl(GLFWwindow* window, int button, int action, int mods);
	GLFWwindow* window;
};