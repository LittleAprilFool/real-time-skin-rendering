#include "stdafx.h"
#include "myscene.h"

HeadScene::HeadScene()
{
}


HeadScene::~HeadScene()
{
}

void HeadScene::RenderScene() 
{
	UpdateModelMatrix_();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth_ID);
	RenderObject_(head, shader_depth_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_light_ID);
	RenderObject_(head, shader_light_ID);
	CopyTexture_(texture_light_ID, texture_toblur_ID);
	RenderBlur_(3, 0);
	//CopyTexture_(texture_blur_ID, texture_toadd_ID);
	//gaussion_type = 3;
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo_add_ID);
	//RenderObject_(head, shader_add_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderObject_(head, shader_bling_ID);
}

void HeadScene::RenderBlur_(int para, int rendered) {
	gaussion_type = para;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_blur_ID);
	blur_type = 1;
	RenderObject_(head, shader_blur_ID);
	CopyTexture_(texture_blur_ID, texture_toblur_ID);
	if (rendered == 1) glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else glBindFramebuffer(GL_FRAMEBUFFER, fbo_blur_ID);
	blur_type = 2;
	RenderObject_(head, shader_blur_ID);
}

void HeadScene::CopyTexture_(GLuint tex_src, GLuint tex_dst)
{
	GLuint tex1 = tex_src - GL_TEXTURE0 + 1;
	GLuint tex2 = tex_dst - GL_TEXTURE0 + 1;
	glCopyImageSubData(tex1, GL_TEXTURE_2D, 0, 0, 0, 0,
		tex2, GL_TEXTURE_2D, 0, 0, 0, 0,
		scene_width, scene_height, 1);
}

void HeadScene::RenderLight()
{
	//bind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_light_ID);
	RenderObject_(head, shader_light_ID);
	CopyTexture_(texture_light_ID, texture_toblur_ID);
	RenderBlur_(3, 1);
	//CopyTexture_(texture_blur_ID, texture_toadd_ID);
	//gaussion_type = 3;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//RenderObject_(head, shader_add_ID);
}

void HeadScene::RenderObject_(Object* obj, GLuint shader_ID) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//bind cube vao
	glBindVertexArray(obj->vao_handles);
	//use cube shader
	glUseProgram(shader_ID);
	//get uniform locations
	GetUniformLocations_(shader_ID);
	//update data to shader
	TransferDataToShader_();
	//draw array
	DrawArray_(obj->face_number);
}

void HeadScene::InitScene(int width, int height) 
{
	InitGLFunc_();
	InitParameters_();

	scene_width = width;
	scene_height = height;

	beckmann.Precompute(1, 1);
	//load textures
	LoadTexture(texture_kd_ID, "head-texture.jpg");
	LoadTexture(texture_bump_ID,  "head-normal.jpg");
	LoadTexture(texture_scattered_ID, "head-scattered.jpg");

	//load shaders
	shader_bling_ID = LoadShader(shader_bling, "vbling.glsl", "fbling.glsl");
	shader_light_ID = LoadShader(shader_light, "vtexture.glsl", "ftexture.glsl");
	shader_test_ID = LoadShader(shader_test, "vtest.glsl", "ftest.glsl");
	shader_blur_ID = LoadShader(shader_blur, "vblur.glsl", "fblur.glsl");
	shader_depth_ID = LoadShader(shader_depth, "vshadow.glsl", "fshadow.glsl");
	shader_add_ID = LoadShader(shader_add, "vadd.glsl", "fadd.glsl");

	//create fbo
	fbo_depth_ID= CreateRenderTextureForDepth_(texture_depth_ID, scene_width, scene_height);
	fbo_light_ID = CreateRenderTexture_(texture_light_ID, scene_width, scene_height);
	fbo_blur_ID = CreateRenderTexture_(texture_blur_ID, scene_width, scene_height);
	fbo_beckmann_ID = CreateRenderTexture_(texture_beckmann_ID, scene_width, scene_height);
	fbo_add_ID = CreateRenderTexture_(texture_afteradd_ID, scene_width, scene_height);

	LoadTexture(texture_toblur_ID, "toblur.jpg");
	LoadTexture(texture_toadd_ID, "toblur.jpg");
	LoadTexture(texture_add_ID, "toblur.jpg");

	head = new Object;
	head->LoadMesh("head.obj");
	head->BufferObjectData();

	cube = new Object;
	cube->LoadMesh("cube.obj");
	cube->BufferObjectData();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_beckmann_ID);
	beckmann.Render();
}

