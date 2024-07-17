#include <iostream>
//#include <glfw3.h>
#include "Renderer.h"


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__unix__) || defined(__APPLE__) && defined(__MACH__)
#include <unistd.h>
#include <fcntl.h>
#endif

int main(void)
{
    GLFWwindow* window;
    Renderer r;
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int w_width = 1280, w_height = 720;
    window = glfwCreateWindow(1280, 720, "Light Weight Model Viewer", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //glfwGetWindowSize(window, &w_width, &w_height);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK)
        std::cout << "Error";
    glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);


    
    //glfwSetScrollCallback(window, scroll_callback);
    //glfwSetMouseButtonCallback(window, mouse_callback);

    Shader test_s("res/shaders/test.shader"),grid_s("res/shaders/grid.shader");
    test_s.CreateShader();
    grid_s.CreateShader();
    
    
    Model model,world_grid;
    Model XY,XZ,YZ;
    
    float v[40] = {
      -1.0f, -1.0f,  1.0, 1.0, 0.0,
      -1.0f, -1.0f, -1.0, 0.0, 1.0,
       1.0f, -1.0f, -1.0, 0.0, 0.0,
       1.0f, -1.0f,  1.0, 1.0, 0.0,
      -1.0f,  1.0f,  1.0, 0.0, 1.0,
      -1.0f,  1.0f, -1.0, 0.0, 0.0,
       1.0f,  1.0f, -1.0, 1.0, 0.0,
       1.0f,  1.0f,  1.0, 0.0, 1.0
    };
    unsigned int i[36] = {
    0,1,2,
    0,2,3,
    0,4,5,
    0,5,1,
    3,7,6,
    3,6,2,
    0,3,7,
    0,7,4,
    4,7,6,
    4,6,5,
    1,2,6,
    1,6,5
    };

    float v2[20] = {
     -1.0f, -1.0f, 0.0, 0.0, 0.0,
     -1.0f,  1.0f, 0.0, 1.0, 0.0,
      1.0f,  1.0f, 0.0, 1.0, 1.0,
      1.0f, -1.0f, 0.0, 0.0, 1.0
    };
    unsigned int i2[6] = {
    0,1,2,0,2,3
    };

    model.initialize(v,std::size(v), i, std::size(i), test_s.ID);
    world_grid.initialize(v2,std::size(v2), i2, std::size(i2), grid_s.ID);
    XY.initialize(v2,std::size(v2), i2, std::size(i2), grid_s.ID);
    YZ.initialize(v2,std::size(v2), i2, std::size(i2), grid_s.ID);

    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;

    Camera camera(w_width, w_height);
    glfwSetWindowUserPointer(window, &camera);

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            Camera* c = static_cast<Camera*>(glfwGetWindowUserPointer(window));
            c->zoom(window, xoffset, yoffset);
        });
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            Camera* c = static_cast<Camera*>(glfwGetWindowUserPointer(window));
            c->keys_callback(window, key, scancode, action, mods);
        });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
        {
            Camera* c = static_cast<Camera*>(glfwGetWindowUserPointer(window));
            c->mbutton_callback(window, button, action, mods);
        });


    r.window = window;
    r.camera = &camera;
    world_grid.rotate(true,false,false,-90);
    XY.rotate(false,true,false,-90);
    //YZ.rotate(true,false,false,-90);
    world_grid.scale(true,true,true,10);
    XY.scale(true,true,true,10);
    YZ.scale(true,true,true,10);
    //world_grid.scale(true,true,true,10);
    //model.scale(true,true,true,1.0);

    double x_temp, y_temp;

    glm::vec3 vec = glm::vec3(1.0);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;
        if (timeDiff >= 1.0 / 30.0)
        {
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string newTit1e = "Light Weight Model Viewer " + FPS + "FPS / +" + ms + "ms";
            glfwSetWindowTitle(window, newTit1e.c_str());
            prevTime = crntTime;
            counter = 0;
        }
        glfwGetWindowSize(window, &w_width, &w_height);
        glViewport(0, 0, w_width, w_height);

        model.rotate(false,true,false,1);
        //ADD IF NEEDED
        camera.update(w_width, w_height); 
        
        
        
        if (camera.needs_update)
        {
            glfwGetCursorPos(window, &x_temp, &y_temp);
            camera.move(r.x,r.y, x_temp, y_temp, vec);
            glfwGetCursorPos(window, &r.x, &r.y);
        }
        else
        {
            glfwGetCursorPos(window, &r.x, &r.y);
        }
        model.set_pos(vec);
        
        r.draw(model);
        r.draw(world_grid);
        r.draw(YZ);
        r.draw(XY);
        
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}