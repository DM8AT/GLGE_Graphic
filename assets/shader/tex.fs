#version 450 core

layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec3 f_pos;
layout (location = 1) in vec3 f_norm;
layout (location = 2) in vec2 f_tex;

layout (binding = 0) uniform sampler2D tex;

const vec3 lightDir = normalize(vec3(0.5, 10, 0.5));
const float lightPower = 1.f;
const vec3 lightColor = vec3(250./255., 237./255., 201./255.);

void main() {
    vec3 col = texture(tex, f_tex).rgb;
    col *= lightColor * ( dot(lightDir, f_norm) * 0.5 + 0.5 ) * lightPower;
    FragColor = vec4(col,1);
}