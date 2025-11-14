#include "GLGEGraphic/GLGEGraphic.h"
#include "GLGE_Core/GLGECore.h"

#define PI_2 6.28318405151367188F

struct CustomMatData {
    vec4 color = vec4(1,0,0,1);
    float roughness = 0.8;
    float metallic = 0.f;
};

int main()
{
    Window win = Window("Hello World!", 600);
    win.setVSync(GLGE_VSYNC_ON);

    CustomMatData customMatData;
    Buffer material(&customMatData, sizeof(customMatData), GLGE_BUFFER_TYPE_UNIFORM);

    //CompressedTransform cTransf;
    //Buffer transforms(&cTransf, sizeof(cTransf), GLGE_BUFFER_TYPE_SHADER_STORAGE, 3);
    
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
            .sourceCode = File("assets/shader/tex.fs").getContents(),
            .srcType = SHADER_SOURCE_GLSL,
            .stage = SHADER_STAGE_FRAGMENT
        }};
    AssetManager::waitForLoad(tex);
    Material mat(&shader, AssetManager::getAsset<TextureAsset>(tex)->getTexture(), glge_Graphic_GetTransformBuffer(), GLGE_VERTEX_LAYOUT_SIMPLE_VERTEX);
    Buffer* buffers[] = {&material, glge_Graphic_GetTransformBuffer()};
    AssetManager::waitForLoad(mesh);
    RenderMeshHandle rMesh = RenderMeshRegistry::create(&AssetManager::getAsset<MeshAsset>(mesh)->mesh());
    AssetManager::waitForLoad(mesh2);
    RenderMeshHandle rMesh2 = RenderMeshRegistry::create(&AssetManager::getAsset<MeshAsset>(mesh2)->mesh());

    Scene scene = "Main";
    Object obj = scene.createObject("Hello", Transform(vec3(0,0,-3), Quaternion(), 1));
    obj->add<Renderer>(rMesh, &mat);
    //cTransf = CompressedTransform(*obj->get<Transform>());
    //transforms.write(&cTransf, sizeof(cTransf)*0, sizeof(cTransf));
    Object obj2 = scene.createObject("Other");
    //uint64_t offs = transforms.getSize();
    //transforms.resize(transforms.getSize() + sizeof(CompressedTransform));
    //cTransf = CompressedTransform(*obj2->get<Transform>());
    //transforms.write(&cTransf, offs, sizeof(cTransf));
    //obj2->add<Renderer>(rMesh2, &mat);

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
        float delta = M_PI_2 * pipe.getDelta();
        float halfDelta = delta * 0.5f;
        Quaternion quat{std::cos(halfDelta), std::sin(halfDelta), 0, 0};
        
        //obj->get<Transform>()->rot.vec = normalize((obj->get<Transform>()->rot * quat).vec);
        //cTransf = CompressedTransform(*obj->get<Transform>());
        //transforms.write(&cTransf, sizeof(cTransf) * 0, sizeof(cTransf));

        glge_Graphic_MainTick();
        pipe.play();
    }
}