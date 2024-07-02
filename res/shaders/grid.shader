#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
out vec2 uv_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
 
    uv_coords = uv;
};


#shader fragment
#version 330 core

in vec2 uv_coords;
out vec4 color;

#ifdef GL_ES
precision mediump float;
#endif
#define M_PI 3.1415926535897932384626433832795

void main()
{
    float grid_val = pow(tan(16 * M_PI * uv_coords.y + M_PI * 0.5), 2.0)/10;
    float grid_val2 = pow(tan(16 * M_PI * uv_coords.x + M_PI * 0.5), 2.0)/10;
    if (grid_val > 10.0 || grid_val2 > 10.0)
    {
        color = vec4(0.7f, 0.7f, 0.7f, 1.0f);
    }
    else
    {
        vec4 vertical = mix(vec4(0.7f, 0.7f, 0.7f, 0.0f), vec4(0.7f, 0.7f, 0.7f, 1.0f), clamp(grid_val - 9.0, 0.0, 1.0));
        vec4 horizontal = mix(vec4(0.7f, 0.7f, 0.7f, 0.0f), vec4(0.7f, 0.7f, 0.7f, 1.0f), clamp(grid_val2 - 9.0, 0.0, 1.0));
        color = max(horizontal, vertical);
    }
    
};