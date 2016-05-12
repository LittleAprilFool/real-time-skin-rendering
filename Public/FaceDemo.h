#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include <vector>
#include "MyScene.h"
//#include "TestScene.h"

class FaceDemo {
public:
	FaceDemo();
	~FaceDemo();
	void Init();
	void Render();
	void Terminate();
	void Loop();
private:
	static void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseControl(GLFWwindow* window, int button, int action, int mods);
	static void CursorPosition(GLFWwindow* window, double xpos, double ypos);
	GLFWwindow* window;
	GLFWwindow* light;
	int width;
	int height;
};