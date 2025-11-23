#version 460 core

layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec3 f_pos;
layout (location = 1) in vec3 f_norm;
layout (location = 2) in vec2 f_tex;

layout (location = 3) flat in uint drawID;

layout (binding = 0) uniform sampler2D tex0;
layout (binding = 1) uniform sampler2D tex1;

const vec3 lightDir = normalize(vec3(0.5, 10, 0.5));
const float lightPower = 1.f;
const vec3 lightColor = vec3(250./255., 237./255., 201./255.);

layout (binding = 2, std430) readonly buffer buffer_TextureIDs {
    uint textureIDs[];
};

vec4 sampleTextureByID(uint id, vec2 uv) {
    if (id == 0) {return texture(tex0, uv);}
    else if (id == 1) {return texture(tex1, uv);}
    return vec4(0);
}

void main() {
    //get the texture
    uint tex = textureIDs[drawID];

    vec3 col = sampleTextureByID(tex, f_tex).rgb;
    col *= lightColor * ( dot(lightDir, f_norm) * 0.5 + 0.5 ) * lightPower;
    FragColor = vec4(col,1);
}