void HeadScene::KeyboardFunction(int key, int action) 
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS) light_position.y += 0.2;
	if (key == GLFW_KEY_S && action == GLFW_PRESS) light_position.y -= 0.2;
	if (key == GLFW_KEY_A && action == GLFW_PRESS) light_position.x += 0.2;
	if (key == GLFW_KEY_D && action == GLFW_PRESS) light_position.x -= 0.2;
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) light_position.z += 0.2;
	if (key == GLFW_KEY_E && action == GLFW_PRESS) light_position.z -= 0.1;
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) eye = vec3(0, 0, 1);
	if (key == GLFW_KEY_X && action == GLFW_PRESS) eye = vec3(1, 0, 0);

	if (key == GLFW_KEY_C && action == GLFW_PRESS) light_position = vec3(0, 0, -1);
	if (key == GLFW_KEY_V && action == GLFW_PRESS) light_position = vec3(1, 0, 0);
	if (key == GLFW_KEY_B && action == GLFW_PRESS) light_position = vec3(-1, 0, 0);
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) rotate_factor.y += 0.2;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) rotate_factor.y -= 0.2;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) rotate_factor.x += 0.2;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) rotate_factor.x -= 0.2;
	if (key == GLFW_KEY_R && action == GLFW_PRESS) InitParameters_();

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS) blur_time = blur_time++;
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS) blur_time = blur_time--;
	if (blur_time < 0) blur_time = 0;

	if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS) display_mode = 1;
	if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS) display_mode = 2;
	if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS) display_mode = 3;
	if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS) shading_mode = 1;
	if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS) shading_mode = 2;
	if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS) shading_mode = 3;
	if (key == GLFW_KEY_KP_7 && action == GLFW_PRESS) shading_mode = 4;
	if (key == GLFW_KEY_KP_8 && action == GLFW_PRESS) shading_mode = 5;
	if (key == GLFW_KEY_KP_9 && action == GLFW_PRESS) shading_mode = 6;
	if (key == GLFW_KEY_KP_DIVIDE && action == GLFW_PRESS) test_mode = 0;
	if (key == GLFW_KEY_KP_MULTIPLY && action == GLFW_PRESS) test_mode = 1;
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) scale_factor += 0.1;
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) scale_factor -= 0.1;
}

void HeadScene::MouseControl(int button, int action, int mods) 
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouse_press_left = true;
		mouse_position_o = mouse_position;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mouse_press_left = false;
		mouse_position_o = vec2(0, 0);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		mouse_press_right = true;
		mouse_position_o = mouse_position;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		mouse_press_right = false;
		mouse_position_o = vec2(0, 0);
	}
}

void HeadScene::CursorPosition(double xpos, double ypos)
{
	mouse_position.x = xpos;
	mouse_position.y = ypos;

	if (mouse_press_left == true)
	{
		rotate_factor.y = rotate_factor.y + (mouse_position.x - mouse_position_o.x) / (10 * scene_width);
		rotate_factor.x = rotate_factor.x - (mouse_position.y - mouse_position_o.y) / (10 * scene_height);
	}


	if (mouse_press_right == true)
	{
		scale_factor = scale_factor + (mouse_position.y - mouse_position_o.y + mouse_position.x - mouse_position_o.x) / (20 * scene_width);
	}
}

