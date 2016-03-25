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
vec3 mPosition = vec3(0, 0, 0);
vec3 light_pos = vec3(1.0, 1.0, 1.0);
vec3 light_color = vec3(1.0, 1.0, 1.0);
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

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points  
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//mouse&keyboard function
	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mousecontrol);

	//data initiation
	display_mode = 1;
	translucency_value = 0.5;
	Kd = vec3(0.5, 0.5, 0.5);
	global_ambient = vec3(0.5, 0.5, 0.5);
	
	//translate&rotate
}

void faceDemo::mousecontrol(GLFWwindow* window, int button, int action, int mods) {
	
}

void faceDemo::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_W && action == GLFW_PRESS) light_pos.y += 0.5;
	if (key == GLFW_KEY_S && action == GLFW_PRESS) light_pos.y -= 0.5;
	if (key == GLFW_KEY_A && action == GLFW_PRESS) light_pos.x += 0.5;
	if (key == GLFW_KEY_D && action == GLFW_PRESS) light_pos.x -= 0.5;
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) light_pos.z += 0.5;
	if (key == GLFW_KEY_E && action == GLFW_PRESS) light_pos.z -= 0.5;
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) theta = theta + dr;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) theta = theta - dr;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) mPosition.y += 0.02;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) mPosition.y -= 0.02;
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		radius = 1.5;
		theta = 0.0;
		phi = 0.0;
		iLeft = -0.2;
		iRight = 0.2;
		iBottom = -0.2;
		iTop = 0.2;
		zNear = -5;
		zFar = 1;
		mPosition = vec3(0.0, 0.0, 0.0);
		light_pos = vec3(1.0, 1.0, 1.0);
		light_color = vec3(1.0, 1.0, 1.0);
	}
	if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS) display_mode = 1;
	if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS) display_mode = 2;
	if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS) display_mode = 3;
	if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS) light_color = vec3(0.5, 0.5, 0.5);
	if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS) light_color = vec3(0.8, 0.8, 0.8);
	if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS) light_color = vec3(1.0, 1.0, 1.0);
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) mPosition.z += 0.1;
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) mPosition.z -= 0.1;
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
	GLuint vboHandles[5];
	glGenBuffers(5, vboHandles);
	GLuint posHandle = vboHandles[0];
	GLuint texHandle = vboHandles[1];
	GLuint normHandle = vboHandles[2];
	GLuint tHandle = vboHandles[3];
	GLuint nHandle = vboHandles[4];

	//add position data into buffers
	GLfloat *pdata = new GLfloat[iface->faceNum * 12];

	int dataSum = 0;

	for (auto i_group = iface->vgroup.begin(); i_group != iface->vgroup.end(); i_group ++)
	{
		for (auto i_face = i_group->begin();  i_face != i_group->end(); i_face ++)
		{
			for (auto i_point = i_face->begin(); i_point != i_face->end(); i_point ++)
			{
				int t = *i_point;
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

	for (auto i_group = iface->tgroup.begin(); i_group != iface->tgroup.end(); i_group ++)
	{
		for (auto i_face = i_group->begin(); i_face != i_group->end(); i_face ++)
		{
			for (auto i_point = i_face->begin(); i_point != i_face->end(); i_point++)
			{
				int t = *i_point;
				tdata[dataSum] = iface->texture[t].x;
				tdata[dataSum + 1] = iface->texture[t].y;
				dataSum += 2;
			}
		}
	}

	//buffer data
	glBindBuffer(GL_ARRAY_BUFFER, texHandle);
	glBufferData(GL_ARRAY_BUFFER, dataSum * sizeof(GLfloat), tdata, GL_STATIC_DRAW);
	
	//add normal data into buffers
	GLfloat *ndata = new GLfloat[iface->faceNum * 12];

	dataSum = 0;

	for (auto i_group = iface->vgroup.begin(); i_group != iface->vgroup.end(); i_group ++)
	{
		for (auto i_face = i_group->begin(); i_face != i_group->end(); i_face ++)
		{
			for (auto i_point = i_face->begin(); i_point != i_face->end(); i_point++)
			{
				int t = *i_point;
				ndata[dataSum] = iface->vnormal[t].x;
				ndata[dataSum + 1] = iface->vnormal[t].y;
				ndata[dataSum + 2] = iface->vnormal[t].z;
				dataSum += 3;
			}
		}
	}

	//buffer data
	glBindBuffer(GL_ARRAY_BUFFER, normHandle);
	glBufferData(GL_ARRAY_BUFFER, dataSum * sizeof(GLfloat), ndata, GL_STATIC_DRAW);
	
	//add T data into buffers
	GLfloat *ttdata = new GLfloat[iface->faceNum * 16];

	dataSum = 0;

	for (auto i_group = iface->vgroup.begin(); i_group != iface->vgroup.end(); i_group ++)
	{
		for (auto i_face = i_group->begin(); i_face != i_group->end(); i_face ++)
		{
			for (auto i_point = i_face->begin(); i_point != i_face->end(); i_point ++)
			{
				int t = *i_point;
				ttdata[dataSum] = iface->T[t].a;
				ttdata[dataSum + 1] = iface->T[t].b;
				ttdata[dataSum + 2] = iface->T[t].g;
				ttdata[dataSum + 3] = iface->T[t].w;
				dataSum += 4;
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, tHandle);
	glBufferData(GL_ARRAY_BUFFER, dataSum * sizeof(GLfloat), ttdata, GL_STATIC_DRAW);


	//add N data into buffers
	GLfloat *nndata = new GLfloat[iface->faceNum * 12];
	
	dataSum = 0;

	for (auto i_group = iface->vgroup.begin(); i_group != iface->vgroup.end(); i_group++)
	{
		for (auto i_face = i_group->begin(); i_face != i_group->end(); i_face++)
		{
			for (auto i_point = i_face->begin(); i_point != i_face->end(); i_point++)
			{
				int t = *i_point;
				nndata[dataSum] = iface->N[t].x;
				nndata[dataSum + 1] = iface->N[t].y;
				nndata[dataSum + 2] = iface->N[t].z;
				dataSum += 3;
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, nHandle);
	glBufferData(GL_ARRAY_BUFFER, dataSum * sizeof(GLfloat), nndata, GL_STATIC_DRAW);

	//generate arrays
	GLuint vaoHandles;
	glGenVertexArrays(1, &vaoHandles);
	glBindVertexArray(vaoHandles);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, posHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, texHandle);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, normHandle);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, tHandle);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, nHandle);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	loc_model_view = glGetUniformLocation(program, "model_view");
	loc_projection = glGetUniformLocation(program, "projection");
	loc_translucency = glGetUniformLocation(program, "translucency");
	loc_map_kd = glGetUniformLocation(program, "map_kd");
	loc_map_bump = glGetUniformLocation(program, "map_bump");
	loc_model_position = glGetUniformLocation(program, "model_pos");
	loc_light_pos = glGetUniformLocation(program, "light_pos");
	loc_light_color = glGetUniformLocation(program, "light_color");
	loc_Kd = glGetUniformLocation(program, "Kd");
	loc_global_ambient = glGetUniformLocation(program, "global_ambient");
}

void faceDemo::loadTexture(char* filename) {
	texture_kd = 0;
	glActiveTexture(GL_TEXTURE0);
	TextureManager::Inst()->LoadTexture("lambertian.jpg", texture_kd, GL_BGR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // 线性滤波
	glUniform1i(loc_map_kd, texture_kd);


	texture_bump = 1;
	glActiveTexture(GL_TEXTURE1);
	TextureManager::Inst()->LoadTexture("normal.jpg", texture_bump, GL_RGB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // 线性滤波
	glUniform1i(loc_map_bump, texture_bump);
	
}

void faceDemo::setup() {
	loadOBJ("head.obj");
	loadShader();
	initBuffer();
	loadTexture("lambertian.jpg");
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vec3 eye(radius*sin(theta)*cos(phi),
		radius*sin(theta)*sin(phi),
		radius*cos(theta));
	vec3  at(0.0, 0.0, 0.0);
	vec3    up(0.0, 1.0, 0.0);
	mat4  mv = lookAt(eye, at, up);
	glUniformMatrix4fv(loc_model_view, 1, GL_TRUE, &mv[0][0]);
	mat4  p = ortho(iLeft, iRight, iBottom, iTop, zNear, zFar);
	glUniformMatrix4fv(loc_projection, 1, GL_TRUE, &p[0][0]);
	glUniform1f(loc_translucency, translucency_value);
	glUniform3f(loc_model_position, mPosition.x, mPosition.y, mPosition.z);

	glUniform3f(loc_light_pos, light_pos.x, light_pos.y, light_pos.z);
	glUniform3f(loc_light_color, light_color.x, light_color.y, light_color.z);
	glUniform3f(loc_Kd, Kd.x, Kd.y, Kd.z);
	glUniform3f(loc_global_ambient, global_ambient.x, global_ambient.y, global_ambient.z);

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

