#include "GLGEGraphic/GLGEGraphic.h"
#include "GLGE_Core/GLGECore.h"

int main()
{
    Window win = Window("Hello World!", 600);

    AssetHandle mesh = AssetManager::create<MeshAsset>(MeshAsset::import("assets/mesh/Suzane.fbx"));
    AssetHandle tex = AssetManager::create<TextureAsset>("assets/textures/cubeTexture.png", true, GLGE_TEXTURE_RGBA_H);
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
    Texture* textures[] = {AssetManager::getAsset<TextureAsset>(tex)->getTexture()};
    Material mat(&shader, textures, sizeof(textures)/sizeof(*textures), GLGE_VERTEX_LAYOUT_SIMPLE_VERTEX);
    AssetManager::waitForLoad(mesh);
    RenderMeshHandle rMesh = RenderMeshRegistry::create(&AssetManager::getAsset<MeshAsset>(mesh)->mesh());

    RenderPipeline pipe({{
            "Draw", 
            RenderPipelineStage{
                .type = GLGE_RENDER_PIPELINE_STAGE_SIMPLE_DRAW_RENDER_MESH,
                .data{.simpleDrawRenderMesh{.handle=rMesh,.material=&mat}}}
        }
    }, &win);

    pipe.record();
    glge_Shader_Compile();

    while (!win.isClosingRequested()) {
        glge_Graphic_MainTick();
        pipe.play();
    }
}