void HeadScene::InitGLFunc_()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void HeadScene::InitParameters_()
{
	scale_factor = 1;
	rotate_factor = vec3(0);
	translucency_value = 0.5;
	shading_mode = 1;
	display_mode = 1;
	blur_time = 5;
	blur_type = 1;
	gaussion_type = 0;
	test_mode = 0;

	GLfloat  iLeft = -0.2;
	GLfloat iRight = 0.2;
	GLfloat iBottom = -0.2;
	GLfloat iTop = 0.2;
	GLfloat  zNear = 0;
	GLfloat zFar = 1;
	projection_matrix = ortho(iLeft, iRight, iBottom, iTop, zNear, zFar);

	eye = vec3(0.0, 0.0, 1.0);
	at = vec3(0.0, 0.0, 0.0);
	up = vec3(0.0, 1.0, 0.0);
	view_matrix = lookAt(eye, at, up);
	model_matrix = mat4(1.0);
	
	light_position = vec3(0, 0, 1);
	light_la = vec3(0.5, 0.5, 0.5);
	light_ld = vec3(1, 1, 1);
	light_ls = vec3(0.3, 0.3, 0.3);

	material_ka = vec3(0.5, 0.5, 0.5);
	material_kd = vec3(0.5, 0.5, 0.5);
	material_ks = vec3(0.2, 0.2, 0.2);
	material_shininess = 1;

	mouse_position = vec2(0, 0);
	mouse_position_o = vec2(0, 0);
	mouse_press_left = false;
	mouse_press_right = false;

	texture_kd_ID = GL_TEXTURE0;
	texture_bump_ID = GL_TEXTURE1;
	texture_scattered_ID = GL_TEXTURE2;
	texture_depth_ID = GL_TEXTURE3;
	texture_light_ID = GL_TEXTURE4;
	texture_blur_ID = GL_TEXTURE5;
	texture_beckmann_ID = GL_TEXTURE6;
	texture_afteradd_ID = GL_TEXTURE7;
	texture_toblur_ID = GL_TEXTURE8;
	texture_toadd_ID = GL_TEXTURE9;
	texture_add_ID = GL_TEXTURE10;
}


void HeadScene::DrawArray_(int face_number)
{
	if (display_mode == 1) glDrawArrays(GL_QUADS, 0, face_number * 4);
	if (display_mode == 2) glDrawArrays(GL_POINTS, 0, face_number * 4);
	if (display_mode == 3) glDrawArrays(GL_LINES, 0, face_number * 4);
}

void HeadScene::TransferDataToShader_()
{
	glUniformMatrix4fv(loc_model, 1, GL_TRUE, &model_matrix[0][0]);
	glUniformMatrix4fv(loc_view, 1, GL_TRUE, &view_matrix[0][0]);
	glUniformMatrix4fv(loc_projection, 1, GL_TRUE, &projection_matrix[0][0]);

	glUniform1f(loc_mode, shading_mode);
	glUniform1i(loc_blur_type, blur_type);
	glUniform1i(loc_gaussion_type, gaussion_type);

	glUniform3f(loc_light_pos, light_position.x, light_position.y, light_position.z);
	glUniform3f(loc_light_la, light_la.x, light_la.y, light_la.z);
	glUniform3f(loc_light_ld, light_ld.x, light_ld.y, light_ld.z);
	glUniform3f(loc_light_ls, light_ls.x, light_ls.y, light_ls.z);
	glUniform3f(loc_material_ka, material_ka.x, material_ka.y, material_ka.z);
	glUniform3f(loc_material_kd, material_kd.x, material_kd.y, material_kd.z);
	glUniform3f(loc_material_ks, material_ks.x, material_ks.y, material_ks.z);
	glUniform1f(loc_material_shininess, material_shininess);
	glUniform3f(loc_eye_pos, eye.x, eye.y, eye.z);

	glUniformMatrix4fv(loc_depth_model_matrix, 1, GL_TRUE, &depth_model_matrix[0][0]);
	glUniformMatrix4fv(loc_depth_view_matrix, 1, GL_TRUE, &depth_view_matrix[0][0]);
	glUniformMatrix4fv(loc_depth_projection_matrix, 1, GL_TRUE, &depth_projection_matrix[0][0]);


	glUniform1i(loc_map_kd, texture_kd_ID - GL_TEXTURE0);
	glUniform1i(loc_map_bump, texture_bump_ID - GL_TEXTURE0);
	glUniform1i(loc_map_light, texture_light_ID - GL_TEXTURE0);
	glUniform1i(loc_map_depth, texture_depth_ID - GL_TEXTURE0);
	glUniform1i(loc_map_scattered, texture_scattered_ID - GL_TEXTURE0);
	glUniform1i(loc_map_blur, texture_blur_ID - GL_TEXTURE0);
	glUniform1i(loc_map_beckmann, texture_beckmann_ID - GL_TEXTURE0);
	glUniform1i(loc_map_toblur, texture_toblur_ID - GL_TEXTURE0);
	glUniform1i(loc_map_add, texture_add_ID - GL_TEXTURE0);
	glUniform1i(loc_map_toadd, texture_toadd_ID - GL_TEXTURE0);
	glUniform1i(loc_map_afteradd, texture_afteradd_ID - GL_TEXTURE0);
}

