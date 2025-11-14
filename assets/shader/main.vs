#version 450 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_norm;
layout (location = 2) in vec2 v_tex;

layout (location = 0) out vec3 f_pos;
layout (location = 1) out vec3 f_norm;
layout (location = 2) out vec2 f_tex;

struct CompressedTransform {
	float x;
	float y;
	float z;
	uint quat_version_i;
	uint quat_jk;
	float sx;
	float sy;
	float sz;
};

struct Quaternion {
    float w;
    float i;
    float j;
    float k;
};

layout (std430, binding = 0) readonly buffer buffer_Transforms {
    CompressedTransform transforms[];
};

uint compressFloat(float angle) {
    return uint(mod(angle * 0.5 + 0.5, 1.f) * 65535u);
}

float decompressFloat(uint angle) {
    return (float(angle) / 65535.f) * 2.f - 1.f;
}

uint low16(uint v)  { return v & 0xFFFFu; }
uint high16(uint v) { return (v >> 16) & 0xFFFFu; }

Quaternion decodeQuaternion(uint quat_version_i, uint quat_jk) {
    Quaternion q;
    uint x16 = high16(quat_version_i);
    uint y16 = low16(quat_jk);
    uint z16 = high16(quat_jk);

    float pos = float((quat_version_i >> 10) & 1);
    q.i = decompressFloat(x16);
    q.j = decompressFloat(y16);
    q.k = decompressFloat(z16);

    //re-construct the w component
    float wsq = 1.f - q.i*q.i - q.j*q.j - q.k*q.k;
    q.w = sqrt(abs(wsq)) * (pos*2.f-1.f);

    return q;
}

/**
 * Build a 3x3 rotation matrix from quaternion
 */
mat3 quatToMat3(Quaternion q) {
    float w = q.w;
    float x = q.i;
    float y = q.j;
    float z = q.k;

    //Precompute products
    float xx = x * x;
    float yy = y * y;
    float zz = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    //Columns of the rotation matrix
    vec3 col0 = vec3(1.0 - 2.0 * (yy + zz),
                     2.0 * (xy + wz),
                     2.0 * (xz - wy));

    vec3 col1 = vec3(2.0 * (xy - wz),
                     1.0 - 2.0 * (xx + zz),
                     2.0 * (yz + wx));

    vec3 col2 = vec3(2.0 * (xz + wy),
                     2.0 * (yz - wx),
                     1.0 - 2.0 * (xx + yy));

    return mat3(col0, col1, col2);
}

/**
 * Apply a specific transformation to the position
 */
void applyTransform(inout vec4 pos, inout vec3 normal, int transfIndex) {
    //apply the scaling
    pos.xyz *= vec3(transforms[transfIndex].sx, transforms[transfIndex].sy, transforms[transfIndex].sz);
    //apply the rotation
    Quaternion rot = decodeQuaternion(transforms[transfIndex].quat_version_i, transforms[transfIndex].quat_jk);
    mat3 rotMat = quatToMat3(rot);
    pos.xyz *= rotMat;
    //make sure to also rotate the normal
    normal *= rotMat;
    //apply the position
    pos.xyz += vec3(transforms[transfIndex].x, transforms[transfIndex].y, transforms[transfIndex].z);
}

const float FOV = 90.f;
const float FOV_Rad_h = (FOV * 0.0174532925199f) * 0.5f;
const float near = 0.1f;
const float far = 1000.f;
const mat4 proj = mat4(
    1./tan(FOV_Rad_h), 0, 0, 0,
    0, 1./tan(FOV_Rad_h), 0, 0, 
    0, 0, -(far + near) / (far - near), -(2.f*far*near) / (far - near),
    0, 0, -1, 0
);

void main() {
    vec4 p = vec4(v_pos, 1);
    vec3 norm = v_norm;
    applyTransform(p, norm, 0);
    gl_Position = p * proj;
    f_pos = v_pos;
    f_norm = norm;
    f_tex = v_tex;
}