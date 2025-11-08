#include "GLGEGraphic/GLGEGraphic.h"
#include "GLGE_Core/GLGECore.h"

int main()
{
    Window win = Window("Hello World!", 600);
    win.setVSync(GLGE_VSYNC_ON);

    AssetHandle mesh = AssetManager::create<MeshAsset>(MeshAsset::import("assets/mesh/Suzane.fbx"));
    AssetHandle mesh2 = AssetManager::create<MeshAsset>(MeshAsset::import("assets/mesh/Cube.glb"));
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
    AssetManager::waitForLoad(mesh2);
    RenderMeshHandle rMesh2 = RenderMeshRegistry::create(&AssetManager::getAsset<MeshAsset>(mesh2)->mesh());

    Scene scene = "Main";
    Object obj = scene.createObject("Hello");
    obj->assignOrAdd<Renderer>(rMesh, &mat);
    Object obj2 = scene.createObject("Other");
    obj2->assignOrAdd<Renderer>(rMesh2, &mat);

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