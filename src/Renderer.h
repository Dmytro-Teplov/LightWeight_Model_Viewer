#pragma once
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include "GLHelpers.h"

struct Frustum
{
    glm::vec3 Near;
    glm::vec3 Far;

    glm::vec3 Right;
    glm::vec3 Left;

    glm::vec3 Top;
    glm::vec3 Bottom;

    void initialize(float left, float right, float bottom, float top, float near, float far);
    void adjust(float zoom, glm::vec3 transform);
    void adjust(glm::vec3 transform);

};
class Camera
{
private:
    short ID = 0; 

public:
    glm::vec3 pos = glm::vec3(0.0f, 2.0f, 20.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view = glm::lookAt(pos, target, up);//glm::translate(glm::mat4(1.0f), pos);
    glm::mat4 projection = glm::ortho(-1.0f, +1.0f, -1.0f, +1.0f, 0.1f, 100.0f);

    Camera();
    Camera(float width, float height);

    void update(float width, float height);
    void zoom(GLFWwindow* window, double xoffset, double yoffset);
    void keys_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mbutton_callback(GLFWwindow* window, int button, int action, int mods);
};
class Model
{
private:
    short ID = 0;

    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ebo = 0;
    VertexBuffer vb;
    IndexBuffer ib;

    unsigned int indxs = 0;
public:
    unsigned int shader = 0;
    unsigned int vert_count = 0;

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1));

    Model();
    ~Model();

    void initialize(float* verts, short verts_size, unsigned int* inds, short inds_size, unsigned int shader_);
    void rotate(bool x,bool y,bool z,float angle);
    void scale(bool x,bool y,bool z,float scale);
    void draw();
};

class Renderer
{
private:
    short ID = 0;
    unsigned int shader = 0;
public:

    GLFWwindow* window;
    Frustum frustum;
    Camera* camera = nullptr;

    Renderer();
    ~Renderer();

    void attachShader(unsigned int shader_);
    void updMat(glm::mat4 matrix, const char* matrix_name);
    void updFloat(float num, const char* name);
    void updInt(int val, const char* name);
    void updSampler(int sampler, const char* name);
    void updVec(glm::vec3 vec, const char* vec_name);
    void updVec(glm::vec2 vec, const char* vec_name);
    void updVec(glm::vec4 vec, const char* vec_name);
    void draw(Model &model);
};