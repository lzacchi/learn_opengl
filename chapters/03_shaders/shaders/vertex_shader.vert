#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) uniform float h_offset;

layout(location = 0) out vec3 vertexColor;
layout(location = 1) out vec3 vertexPosition;

void main() {
    gl_Position = vec4(aPos.x + h_offset, -aPos.y, aPos.z, 1.0);
    vertexColor = aColor;
    vertexPosition = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
}