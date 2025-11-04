#include "GLGEGraphic/GLGEGraphic.h"
#include "GLGE_Core/GLGECore.h"

int main()
{
    String p = MeshAsset::import("assets/mesh/Suzane.fbx");
    AssetHandle mesh = AssetManager::create<MeshAsset>(p);

    Window win("Hello World!", uivec2(600,600));
    win.setVSync(GLGE_VSYNC_ON);

    AssetHandle tex = AssetManager::create<TextureAsset>("assets/textures/cubeTexture.png", true, GLGE_TEXTURE_RGBA_H);

    const char* vertexShader = R"(
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
})";

    const char* fragmentShader = R"(
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
})";

    Shader shader = {
        ShaderStage{
            .sourceCode = vertexShader,
            .srcType = SHADER_SOURCE_GLSL,
            .stage = SHADER_STAGE_VERTEX
        },
        ShaderStage{
            .sourceCode = fragmentShader,
            .srcType = SHADER_SOURCE_GLSL,
            .stage = SHADER_STAGE_FRAGMENT
        }
    };

    AssetManager::waitForLoad(tex);
    Texture* textures[] = {AssetManager::getAsset<TextureAsset>(tex)->getTexture()};
    Material mat(&shader, textures, sizeof(textures)/sizeof(*textures), GLGE_VERTEX_LAYOUT_SIMPLE_VERTEX);
    AssetManager::waitForLoad(mesh);
    MeshAsset* m = AssetManager::getAsset<MeshAsset>(mesh);
    Mesh& m_mesh = m->mesh();
    RenderMesh rMesh(&m_mesh, &mat);

    RenderPipeline pipe({
        {"Draw", RenderPipelineStage{
            .type = GLGE_RENDER_PIPELINE_STAGE_SIMPLE_DRAW_RENDER_MESH,
            .data{.simpleDrawRenderMesh=&rMesh}
        }}
    }, &win);
    
    glge_Shader_Compile();
    pipe.record();

    while (!win.isClosingRequested()) {
        static uivec3 color = uivec3(0,0,0);

        glge_Graphic_MainTick();
        pipe.play();
        win.setClearColor(vec4(color.r, color.g, color.b, 255.f) / 255.f);
        color.r = (color.r + 1) % 256;
        color.g = (color.g + 3) % 256;
        color.b = (color.b + 5) % 256;
        pipe.record();
    }
}