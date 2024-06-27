#include "Renderer.h"

Camera::Camera(float width, float height)
{
	/*view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));*/
	//view = view_;
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
}	//glm::ortho(-(float)width, (float)width, -(float)height, (float)height, 0.1f, 100.0f);
	

void Camera::update(float width, float height)
{
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
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
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f*x, 1.0f*y, 1.0f*z));
}

void Model::scale(bool x, bool y, bool z, float scale)
{
	model = glm::scale(model,glm::vec3(scale * x, scale * y, scale * z));
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

