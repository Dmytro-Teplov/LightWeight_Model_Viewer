#include "Renderer.h"

Camera::Camera(float width_, float height_)
{
	/*view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));*/
	//view = view_;
	width = width_;
	height = height_;
	projection = glm::perspective(glm::radians(45.0f), (float)width_ / (float)height_, 0.1f, 1000.0f);
}	//glm::ortho(-(float)width, (float)width, -(float)height, (float)height, 0.1f, 100.0f);
	

void Camera::update(float width_, float height_)
{
	width = width_;
	height = height_;
	projection = glm::perspective(glm::radians(45.0f), (float)width_ / (float)height_, 0.1f, 1000.0f);
}

void Camera::move(float x_old, float y_old, float x_new, float y_new, glm::vec3 &m)
{
	glm::vec3 raw_vec= glm::vec3(x_new - x_old, y_old - y_new, 0.0);
	if (raw_vec == glm::vec3(0, 0, 0)|| x_new==0 ||y_new==0)
		return;
	glm::vec3 transf = raw_vec.y * up + raw_vec.x * right;
	float sign = std::copysign(1.0f, raw_vec.x);
	//float radius = 0;
	/*float camX = 0;
	float camZ = 0;*/
	/*float x_norm_new = 2.0 * (x_new) / width - 1.0;
	float x_norm_old = 2.0 * (x_old) / width - 1.0;
	float y_norm_new = 1.0 - 2.0 * (y_new) / height;
	float y_norm_old = 1.0 - 2.0 * (y_old) / height;
	float x_sp, y_sp, z_sp;
	float x_sp_old, y_sp_old, z_sp_old;*/
	float angle;
	//std::cout << "X: " << x_norm_new << "\nY: " << y_norm_new;
	if (!glm::length(raw_vec))
		return;
	switch (mode)
	{
	case 1:
		//radius = glm::length(pos);
		angle = glm::radians(90.0f);
		glm::mat2 rotationMatrix = glm::mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
		glm::vec2 axis_2d = glm::normalize(rotationMatrix * glm::vec2(raw_vec.x, raw_vec.y));
		glm::vec3 axis_3d = glm::vec3(axis_2d.x, axis_2d.y, 0.0f);
		angle = glm::radians(glm::length(raw_vec));
		glm::vec3 new_pos = pos * cos(angle) + glm::cross(axis_3d, pos)*sin(angle)+ axis_3d*(glm::dot(axis_3d,pos))*(1.0f-cos(angle));
		pos = new_pos;
		view = glm::lookAt(pos, target, up);
		/*std::cout << "X " << axis_3d.x << "\n";
		std::cout << "Y " << axis_3d.y << "\n";*/
		break;
	case 2:
		glm::vec3 transl = sign * (float)glm::length(raw_vec) * (float)0.01 * glm::normalize(pos - target);
		pos = pos - transl;
		target = target - transl;
		view = glm::lookAt(pos, target, up);
		break;
	case 3:
		pos = pos - transf * 0.01f;
		target = target - transf * 0.01f;
		view = glm::lookAt(pos, target, up);
		break;
	}
	right = glm::normalize(glm::cross(up, pos - target));
}

void Camera::zoom(GLFWwindow* window, double xoffset, double yoffset)
{
	glm::vec3 dir = glm::normalize(pos - target);
	pos = pos + (float)yoffset * dir;
	target = target + (float)yoffset*dir;
	view = glm::lookAt(pos, target, up);
}

