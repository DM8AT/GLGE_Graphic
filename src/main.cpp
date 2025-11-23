#include "GLGEGraphic/GLGEGraphic.h"
#include "GLGE_Core/GLGECore.h"
//add ImGui
#include "imgui.h"
#include "GLGE_ImGuiImpl.h"

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

static void imgui_draw(void*) noexcept {
    static bool button = false;
    static char buff[128] {'\0'};
    ImGui::Begin("Hello World!");
    ImGui::Text("This is a test");
    ImGui::Checkbox("Button: ", &button);
    if (button) {
        ImGui::Button("Button");
        ImGui::InputText("Write somethings", buff, sizeof(buff));
    }
    ImGui::End();
}

int main()
{
    Window win = Window("GLGE Example", 600);
    win.setVSync(GLGE_VSYNC_OFF);
    win.setRelativeMouseMode(true);

    Texture renderTarget(TextureStorage{
        .extent = win.getSize(),
        .isHDR = false,
        .channels = 3,
        .data = nullptr,
    }, GLGE_TEXTURE_RGB_H, GLGE_FILTER_MODE_NEAREST, 0.f, GLGE_TEXTURE_SAMPLE_X16);
    Texture depthBuffer(TextureStorage{
        .extent = win.getSize(),
        .isHDR = true,
        .channels = 1,
        .data = nullptr
    }, GLGE_TEXTURE_DEPTH_32, GLGE_FILTER_MODE_NEAREST, 0.f, GLGE_TEXTURE_SAMPLE_X16);
    Framebuffer fbuff({&renderTarget, &depthBuffer});

    //init the ImGui extension
    glge_ImGui_init(win);

    //check if mice exists. At least one mouse is required.
    if (glge_Graphic_GetMiceCount() == 0) {
        std::cerr << "At least one mouse is required for the program\n";
        return 1;
    }
    
    Scene scene = "Main";

    AssetHandle mesh = AssetManager::create<MeshAsset>(MeshAsset::import("assets/mesh/Suzane.fbx"));
    AssetHandle mesh2 = AssetManager::create<MeshAsset>(MeshAsset::import("assets/mesh/Cube.glb"));
    AssetHandle tex = AssetManager::create<TextureAsset>("assets/textures/cubeTexture.png", false, GLGE_TEXTURE_RGB, 
                                                         GLGE_FILTER_MODE_NEAREST, 16.f);
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
    camera->add<Camera>(1.570796f, 0.1f, 1000.f, RenderTarget(&fbuff, GLGE_FRAMEBUFFER));

    Buffer* buffers[] = {GLGE_SKIP_SLOT(GLGE_BUFFER_TYPE_SHADER_STORAGE), glge_Graphic_GetTransformBuffer(), GLGE_SKIP_SLOT(GLGE_BUFFER_TYPE_UNIFORM)};
    Texture* textures[] = { AssetManager::getAsset<TextureAsset>(tex)->getTexture(), };
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
            "Clear Color",
            RenderPipelineStage{
                .type = GLGE_RENDER_PIPELINE_CLEAR,
                .data{.clear{
                    .fbuff = &fbuff,
                    .value = win.getClearColor(),
                    .type = GLGE_CLEAR_COLOR,
                    .attachment = 0
                }}
            }
        }, {
            "Clear Depth",
            RenderPipelineStage{
                .type = GLGE_RENDER_PIPELINE_CLEAR,
                .data{.clear{
                    .fbuff = &fbuff,
                    .value = vec4(1),
                    .type = GLGE_CLEAR_DEPTH
                }}
            }
        }, {
            "Draw", 
            RenderPipelineStage{
                .type = GLGE_RENDER_PIPELINE_STAGE_DRAW_SCENE,
                .data{.drawScene{
                    .scene = &scene,
                    .camera = camera->get<Camera>(),
                    .batchShader=(void**)renderList,
                    .batchShaderCount=(sizeof(renderList) / sizeof(*renderList))
                }}}
        }, {
            "ImGui - Start",
            RenderPipelineStage{
                .type = GLGE_RENDER_PIPELINE_STAGE_CUSTOM, 
                .data{.customStage{
                    .custom_func = glge_ImGui_FrameStart,
                    .userData = nullptr
                }}}
        }, {
            "ImGui - Draw",
            RenderPipelineStage{
                .type = GLGE_RENDER_PIPELINE_STAGE_CUSTOM,
                .data{.customStage{
                    .custom_func = imgui_draw,
                    .userData = nullptr
                }}}
        }, {
            "ImGui - End",
            RenderPipelineStage{
                .type = GLGE_RENDER_PIPELINE_STAGE_CUSTOM,
                .data{.customStage{
                    .custom_func = glge_ImGui_FrameEnd,
                    .userData = &win
                }}}
        }, {
            "Copy to main frame", 
            RenderPipelineStage{
                .type = GLGE_RENDER_PIPELINE_BLIT,
                .data{.blit{
                    .from{RenderTarget{&fbuff, GLGE_FRAMEBUFFER}, win.getSize(), uivec2(0,0)},
                    .to{RenderTarget{&win, GLGE_WINDOW}, win.getSize(), uivec2(0,0)},
                    .filter = GLGE_FILTER_MODE_NEAREST,
                    .copyColor = true,
                    .copyDepth = false,
                    .copyStencil = false
                }
                }
            }
        }
    }, &win);

    pipe.setIterationRate(120);

    glge_Graphic_MainTick();
    glge_Shader_Compile();
    pipe.record();

    std::vector<double> fpsAverage;
    fpsAverage.reserve(60);

    while (!win.isClosingRequested()) {
        float delta = M_PI_2 * pipe.getDelta();
        float halfDelta = delta * 0.5f;
        Quaternion quat{std::cos(halfDelta), std::sin(halfDelta), 0, 0};
        Quaternion quat2{std::cos(halfDelta), 0, std::sin(halfDelta), 0};
        
        obj->get<Transform>()->rot.vec = normalize((obj->get<Transform>()->rot * quat).vec);
        obj->get<Renderer>()->reupload();

        camera->get<CameraController>()->delta = (win.getSettings().relative) ? pipe.getDelta() : 0.f;
        camera->get<Camera>()->update();

        if (key_isSignaled(GLGE_KEY_F11, glge_Graphic_GetPressedKeys())) 
        {win.setFullscreen(!win.getSettings().fullscreen);}
        
        if (key_isSignaled(GLGE_KEY_ESCAPE, glge_Graphic_GetPressedKeys()))
        {win.setRelativeMouseMode(false);}
        if (mouse_isPressed(GLGE_MOUSE_LEFT, &glge_Graphic_GetMouse(0)->pressed))
        {win.setRelativeMouseMode(true);}

        scene.forAllObjects(cameraController, false);
        glge_Graphic_MainTick();

        //handle window resizing
        if (win.didResize()) {
            //update the sizes of the textures
            renderTarget.resizeAndClear(win.getSize());
            depthBuffer.resizeAndClear(win.getSize());
            //re-create the framebuffer
            fbuff.recreate();

            //update the render pipeline
            RenderPipelineStageData& data = pipe.getStage("Copy to main frame").data;
            data.blit.from.extend = win.getSize();
            data.blit.to.extend = win.getSize();

            //re-record the pipeline
            pipe.record();

            win.handledResize();
        }

        pipe.play();

        //record the FPS
        fpsAverage.push_back(1. / pipe.getDelta());
        //check if the buffer is full
        if (fpsAverage.size() == fpsAverage.capacity()) {
            //average out the fps
            double fps = 0;
            for (auto f : fpsAverage) {
                fps += f / (double)fpsAverage.size();
            }
            fpsAverage.clear();
            //set the window's title to include the FPS
            win.rename(String("GLGE Example | FPS: ") + std::to_string(fps));
        }
    }

    //shutdown the ImGui extension
    glge_ImGui_Shutdown();
}