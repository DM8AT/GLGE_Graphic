/**
 * @file FirstPersonExample.cpp
 * @author DM8AT
 * @brief implement a simple first person example for GLGE
 * @version 0.1
 * @date 2025-11-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the function to define
#include "FirstPersonExample.h"

/**
 * @brief a structure to store settings for a camera controller
 */
struct CameraController {
    //the speed of the camera in units per second
    float speed = 10.f;
    //the mouse sensitivity for the camera
    float sens = 0.2f;
    //the current delta time in seconds
    float delta = 0.f;
};

/**
 * @brief a function that is used to run a camera controller
 * 
 * @param transf a reference to the transform of the object
 * @param settings the camera controller settings
 */
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
    //clamp the rotation on the x axis
    rot.x = (rot.x < -1.570796f) ? -1.570796f : ((rot.x > 1.570796f) ? 1.570796f : rot.x);
    //compute the new rotation quaternion
    transf.rot = Quaternion(vec3(rot, 0));

    //store the new position
    transf.pos = nPos;
}

//create the main target constructor
FirstPersonExample::MainTarget::MainTarget(FirstPersonExample& example) noexcept
    //create the main color texture
:   color{TextureStorage{
        .extent = example.m_win.getSize()*example.SUPER_SAMPLE,
        .isHDR = true,
        .channels = 3,
        .data = nullptr
    }, GLGE_TEXTURE_RGB_H, GLGE_FILTER_MODE_NEAREST, 0.f, example.SAMPLES},
    //create the main depth texture
    depth{TextureStorage{
        .extent = example.m_win.getSize()*example.SUPER_SAMPLE,
        .isHDR = true,
        .channels = 1,
        .data = nullptr
    }, GLGE_TEXTURE_DEPTH_32, GLGE_FILTER_MODE_NEAREST, 0.f, example.SAMPLES},
    //create the framebuffer
    framebuffer{{&color, &depth}},
    //store the example
    self(example)
{}

//handle what needs to be done to resize the main render target
void FirstPersonExample::MainTarget::resize() {
    //store the new size
    uivec2 newSize = self.m_win.getSize() * self.SUPER_SAMPLE;
    //resize all the textures
    color.resizeAndClear(newSize);
    depth.resizeAndClear(newSize);
    //re-create the framebuffer
    framebuffer.recreate();

    //update the size for the blit command
    self.m_pipe.getStage("Main Blit").data.blit.from.extend = newSize;
    self.m_pipe.getStage("Main Blit").data.blit.to.extend = self.m_win.getSize();
}

//implement the construction of the first person example
FirstPersonExample::FirstPersonExample() 
    //create the window - this also sets up the OpenGL context
