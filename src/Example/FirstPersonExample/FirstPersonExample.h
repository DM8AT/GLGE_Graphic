/**
 * @file FirstPersonExample.h
 * @author DM8AT
 * @brief define a function to call to run the first person example
 * @version 0.1
 * @date 2025-11-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_EXAMPLE_FIRST_PERSON_EXAMPLE_
#define _GLGE_EXAMPLE_FIRST_PERSON_EXAMPLE_

//add GLGE
#include "../../GLGEGraphic/GLGEGraphic.h"
#include "../../GLGE_Core/GLGECore.h"
//optionals are required because almost nothing is initialized by default

/**
 * @brief a class that contains the data used to run a first person example
 */
class FirstPersonExample {
public:

    /**
     * @brief Construct a new First Person Example
     * 
     * this automatically creates the window for the example - it is needed for setup as it creates the render context
     * This also starts loading for most of the resources
     */
    FirstPersonExample();

    /**
     * @brief the main function for the first person example
     */
    void run();

protected:

    //store some constant settings
    
    //store the multi-sampling setting
    inline static constexpr const TextureMultiSample SAMPLES = GLGE_TEXTURE_SAMPLE_X16;
    //store the super-sampling setting
    inline static constexpr const uint8_t SUPER_SAMPLE = 1;

    //super and multi sampling can't be used at the same time
    static_assert(!((SAMPLES != GLGE_TEXTURE_SAMPLE_X1) && (SUPER_SAMPLE != 1)), "Multi sampling and super sampling can't be active at the same time");
    
    /**
     * @brief load the scene's contents
     */
    void loadScene();

    /**
     * @brief finalize the render pipeline
     */
    void finalizeRenderPipeline();

    //store the window used by the first person example
    Window m_win;

    //store data for the main render target
    struct MainTarget {
        //store the main color texture
        Texture color;
        //store the main depth texture
        Texture depth;
        //store a framebuffer that combines both textures
        Framebuffer framebuffer;
        //store a constant reference to the first person example this target belongs to
        FirstPersonExample& self;

        /**
         * @brief Construct a new Main Target
         * 
         * @param from the first person example to create the target from
         */
        MainTarget(FirstPersonExample& from) noexcept;

        /**
         * @brief handle the resizing of the window
         */
        void resize();
    } m_mainTarget;
    //add the main target as a friend structure
    friend struct MainTarget;

    //store the actual batch shaders
    Shader m_batchShader;
    //store the compute element for batching
    Compute m_batchCompute;
    //store a list of all batch computes
    Compute* m_batchList[1] = { &m_batchCompute };

    //store the main render pipeline
    RenderPipeline m_pipe;

    //store data for assets used by the scene
    struct HandleStorage {
        //store the first person example the handle storage belongs to
        FirstPersonExample& self;

        //store the handle for the cube texture
        AssetHandle texture_Cube;
        //store the handle for the grass texture
        AssetHandle texture_Grass;

        //store the handle for the cube mesh
        AssetHandle mesh_Cube;
        //store the render mesh for the cube mesh
        RenderMeshHandle renderMesh_Cube;

        //store the handle for the terrain mesh
        AssetHandle mesh_Terrain;
        //store the render mesh for the terrain mesh
        RenderMeshHandle renderMesh_Terrain;

        //store the handle for the suzan mesh
        AssetHandle mesh_Suzan;
        //store the render mesh for the suzan mesh
        RenderMeshHandle renderMesh_Suzan;

        /**
         * @brief Construct a new Handle Storage
         * 
         * @param example a reference to the first person example the handle storage belongs to
         */
        HandleStorage(FirstPersonExample& example);

        /**
         * @brief wait until all assets are loaded
         */
        void waitTillLoaded();

        /**
         * @brief create all the render meshes and wait for all assets to load
         */
        void initializeForRendering();

    } m_handleStorage;

    //store the main shader
    Shader m_shader;
    
    /**
     * @brief store per-draw data for the material
     */
    struct MaterialMeshData {
        //store which texture to use
        uint32_t texture;
    }; 
    //store some constant material data
    MaterialMeshData m_meshMaterial[3] = {MaterialMeshData{.texture = 0}, MaterialMeshData{.texture = 0}, MaterialMeshData{.texture = 1}};
    //store the buffer for the material data
    StructuredBuffer<MaterialMeshData> m_meshMaterialData;

    //store the material for everything
    std::optional<Material> m_material;

    //store the scene for the example
    Scene m_scene;
    //store the camera
    Object m_camera;
    //store the cube
    Object m_cube;
    //store the terrain
    Object m_terrain;
    //store suzan
    Object m_suzan;

};

#endif