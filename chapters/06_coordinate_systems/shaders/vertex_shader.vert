#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) uniform mat4 model;
layout(location = 4) uniform mat4 view;
layout(location = 5) uniform mat4 projection;
// layout(location = 2) uniform float h_offset;

layout(location = 0) out vec3 VertexColor;
layout(location = 1) out vec2 TexCoord;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    VertexColor = aColor;
    TexCoord    = aTexCoord;
}