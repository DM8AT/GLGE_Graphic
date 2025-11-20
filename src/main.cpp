#include "GLGEGraphic/GLGEGraphic.h"
#include "GLGE_Core/GLGECore.h"

struct CameraController {
    float speed = 10.f;
    float sens = 0.2f;
    float delta = 0.f;
};

void cameraController(Transform& transf, const CameraController& settings) {
    //store the current pitch and yaw of the camera
    static vec2 rot = 0.f;

    //compute the normalized direction vector
    Quaternion res = transf.rot.conjugate() * vec3(0,0,-1) * transf.rot;
    float d = settings.speed * settings.delta;
    vec3 dir = normalize(vec3(res.i, 0, res.k)) * d;

    //store the new position
    vec3 nPos = transf.pos;

    //forward
    if (key_isSignaled(GLGE_KEY_w, glge_Graphic_GetCurrentKeys())) 
    {nPos += dir;}
    //backward
    if (key_isSignaled(GLGE_KEY_s, glge_Graphic_GetCurrentKeys())) 
    {nPos -= dir;}
    //left
    if (key_isSignaled(GLGE_KEY_a, glge_Graphic_GetCurrentKeys())) 
    {nPos += vec3(dir.z, 0, -dir.x);}
    //right
    if (key_isSignaled(GLGE_KEY_d, glge_Graphic_GetCurrentKeys())) 
    {nPos -= vec3(dir.z, 0, -dir.x);}
    //up
    if (key_isSignaled(GLGE_KEY_SPACE, glge_Graphic_GetCurrentKeys()))
    {nPos.y += d;}
    //down
    if (key_isSignaled(GLGE_KEY_LEFT_SHIFT, glge_Graphic_GetCurrentKeys())) 
    {nPos.y -= d;}

    //compute by how much to rotate
    vec2 deltaRot(
        glge_Graphic_GetRelativeMouse()->pixelPos.y * settings.delta * settings.sens,
        glge_Graphic_GetRelativeMouse()->pixelPos.x * settings.delta * settings.sens);
    //rotate the rotation
    rot += deltaRot;
    //compute the new rotation quaternion
    transf.rot = Quaternion(vec3(rot, 0));

    //store the new position
    transf.pos = nPos;
}

int main()
{
    Window win = Window("Hello World!", 600);
    win.setVSync(GLGE_VSYNC_OFF);
    win.setRelativeMouseMode(true);

    //check if mice exists. At least one mouse is required.
    if (glge_Graphic_GetMiceCount() == 0) {
        std::cerr << "At least one mouse is required for the program\n";
        return 1;
    }

    for (uint32_t i = 0; i < glge_Graphic_GetMiceCount(); ++i) {
        std::cout << glge_Graphic_GetMouse(i)->name << "\n";
    }

    Scene scene = "Main";

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

    Object camera = scene.createObject<CameraController>("Camera", Transform(vec3(0,0,3)));
    camera->add<Camera>(1.570796f, 0.1f, 1000.f, &win);

    Buffer* buffers[] = {GLGE_SKIP_SLOT(GLGE_BUFFER_TYPE_SHADER_STORAGE), glge_Graphic_GetTransformBuffer(), camera->get<Camera>()->getBuffer()};
    Texture* textures[] = { AssetManager::getAsset<TextureAsset>(tex)->getTexture() };
    Material mat(&shader, textures, sizeof(textures)/sizeof(*textures), buffers, sizeof(buffers)/sizeof(*buffers), GLGE_VERTEX_LAYOUT_SIMPLE_VERTEX);

    AssetManager::waitForLoad(mesh);
    RenderMeshHandle rMesh = RenderMeshRegistry::create(&AssetManager::getAsset<MeshAsset>(mesh)->mesh());
    AssetManager::waitForLoad(mesh2);
    RenderMeshHandle rMesh2 = RenderMeshRegistry::create(&AssetManager::getAsset<MeshAsset>(mesh2)->mesh());

    Shader cmpShader = {
        ShaderStage{
            .sourceCode = File("assets/shader/draw.cs").getContents(),
            .srcType = SHADER_SOURCE_GLSL,
            .stage = SHADER_STAGE_COMPUTE
        }};
    Compute compute(cmpShader, {
        GLGE_SKIP_SLOT(GLGE_BUFFER_TYPE_SHADER_STORAGE), GLGE_SKIP_SLOT(GLGE_BUFFER_TYPE_SHADER_STORAGE), glge_Graphic_GetMeshBuffer()
    }, {});
    Compute* renderList[] = { &compute };

    Object obj = scene.createObject("Hello", Transform(vec3(0,0,0)));
    obj->add<Renderer>(rMesh, &mat);
    Object obj2 = scene.createObject("Other", Transform(vec3(1.2,2,-4)));
    obj2->add<Renderer>(rMesh2, &mat);

    RenderPipeline pipe({{
            "Draw", 
            RenderPipelineStage{
                .type = GLGE_RENDER_PIPELINE_STAGE_DRAW_SCENE,
                .data{.drawScene{
                    .scene = &scene,
                    .batchShader=(void**)renderList,
                    .batchShaderCount=(sizeof(renderList) / sizeof(*renderList))
                }}}
        }
    }, &win);

    pipe.setIterationRate(120);

    pipe.record();
    glge_Shader_Compile();

    while (!win.isClosingRequested()) {
        float delta = M_PI_2 * pipe.getDelta();
        float halfDelta = delta * 0.5f;
        Quaternion quat{std::cos(halfDelta), std::sin(halfDelta), 0, 0};
        Quaternion quat2{std::cos(halfDelta), 0, std::sin(halfDelta), 0};
        
        obj->get<Transform>()->rot.vec = normalize((obj->get<Transform>()->rot * quat).vec);
        obj->get<Renderer>()->reupload();

        camera->get<CameraController>()->delta = pipe.getDelta();
        camera->get<Camera>()->update();

        if (key_isSignaled(GLGE_KEY_F11, glge_Graphic_GetPressedKeys())) {
            win.setFullscreen(!win.getSettings().fullscreen);
        }

        scene.forAllObjects(cameraController, false);
        glge_Graphic_MainTick();
        pipe.play();
    }
}