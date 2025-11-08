#version 450 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_norm;
layout (location = 2) in vec2 v_tex;

layout (location = 0) out vec3 f_pos;
layout (location = 1) out vec3 f_norm;
layout (location = 2) out vec2 f_tex;

const float FOV = 90.f;
const float FOV_Rad_h = (FOV * 0.0174532925199f) * 0.5f;
const float near = 0.1f;
const float far = 1000.f;
const mat4 proj = mat4(
    tan(FOV_Rad_h), 0, 0, 0,
    0, tan(FOV_Rad_h), 0, 0, 
    0, 0, -(far + near) / (far - near), -(2.f*far*near) / (far - near),
    0, 0, -1, 0
);

void main() {
    gl_Position = vec4(v_pos + vec3(0,0,-3), 1) * proj;
    f_pos = v_pos;
    f_norm = v_norm;
    f_tex = v_tex;
}