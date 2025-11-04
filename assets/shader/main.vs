#version 450 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_norm;
layout (location = 2) in vec2 v_tex;

layout (location = 0) out vec3 f_pos;
layout (location = 1) out vec3 f_norm;
layout (location = 2) out vec2 f_tex;

void main() {
    gl_Position = vec4(v_pos.x, v_pos.y, 0, 1);
    f_pos = v_pos;
    f_norm = v_norm;
    f_tex = v_tex;
}