:   m_win("GLGE - First person example", 600), 
    //setup the main render target
    m_mainTarget(*this),

    //load the batch shader
    m_batchShader{{ShaderStage{
        .sourceCode = File("assets/shader/draw.cs").getContents(),
        .srcType = SHADER_SOURCE_GLSL,
        .stage = SHADER_STAGE_COMPUTE
    }}},
    //create the batch compute element
    m_batchCompute{m_batchShader, 
        {GLGE_SKIP_SLOT(GLGE_BUFFER_TYPE_SHADER_STORAGE), GLGE_SKIP_SLOT(GLGE_BUFFER_TYPE_SHADER_STORAGE), glge_Graphic_GetMeshBuffer()}, 
        {}
    },

    //setup the main render pipeline
    m_pipe{{{
        "Clear - Main Color",
        RenderPipelineStage{
            .type = GLGE_RENDER_PIPELINE_CLEAR,
            .data{.clear{
                .fbuff = &m_mainTarget.framebuffer,
                .value = m_win.getClearColor(),
                .type = GLGE_CLEAR_COLOR,
                .attachment = 0
            }}
        }
    }, {
        "Clear - Main Depth",
        RenderPipelineStage{
            .type = GLGE_RENDER_PIPELINE_CLEAR,
            .data{.clear{
                .fbuff = &m_mainTarget.framebuffer,
                .value{1},
                .type = GLGE_CLEAR_DEPTH,
                .attachment = 0
            }}
        }
    }, {
        "Main Render Pass",
        RenderPipelineStage{
            .type = GLGE_RENDER_PIPELINE_STAGE_DRAW_SCENE,
            .data{.drawScene{
                .scene = &m_scene, 
                .camera = nullptr, //will be set while finalizing the render pipeline
                .batchShader = (void**)m_batchList,
                .batchShaderCount = sizeof(m_batchList) / sizeof(*m_batchList)
            }}
        }
    }, {
        "Main Blit",
        RenderPipelineStage{
            .type = GLGE_RENDER_PIPELINE_BLIT,
            .data{.blit{
                .from{RenderTarget(&m_mainTarget.framebuffer, GLGE_FRAMEBUFFER), m_win.getSize()*SUPER_SAMPLE, 0u},
                .to{RenderTarget(&m_win, GLGE_WINDOW), m_win.getSize(), 0u},
                .filter = GLGE_FILTER_MODE_LINEAR,
                .copyColor = true,
                .copyDepth = false,
                .copyStencil = false
            }}
        }
    }}, &m_win},
    //load all required assets
    m_handleStorage(*this),
    //load the shader
    m_shader{{
    ShaderStage{
        .sourceCode = File("assets/shader/main.vs").getContents(),
        .srcType = SHADER_SOURCE_GLSL,
        .stage = SHADER_STAGE_VERTEX
    }, 
    ShaderStage{
        .sourceCode = File("assets/shader/main.fs").getContents(),
        .srcType = SHADER_SOURCE_GLSL,
        .stage = SHADER_STAGE_FRAGMENT
    }}},
    //create the buffer for the material data
    m_meshMaterialData(m_meshMaterial, sizeof(m_meshMaterial)/sizeof(*m_meshMaterial), GLGE_BUFFER_TYPE_SHADER_STORAGE),
    //create the scene
    m_scene{"Main Scene"}
{
    //load the scene
    loadScene();
    //finish the render pipeline
    finalizeRenderPipeline();

    //setup the window flags
    m_win.setVSync(GLGE_VSYNC_ON);
    m_win.setRelativeMouseMode(true);

    //do an update and compile all the shaders
    glge_Shader_Compile();
    glge_Graphic_MainTick();
    //record the pipeline
    m_pipe.record();
}

void FirstPersonExample::run() {
    //run while the window is open
    while (!m_win.isClosingRequested()) {
        //check if the mouse capture should start / stop
        if (key_isSignaled(GLGE_KEY_ESCAPE, glge_Graphic_GetPressedKeys()))
        {m_win.setRelativeMouseMode(false);}
        if (mouse_isPressed(GLGE_MOUSE_LEFT, &glge_Graphic_GetGlobalMouse()->pressed))
        {m_win.setRelativeMouseMode(true);}

        //check for F11 to toggle fullscreen
        if (key_isSignaled(GLGE_KEY_F11, glge_Graphic_GetPressedKeys()))
        {m_win.setFullscreen(!m_win.getSettings().fullscreen);}

        //update the delta time
        m_camera->get<CameraController>()->delta = (m_win.getSettings().relative) ? m_pipe.getDelta() : 0.f;
        //run the first person controller
        m_scene.forAllObjects(cameraController, false);
        //update the camera
        m_camera->get<Camera>()->update();

        //rotate suzan a bit
        m_suzan->get<Transform>()->rot.rotateThis(Quaternion(vec3(0, m_pipe.getDelta(), 0)));
        m_suzan->get<Renderer>()->reupload();

        //run a main tick
        glge_Graphic_MainTick();

        //check for window resizing
        if (m_win.didResize()) {
            //notify the main target
            m_mainTarget.resize();
            //re-record the pipeline
            m_pipe.record();

            //window was resized
            m_win.handledResize();
        }

        //run the graphic pipeline
        m_pipe.play();
    }
}