void Camera::keys_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		glm::vec3 dir = glm::normalize(pos - target);
		dir.y = 0;
		pos = pos - dir;
		target = target - dir;
		view = glm::lookAt(pos, target, up);
	}
	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		glm::vec3 dir = glm::normalize(pos - target);
		dir.y = 0;
		pos = pos + dir;
		target = target + dir;
		view = glm::lookAt(pos, target, up);
	}
	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		glm::vec3 dir = glm::normalize(pos - target);
		glm::vec3 right = glm::normalize(glm::cross(up, dir));
		dir.y = 0;
		pos = pos - right;
		target = target - right;
		view = glm::lookAt(pos, target, up);
	}
	if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		glm::vec3 dir = glm::normalize(pos - target);
		glm::vec3 right = glm::normalize(glm::cross(up, dir));
		dir.y = 0;
		pos = pos + right;
		target = target + right;
		view = glm::lookAt(pos, target, up);
	}
}

void Camera::mbutton_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (mods == GLFW_MOD_ALT)
	{
		switch(action)
		{
		case GLFW_PRESS:
			if (button == GLFW_MOUSE_BUTTON_LEFT)
				mode = 1;
			else if (button == GLFW_MOUSE_BUTTON_RIGHT)
				mode = 2;
			else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
				mode = 3;
			else
				return;
			needs_update = true;
			break;
		case GLFW_RELEASE:
			mode = 0;
			needs_update = false;
			//view_static = view;
			break;
		}
	}
}


Model::Model()
{
}

Model::~Model()
{
}

void Model::initialize(float *verts, short verts_size,unsigned int *inds, short inds_size, unsigned int shader_)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vb.initialize(verts, verts_size * sizeof(float));
	ib.initialize(inds, inds_size);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 5 - sizeof(float) * 2)); //3 is the amount of uv coordinates

	shader = shader_;
	indxs = inds_size;
}

void Model::rotate(bool x, bool y, bool z, float angle)
{
	rotation = rotation + glm::vec3(angle * x, angle * y, angle * z);
	model = glm::translate(glm::scale(glm::mat4(1.0f), scle),position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::scale(bool x, bool y, bool z, float scale)
{
	scle = scle * glm::vec3(scale * x, scale * y, scale * z);
	model = glm::translate(glm::scale(glm::mat4(1.0f), scle), position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::translate(glm::vec3 pos)
{
	position = position + pos;
	model = glm::translate(glm::scale(glm::mat4(1.0f), scle), position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::set_pos(glm::vec3 pos)
{
	position = pos;
	model = glm::translate(glm::scale(glm::mat4(1.0f), scle), position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::draw()
{
	glBindVertexArray(vao);
	vb.bind();
	ib.bind();
	glDrawElements(GL_TRIANGLES, indxs, GL_UNSIGNED_INT, 0);
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::attachShader(unsigned int shader_)
{
	shader = shader_;
	glUseProgram(shader);
}
void Renderer::updMat(glm::mat4 matrix, const char* matrix_name)
{
	int location = glGetUniformLocation(shader, matrix_name);

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
void Renderer::updFloat(float num, const char* name)
{
	int location = glGetUniformLocation(shader, name);
	glUniform1f(location, num);
}
void Renderer::updInt(int val, const char* name)
{
	int location = glGetUniformLocation(shader, name);
	glUniform1i(location, val);
}
void Renderer::updSampler(int sampler, const char* name)
{
	int location = glGetUniformLocation(shader, name);
	glUniform1i(location, sampler);
}
void Renderer::updVec(glm::vec2 vec, const char* vec_name)
{
	int location = glGetUniformLocation(shader, vec_name);
	glUniform2fv(location, 1, glm::value_ptr(vec));
}
void Renderer::updVec(glm::vec3 vec, const char* vec_name)
{
	int location = glGetUniformLocation(shader, vec_name);
	glUniform3fv(location, 1, glm::value_ptr(vec));
}
void Renderer::updVec(glm::vec4 vec, const char* vec_name)
{
	int location = glGetUniformLocation(shader, vec_name);
	glUniform4fv(location, 1, glm::value_ptr(vec));
}

void Renderer::draw(Model &model)
{
	attachShader(model.shader);
	updMat(model.model, "model");
	updMat(camera->view, "view");
	updMat(camera->projection, "projection");
	model.draw();
}

