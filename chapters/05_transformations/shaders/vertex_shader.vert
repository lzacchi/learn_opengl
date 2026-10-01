#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) uniform mat4 transform;
// layout(location = 2) uniform float h_offset;

layout(location = 0) out vec3 VertexColor;
layout(location = 1) out vec2 TexCoord;

void main() {
    gl_Position = transform * vec4(aPos, 1.0f);
    VertexColor = aColor;
    TexCoord    = aTexCoord;
}