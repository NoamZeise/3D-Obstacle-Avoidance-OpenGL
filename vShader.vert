#version 330 core
layout (location = 0) in vec3 position;

out vec4 colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 shapeColour;
uniform float scale;

void main()
{
    colour = shapeColour;
    gl_Position = projection * view * model * vec4(position * scale, 1.0f);
}