void HeadScene::UpdateModelMatrix_() 
{
	model_matrix = mat4(1.0);
	model_matrix = scale(model_matrix, vec3(scale_factor));
	model_matrix = rotate<float>(model_matrix, rotate_factor.y, vec3(0, 1, 0));
	model_matrix = rotate<float>(model_matrix, rotate_factor.x, vec3(1, 0, 0));
	
	view_matrix = lookAt(eye, at, up);

	depth_projection_matrix = projection_matrix;
	depth_view_matrix = lookAt(light_position, at, up);
	depth_model_matrix = model_matrix;
}

void HeadScene::GetUniformLocations_(GLuint shader_ID)
{
	loc_model = glGetUniformLocation(shader_ID, "model_matrix");
	loc_view = glGetUniformLocation(shader_ID, "view_matrix");
	loc_projection = glGetUniformLocation(shader_ID, "projection_matrix");

	loc_light_pos = glGetUniformLocation(shader_ID, "Light.position");
	loc_light_la = glGetUniformLocation(shader_ID, "Light.la");
	loc_light_ld = glGetUniformLocation(shader_ID, "Light.ld");
	loc_light_ls = glGetUniformLocation(shader_ID, "Light.ls");

	loc_material_ka = glGetUniformLocation(shader_ID, "Material.ka");
	loc_material_kd = glGetUniformLocation(shader_ID, "Material.kd");
	loc_material_ks = glGetUniformLocation(shader_ID, "Material.ks");
	loc_material_shininess = glGetUniformLocation(shader_ID, "Material.shininess");
	
	loc_mode = glGetUniformLocation(shader_ID, "mode");
	loc_eye_pos = glGetUniformLocation(shader_ID, "eye_position");
	loc_blur_type = glGetUniformLocation(shader_ID, "blur_type");
	loc_gaussion_type = glGetUniformLocation(shader_ID, "gaussion_type");

	loc_map_kd = glGetUniformLocation(shader_ID, "map_kd");
	loc_map_bump = glGetUniformLocation(shader_ID, "map_bump");
	loc_map_depth = glGetUniformLocation(shader_ID, "map_depth");
	loc_map_scattered = glGetUniformLocation(shader_ID, "map_scattered");
	loc_map_light = glGetUniformLocation(shader_ID, "map_light");
	loc_map_blur = glGetUniformLocation(shader_ID, "map_blur");
	loc_map_beckmann = glGetUniformLocation(shader_ID, "map_beckmann");
	loc_map_toblur = glGetUniformLocation(shader_ID, "map_toblur");
	loc_map_add = glGetUniformLocation(shader_ID, "map_add");
	loc_map_toadd = glGetUniformLocation(shader_ID, "map_toadd");
	loc_map_afteradd = glGetUniformLocation(shader_ID, "map_afteradd");

	loc_depth_model_matrix = glGetUniformLocation(shader_ID, "depth_model_matrix");
	loc_depth_view_matrix = glGetUniformLocation(shader_ID, "depth_view_matrix");
	loc_depth_projection_matrix = glGetUniformLocation(shader_ID, "depth_projection_matrix");
}