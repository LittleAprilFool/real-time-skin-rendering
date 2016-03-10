// faceDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "faceDemo.h"

GLfloat radius = 1.5;
GLfloat theta = 0.0;
GLfloat phi = 0.0;
GLfloat  iLeft = -0.2;
GLfloat iRight = 0.2;
GLfloat iBottom = -0.2;
GLfloat iTop = 0.2;
GLfloat  zNear = -5;
GLfloat zFar = 1;
const GLfloat  dr = 5.0 * DegreesToRadians;
int display_mode;
float translucency_value;

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

	//mouse&keyboard function
	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mousecontrol);

	display_mode = 1;
	translucency_value = 0.5;

	//translate&rotate

}

void faceDemo::mousecontrol(GLFWwindow* window, int button, int action, int mods) {
	
}

void faceDemo::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		radius = radius * 1.5;
		cout << "q pressed" << endl;
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		radius = radius * 0.8;
		cout << "w pressed" << endl;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		theta = theta + dr;
		cout << "left pressed" << endl;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		theta = theta - dr;
		cout << "right pressed" << endl;
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		phi = phi + dr;
		cout << "up pressed" << endl;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		phi = phi - dr;
		cout << "down pressed" << endl;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		radius = 1.0;
		theta = 0.0;
		phi = 0.0;
		iLeft = -1.0;
		iRight = 1;
		iBottom = -1.0;
		iTop = 1;
		zNear = 0.1;
		zFar = 5.0;
		cout << "r pressed" << endl;
	}
	if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS) display_mode = 1;
	if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS) display_mode = 2;
	if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS) display_mode = 3;
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS && translucency_value < 1) translucency_value += 0.1;
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS && translucency_value > 0) translucency_value -= 0.1;
	//if (key == GLFW_KEY_1 && action == GLFW_PRESS) radius += 0.2;
	//if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS) radius -= 0.2;
	//if (key == GLFW_KEY_2 && action == GLFW_PRESS) theta += 0.2;
	//if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS) theta -= 0.2;
	//if (key == GLFW_KEY_2 && action == GLFW_PRESS) phi += 0.2;
	//if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS) phi -= 0.2;
	//if (key == GLFW_KEY_3 && action == GLFW_PRESS) iLeft += 0.2;
	//if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS) iLeft -= 0.2;
	//if (key == GLFW_KEY_4 && action == GLFW_PRESS) iRight += 0.2;
	//if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS) iRight -= 0.2;
	//if (key == GLFW_KEY_5 && action == GLFW_PRESS) iBottom += 0.2;
	//if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS) iBottom -= 0.2;
	//if (key == GLFW_KEY_6 && action == GLFW_PRESS) iTop += 0.2;
	//if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS) iTop -= 0.2;
	//if (key == GLFW_KEY_7 && action == GLFW_PRESS) zNear += 0.2;
	//if (key == GLFW_KEY_KP_7 && action == GLFW_PRESS) zNear -= 0.2;
	//if (key == GLFW_KEY_8 && action == GLFW_PRESS) zFar += 0.2;
	//if (key == GLFW_KEY_KP_8 && action == GLFW_PRESS) zFar -= 0.2;
}

void faceDemo::loadOBJ(char* filename) {
	iface = new obj3d();
	iface->loadMesh(filename);
};

void faceDemo::loadShader() {
	bling = new shader;
	program = bling->initShader("vbling.glsl", "fbling.glsl");
	glUseProgram(program);
};

void faceDemo::initBuffer() {
	//generate buffers
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint posHandle = vboHandles[0];
	GLuint texHandle = vboHandles[1];

	//add position data into buffers
	GLfloat *pdata = new GLfloat[iface->faceNum * 12];

	int dataSum = 0;

	for (vector<group>::iterator i_group = iface->vgroup.begin(); i_group != iface->vgroup.end(); i_group ++)
	{
		for (vector<face>::iterator i_face = i_group->begin();  i_face != i_group->end(); i_face ++)
		{
			for (vector<int>::iterator i_point = i_face->begin(); i_point != i_face->end(); i_point ++)
			{
				int t = *i_point - 1;
				pdata[dataSum] = iface->vertice[t].x;
				pdata[dataSum + 1] = iface->vertice[t].y;
				pdata[dataSum + 2] = iface->vertice[t].z;
				dataSum += 3;
			}
		}
	}

	//buffer data
	glBindBuffer(GL_ARRAY_BUFFER, posHandle);
	glBufferData(GL_ARRAY_BUFFER, dataSum * sizeof(GLfloat), pdata, GL_STATIC_DRAW);

	//add texcoord data into buffers
	GLfloat *tdata = new GLfloat[iface->faceNum * 8];

	dataSum = 0;

	for (vector<group>::iterator i_group = iface->tgroup.begin(); i_group != iface->tgroup.end(); i_group ++)
	{
		for (vector<face>::iterator i_face = i_group->begin(); i_face != i_group->end(); i_face ++)
		{
			for (vector<int>::iterator i_point = i_face->begin(); i_point != i_face->end(); i_point++)
			{
				int t = *i_point - 1;
				tdata[dataSum] = iface->texture[t].x;
				tdata[dataSum + 1] = iface->texture[t].y;
				dataSum += 2;
			}
		}
	}

	//buffer data
	glBindBuffer(GL_ARRAY_BUFFER, texHandle);
	glBufferData(GL_ARRAY_BUFFER, dataSum * sizeof(GLfloat), tdata, GL_STATIC_DRAW);
	
	//generate arrays
	GLuint vaoHandles;
	glGenVertexArrays(1, &vaoHandles);
	glBindVertexArray(vaoHandles);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, posHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, texHandle);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	model_view = glGetUniformLocation(program, "model_view");
	projection = glGetUniformLocation(program, "projection");
	translucency = glGetUniformLocation(program, "translucency");
}

void faceDemo::setup() {
	this->loadOBJ("head.obj");
	this->loadShader();
	this->initBuffer();
};

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

	glClear(GL_COLOR_BUFFER_BIT);
	vec3 eye(radius*sin(theta)*cos(phi),
		radius*sin(theta)*sin(phi),
		radius*cos(theta));
	vec3  at(0.0, 0.0, 0.0);
	vec3    up(0.0, 1.0, 0.0);
	mat4  mv = lookAt(eye, at, up);
	glUniformMatrix4fv(model_view, 1, GL_TRUE, &mv[0][0]);
	mat4  p = ortho(iLeft, iRight, iBottom, iTop, zNear, zFar);
	glUniformMatrix4fv(projection, 1, GL_TRUE, &p[0][0]);

	glUniform1f(translucency, translucency_value);

	if(display_mode == 1) glDrawArrays(GL_QUADS, 0, iface->faceNum * 4);
	if(display_mode == 2) glDrawArrays(GL_POINTS, 0, iface->faceNum * 4);
	if(display_mode == 3) glDrawArrays(GL_LINES, 0, iface->faceNum * 4);

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
	facedemo->setup();
	facedemo->loop();
    return 0;
}

