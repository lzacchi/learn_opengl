#version 460 core

layout(location = 0) in vec3 ourColor;
layout(location = 1) in vec3 vertexPosition;

layout(location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(vertexPosition, 1.0);
}