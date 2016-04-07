// faceDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "faceDemo.h"
//vec3 mPosition = vec3(0, 0, 0);
//vec3 light_pos = vec3(100.0, 100.0, 100.0);
//vec3 light_color = vec3(1.0, 1.0, 1.0);
//const GLfloat  dr = 5.0 * DegreesToRadians;
//int display_mode;
//float translucency_value;
//float scale_factor = 1;
//vec3 rotate_factor = vec3(0);
//int shading_mode;
HeadScene scene;

faceDemo::faceDemo() {
};

faceDemo::~faceDemo() {
};

void faceDemo::init() {
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
	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mousecontrol);

	scene.InitScene();
}

void faceDemo::mousecontrol(GLFWwindow* window, int button, int action, int mods) {
	
}

void faceDemo::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	scene.KeyboardFunction(key, action);
}

void faceDemo::initFBO() {
	//create the shadow map texture
	GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	int smWidth = 1024;
	int smHeight = 1024;
	GLuint depthTex;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, smWidth, smHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	//assign the shadow map to texture channel 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthTex);

	//create and set up the FBO
	GLuint shadowFBO;
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

	GLenum drawBuffers[] = { GL_NONE };
	glDrawBuffers(1, drawBuffers);

	//revert to the default framebuffer for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint faceDemo::createRT() {
	//create FBO
	GLuint FBOName;
	glGenFramebuffers(1, &FBOName);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOName);

	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	//assign the shadow map to texture channel2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	//bind to shader

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FALSE in creating rendered texture!!" << std::endl;

	return FBOName;
}

void faceDemo::loop() {
	bool running = true;
	do
	{
		this->render();
		running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
		glfwPollEvents();
		glfwSwapBuffers(window);
	} while (running);
};

void faceDemo::render() {
	scene.RenderScene();
};

void faceDemo::terminate() {
	//destroy window
	glfwDestroyWindow(window);
	
	//terminate glfw
	glfwTerminate();
}

int main()
{
	faceDemo* facedemo = new faceDemo();
	facedemo->init();
	facedemo->loop();
    return 0;
}

