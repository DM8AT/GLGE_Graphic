#include "GLGEGraphic/GLGEGraphic.h"
#include "GLGE_Core/GLGECore.h"

/*
PLANING

Buffer Layout:
Vertex & Index buffer (ARRAY_BUFFER and ELEMENT_ARRAY_BUFFER):
 - global buffer
 - index buffer: index_t (alias for uint32_t) []
 - vertex buffer: fully opaque array, meaning only given by the vertex layout
Mesh Buffer (SHADER_STORAGE_BUFFER):
 - Per Mesh (indexed by the mesh index)
 - Holds information about where the mesh is stored
 - Stores the byte offset into the IBO where the mesh indices start (1 uint32_t)
 - Stores the amount of indices used by this object (1 uint32_t)
 - Stores the value to add to all vertices (base vertex value) (1 int32_t)
 => Each element has a size of 12 bytes (padded to 16 bytes for std430)
 -> At most 8.39 Million meshes can be loaded at once (OK, that will never actually fit in RAM with the Vertex / Index buffer)
AABB buffer (SHADER_STORAGE_BUFFER):
 - Per Mesh (indexed by the mesh index)
 - Stores Axis-Aligned bounding boxes for all meshes
   Each AABB is stored as follows: 
     - 1 vec3 for the minimum values (padded to 1 vec4)
     - 1 vec3 for the maximum values (padded to 1 vec4)
   => 6 Floats per entry (24 bytes)
   This is padded to 32 bytes because a vec3 is interpreted as a vec4 by std430
Batch Buffer (SHADER_STORAGE_BUFFER):
 - Holds information about what meshes are currently drawn
 - Computed and uploaded during command buffer recording (loading operation, even in OpenGL with custom command buffer)
 - Holds an object handle that determines to which object the mesh is attached (1 uint32_t)
 - Holds a mesh handle to determine which mesh this is (1 uint32_t)
   => Each entry has a size of 8 bytes
    -> with a maximum size of 128 MiB the maximum amount of meshes drawn per batch is 16.77 Million
Draw Buffer (SHADER_STORAGE_BUFFER / GL_DRAW_INDIRECT_BUFFER):
 - Holds information about what to actually draw
 - Used for calling something like `glMultiDrawElementsIndirect` (while bound to `GL_DRAW_INDIRECT_BUFFER`)
 Layout: 
    Only the structure assumed by the draw command, which is:
    ```
    typedef  struct {
        uint  count;
        uint  instanceCount;
        uint  firstIndex;
        int  baseVertex;
        uint  baseInstance;
    } DrawElementsIndirectCommand;
    ```
    => Each element has the size of 5*uint32_t (20 Bytes), the padding MUST be 0
    Because only integer types are used, no padding is applied. 
Transform / Version buffer (SHADER_STORAGE_BUFFER) (3 buffers exist for tripple-Buffering : 
  1 buffer for CPU writing, 1 that is passed to GPU, 1 that is consumed by GPU):
 - Per object (object index is used to index the data)
 - Transforms are stored as follows:
   3x float for the position (x, y, z)
   2x uint32_t (GLSL does not support uint16_t)
      interpreted as 4x uint16_t : (rx, ry, rz, version)
      rx, ry, rz are euler angles (Axis representation)
   3x float for the scale
   => Entry size of 8 * 4 Bytes (32 Bytes total, 50% of a 4x4 matrix)
   To save on buffers and RAM the version of an object (used for version-handles to ensure index safety)
   is stored in the empty uint16_t of the rotation - the version is a uint16_t so it is a perfect fit and 
   basically stored for free. 
Per-Object material data (SHADER_STORAGE_BUFFER):
 - Per object (object index is used to index the data)
 - Contents of each element are user-defined
 - Each entry has a constant size of 32 bytes
[Over all buffers a Object uses a combined amount of 2*32 = 64 bytes]
Material buffer (multiple may exist) (UNIFORM_BUFFER or SHADER_STORAGE_BUFFER):
 - Changes per material (not an array, just data)
 - Opaque from the engine side (depends on the material, which owns the shader ->
   Lets the user pass custom data to the shader)
 - A buffer that holds per-material constants
Camera Buffer (UNIFORM_BUFFER):
 - Holds information for the camera
   - FOV in radians (1 float)
     - if the FOV is 0 the camera is interpreted as orthographic
   - near / far clipping plane (1 float each)
   - Transform (encoded like in the transform buffer) => 32 bytes
 - Stores the aspect ratio of the current window (1 float)
 - Stores the position of the window (2 uint32_t's)
 - Stores the window's size (2 uint32_t's)
 - Stores the size of the display the window is on (2 uint32_t's)
 => Constant size of 72 bytes (requires padding to 80 bytes, so add 2 floats (8 bytes))
 This is a buffer so the data is easy to access everywhere and easy to integrate cross-API
 This also minimizes command buffer recording (for vulkan / Direct X12) as it is buffered data

Computing the sin / cos for the Euler-Angles of the transform:
1. Do NOT unpack the data
2. use the angle as an index into a lookup table
   LUT:
   const uint32_t[] table in constant shader memory
   Each entry stores 4 sin or cos values, so the index is value / 4 (or value >> 2)
   The entries are actually uint8_t's (packed because GLSL does not have native uint8_t support)
   The byte is extracted using the lower 3 bits of the index:
    `uint s_uint = bitfieldExtract(LUT_sin[angle >> 2], (angle & 0b11) << 2, 8);`
   The uint8_t value is then converted to the final sin / cos value:
    `float s = float(s_uint) / 127. - 1.;`
  The resolution of 256 possible values for sin / cos values results in an error of about 0.23° - too little to be noticed by humans
  This is OK since the transform is only used for rendering and not for physics

=> Cap out at 4.19 Million objects
   This means an object index (just a uint32_t) is build as follows:
   22 bits for the index (i) and 10 bits for the version (v):
   vvvvvvvvvviiiiiiiiiiiiiiiiiiiiii

RENDER PIPELINE

1. During recording, run the batching and create all batch buffers required to draw the entire scene
2. During rendering, iterate over all batches
3. For each batch, bind the correct batch buffer as well as the correct material buffers. Also set up the 
   Render pipeline state (blending, depth test, depth write, ...) correctly for the material. 
4. Cull the current batch using a GPU compute shader. This fills the draw buffer with the correct values. 
5. use `glMultiDrawElementsIndirect` to draw the objects - the object count is assumed to be without culling. 
   Because of this the CPU can remain oblivious to the culling step - this avoid GPU readbacks and GPU / CPU sync 
   steps. 

*/

