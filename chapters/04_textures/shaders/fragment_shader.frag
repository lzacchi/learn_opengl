#version 460 core

layout(location = 0) in vec3 ourColor;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) uniform float mix_attenuation;

uniform sampler2D texture1;
uniform sampler2D texture2;

layout(location = 0) out vec4 FragColor;

void main() {
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mix_attenuation);
}
