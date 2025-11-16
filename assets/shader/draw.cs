#version 450 core

layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Object {
    uint objHandle;
    uint meshIndex;
};

struct DrawInfo {
    uint  count;
    uint  instanceCount;
    uint  firstIndex;
    int   baseVertex;
    uint  baseInstance;
};

struct MeshInfo {
    uint indexOffset;
    uint indexCount;
    int  vertexOffset;
    uint zero;
};

layout (binding = 0) buffer buffer_Objects {
    Object objects[];
};

layout (binding = 1) buffer buffer_DrawBuffer {
    DrawInfo draw[];
};

layout (binding = 2) buffer buffer_MeshInfo {
    MeshInfo meshInfo[];
};

void main() {
    //get the index to use
    uint index = gl_GlobalInvocationID.x;
    //sanity check the index
    if (index > objects.length()) {return;}

    //this is a valid object. 
    //just draw it. 

    //first, get the object info
    Object obj = objects[index];

    //then, get the mesh info
    MeshInfo mesh = meshInfo[obj.meshIndex];

    //fill out the draw info
    draw[index] = DrawInfo(
        mesh.indexCount,
        1,
        mesh.indexOffset,
        mesh.vertexOffset,
        0
    );
}