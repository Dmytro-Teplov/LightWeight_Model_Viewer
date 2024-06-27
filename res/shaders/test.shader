#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
out vec3 colors;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
 
    colors = vec3(uv.x,uv.y,1.0f);
};


#shader fragment
#version 330 core

in vec3 colors;
out vec4 color;

#ifdef GL_ES
precision mediump float;
#endif


void main()
{
    color = vec4(colors,1.0f);
};