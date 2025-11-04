#include "GLGEGraphic/GLGEGraphic.h"
#include "GLGE_Core/GLGECore.h"

int main()
{
    String p = MeshAsset::import("assets/mesh/Suzane.fbx");
    AssetHandle mesh = AssetManager::create<MeshAsset>(p);

    Window win("Hello World!", uivec2(600,600));
    win.setVSync(GLGE_VSYNC_ON);

    AssetHandle tex = AssetManager::create<TextureAsset>("assets/textures/cubeTexture.png", true, GLGE_TEXTURE_RGBA_H);

    Shader shader = {
        ShaderStage{
            .sourceCode = File("assets/shader/main.vs",false).getContents(),
            .srcType = SHADER_SOURCE_GLSL,
            .stage = SHADER_STAGE_VERTEX
        },
        ShaderStage{
            .sourceCode = File("assets/shader/main.fs",false).getContents(),
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