FirstPersonExample::HandleStorage::HandleStorage(FirstPersonExample& example)
 : self(example)
{
    //load the asset for the cube texture
    texture_Cube = AssetManager::create<TextureAsset>("assets/textures/cubeTexture.png", false, GLGE_TEXTURE_RGB, GLGE_FILTER_MODE_NEAREST, 16.f);
    //load the asset for the grass texture
    texture_Grass = AssetManager::create<TextureAsset>("assets/textures/grass.png", false, GLGE_TEXTURE_RGB, GLGE_FILTER_MODE_NEAREST, 16.f);
    //load the asset for the cube
    mesh_Cube = AssetManager::create<MeshAsset>(MeshAsset::import("assets/mesh/Cube.glb"));
    //load the asset for the terrain
    mesh_Terrain = AssetManager::create<MeshAsset>(MeshAsset::import("assets/mesh/Terrain.fbx"));
    //load the asset for suzan
    mesh_Suzan = AssetManager::create<MeshAsset>(MeshAsset::import("assets/mesh/Suzan.fbx"));
}

void FirstPersonExample::HandleStorage::waitTillLoaded() {
    //wait until the texture handles are all loaded
    AssetManager::waitForLoad(texture_Cube);
    AssetManager::waitForLoad(texture_Grass);

    //wait until the meshes are all loaded
    AssetManager::waitForLoad(mesh_Cube);
    AssetManager::waitForLoad(mesh_Terrain);
    AssetManager::waitForLoad(mesh_Suzan);
}

void FirstPersonExample::HandleStorage::initializeForRendering() {
    //wait for the loading to finish
    waitTillLoaded();

    //create the cube render mesh
    renderMesh_Cube = RenderMeshRegistry::create(&AssetManager::getAsset<MeshAsset>(mesh_Cube)->mesh());
    //create the terrain render mesh
    renderMesh_Terrain = RenderMeshRegistry::create(&AssetManager::getAsset<MeshAsset>(mesh_Terrain)->mesh());
    //create the suzan render mesh
    renderMesh_Suzan = RenderMeshRegistry::create(&AssetManager::getAsset<MeshAsset>(mesh_Suzan)->mesh());
}

void FirstPersonExample::loadScene() {
    //create the render data
    m_handleStorage.initializeForRendering();

    //create the material for the cube
    Buffer* buffers[] = { GLGE_SKIP_SLOT(GLGE_BUFFER_TYPE_SHADER_STORAGE), glge_Graphic_GetTransformBuffer(), 
                          &m_meshMaterialData, GLGE_SKIP_SLOT(GLGE_BUFFER_TYPE_UNIFORM) };
    Texture* textures[] = { AssetManager::getAsset<TextureAsset>(m_handleStorage.texture_Cube)->getTexture(),
                            AssetManager::getAsset<TextureAsset>(m_handleStorage.texture_Grass)->getTexture() };
    //load the material for the cube
    m_material.emplace(&m_shader, textures, sizeof(textures)/sizeof(*textures), buffers, sizeof(buffers)/sizeof(*buffers), 
                           GLGE_VERTEX_LAYOUT_SIMPLE_VERTEX);

    //create the main camera
    m_camera = m_scene.createObject<CameraController>("Camera");
    m_camera->add<Camera>(1.570796f, 0.1f, 1000.f, RenderTarget(&m_mainTarget.framebuffer, GLGE_FRAMEBUFFER));

    //create the cube
    m_cube = m_scene.createObject("Cube", Transform(vec3(0,0,-3)));
    m_cube->add<Renderer>(m_handleStorage.renderMesh_Cube, &m_material.value());

    //create the terrain
    m_terrain = m_scene.createObject("Terrain", Transform(vec3(0,-1,0)));
    m_terrain->add<Renderer>(m_handleStorage.renderMesh_Terrain, &m_material.value());

    //create suzan
    m_suzan = m_scene.createObject("Suzan", Transform(3));
    m_suzan->add<Renderer>(m_handleStorage.renderMesh_Suzan, &m_material.value());
}

void FirstPersonExample::finalizeRenderPipeline() {
    //work on the main draw stage
    {
        //get the main draw stage
        RenderPipelineStage& stage = m_pipe.getStage("Main Render Pass");

        //set the camera correctly
        stage.data.drawScene.camera = m_camera->get<Camera>();
        m_camera->get<Camera>()->update();
    }
}