int main()
{
    Window win = Window("Hello World!", 600);
    win.setVSync(GLGE_VSYNC_ON);

    AssetHandle mesh = AssetManager::create<MeshAsset>(MeshAsset::import("assets/mesh/Suzane.fbx"));
    AssetHandle mesh2 = AssetManager::create<MeshAsset>(MeshAsset::import("assets/mesh/Cube.glb"));
    AssetHandle tex = AssetManager::create<TextureAsset>("assets/textures/cubeTexture.png", false, GLGE_TEXTURE_RGB, 
                                                         TEXTURE_FILTER_MODE_NEAREST, 16.f);
    Shader shader = {
        ShaderStage{
            .sourceCode = File("assets/shader/main.vs").getContents(),
            .srcType = SHADER_SOURCE_GLSL,
            .stage = SHADER_STAGE_VERTEX
        },
        ShaderStage{
            .sourceCode = File("assets/shader/main.fs").getContents(),
            .srcType = SHADER_SOURCE_GLSL,
            .stage = SHADER_STAGE_FRAGMENT
        }};
    AssetManager::waitForLoad(tex);
    Material mat(&shader, AssetManager::getAsset<TextureAsset>(tex)->getTexture(), GLGE_VERTEX_LAYOUT_SIMPLE_VERTEX);
    AssetManager::waitForLoad(mesh);
    RenderMeshHandle rMesh = RenderMeshRegistry::create(&AssetManager::getAsset<MeshAsset>(mesh)->mesh());
    AssetManager::waitForLoad(mesh2);
    RenderMeshHandle rMesh2 = RenderMeshRegistry::create(&AssetManager::getAsset<MeshAsset>(mesh2)->mesh());

    Scene scene = "Main";
    Object obj = scene.createObject("Hello");
    obj->assignOrAdd<Renderer>(rMesh, &mat);
    Object obj2 = scene.createObject("Other");
    //obj2->assignOrAdd<Renderer>(rMesh2, &mat);

    RenderPipeline pipe({{
            "Draw", 
            RenderPipelineStage{
                .type = GLGE_RENDER_PIPELINE_STAGE_DRAW_SCENE,
                .data{.drawScene{.scene = &scene}}}
        }
    }, &win);

    pipe.record();
    glge_Shader_Compile();

    while (!win.isClosingRequested()) {
        glge_Graphic_MainTick();
        pipe.play();
    }
}