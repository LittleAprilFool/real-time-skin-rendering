#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include <vector>
#include "myscene.h"

class faceDemo {
public:
	faceDemo();
	~faceDemo();
	void init();
	void render();
	void terminate();
	void loop();
private:
	void initFBO();
	GLuint createRT();
	static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mousecontrol(GLFWwindow* window, int button, int action, int mods);
	GLFWwindow* window;
};