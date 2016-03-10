#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include <vector>
#include "obj3d.h"
#include "shader.h"


class faceDemo {
public:
	faceDemo();
	~faceDemo();
	void init();
	void render();
	void terminate();
	void setup();
	void loop();
private:
	void loadOBJ(char*);
	void loadShader();
	void initBuffer();
	static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mousecontrol(GLFWwindow* window, int button, int action, int mods);
	GLFWwindow* window;
	obj3d* iface;
	shader* bling;
	GLint program;
	GLuint model_view;
	GLuint projection;
	GLuint translucency;
};