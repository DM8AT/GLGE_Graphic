#version 450 core

layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec3 f_pos;
layout (location = 1) in vec3 f_norm;
layout (location = 2) in vec2 f_tex;

layout (binding = 0) uniform sampler2D tex;

void main() {
    vec3 col = texture(tex, f_tex).rgb;
    col *= dot(f_norm, vec3(0,1,0))*0.5f+0.5f;
    FragColor = vec4(col,1);
}