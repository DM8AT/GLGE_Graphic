/**
 * @file Material.h
 * @author DM8AT
 * @brief define what a material is
 * @version 0.1
 * @date 2025-10-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_MATERIAL_
#define _GLGE_GRAPHIC_FRONTEND_MATERIAL_

//add shader
#include "Shader.h"
//also add textures
#include "Texture.h"
//and buffers
#include "Buffer.h"
//add the vertex layout
#include "../../GLGE_Core/Geometry/Surface/VertexLayout.h"

//define how many textures are allowed maximum
#define GLGE_MAX_MATERIAL_TEXTURE_BINDING 48
//define how many buffers are allowed maximum
#define GLGE_MAX_MATERIAL_BUFFER_BINDING 48

//define a simple 64 bit bitmask as the settings for a material
typedef uint64_t MaterialSettings;

/**
 * @brief define an enum to define the settings for materials
 */
typedef enum e_MaterialSetting
#if __cplusplus
 : MaterialSettings
#endif
{
    //a setting used to control if the depth is tested against the current depth buffer value
    MATERIAL_SETTING_ENABLE_DEPTH_TEST = 0b1,
    //a setting used to control if the z component is written to the depth buffer
    MATERIAL_SETTING_ENABLE_DEPTH_WRITE = 0b10,
    //define if the mixing via the alpha channel is enabled
    MATERIAL_SETTING_ENABLE_MIXING = 0b100
} MaterialSetting;

/**
 * @brief define an enum to describe how to test the depth test
 */
typedef enum e_DepthTestOperator {
    //check if the depth value is less than the one stored in the depth buffer
    MATERIAL_DEPTH_TEST_LESS = 0,
    //check if the depth value is less than or equal to the one stored in the depth buffer
    MATERIAL_DEPTH_TEST_LESS_EQUALS,
    //check if the depth value is greater than the one stored in the depth buffer
    MATERIAL_DEPTH_TEST_GREATER,
    //check if the depth value is greater than or equal to the one stored in the depth buffer
    MATERIAL_DEPTH_TEST_GREATER_EQUALS,
    //check if the depth value is equal to the one stored in the depth buffer
    MATERIAL_DEPTH_TEST_EQUALS,
    //check if the depth value is not equal to the one stored in the depth buffer
    MATERIAL_DEPTH_TEST_NOT_EQUAL
} DepthTestOperator;

//define a value for the default setings
#define GLGE_MATERIAL_SETTINGS_DEFAULT ((MaterialSettings)(MATERIAL_SETTING_ENABLE_DEPTH_WRITE | MATERIAL_SETTING_ENABLE_DEPTH_TEST))

//check for C++ to create a class
#if __cplusplus

/**
 * @brief define the material class
 * 
 * A material is just the combination of a shader with the data used in the shader invocation (e.g. buffer and textures)
 */
class Material
{
public:

    /**
     * @brief Construct a new Material
     * 
     * @param shader 
     * @param texture 
     * @param buffer 
     * @param layout 
     * @param settings 
     * @param depthOperator
     */
    inline Material(Shader* shader, Texture* texture, ::Buffer* buffer, const VertexLayout& layout, 
                    MaterialSettings settings = GLGE_MATERIAL_SETTINGS_DEFAULT, DepthTestOperator depthOperator = MATERIAL_DEPTH_TEST_LESS) noexcept
     : Material(shader, &texture, texture ? 1 : 0, &buffer, buffer ? 1 : 0, layout, settings, depthOperator)
    {}

    /**
     * @brief Construct a new Material
     * 
     * @param shader a pointer to the shader for the material
     * @param textures a pointer to an array of texture pointers for the material
     * @param textureCount store the amount of texture pointers in the `textures` array
     * @param buffers 
     * @param bufferCount 
     * @param layout the expected vertex layout
     * @param settings 
     * @param depthOperator 
     */
    Material(Shader* shader, Texture** textures, uint8_t textureCount, ::Buffer** buffers, uint8_t bufferCount, const VertexLayout& layout, 
             MaterialSettings settings = GLGE_MATERIAL_SETTINGS_DEFAULT, DepthTestOperator depthOperator = MATERIAL_DEPTH_TEST_LESS) noexcept;

    /**
     * @brief Destroy the Material
     */
    ~Material() noexcept;

    /**
     * @brief Set the Settings of the material
     * 
     * @param settings the new settings value
     */
    inline void setSettings(MaterialSettings settings) noexcept {m_settings = settings;}

    /**
     * @brief get a reference to the material settings
     * This can be used to enable / disable specific settings
     * 
     * @return MaterialSettings& a reference to the material settings
     */
    inline MaterialSettings& settings() noexcept {return m_settings;}

    /**
     * @brief Set the Depth Test Operator
     * 
     * @param compareOperator the comparison operator to use for the depth testing
     */
    inline void setDepthTestOperator(DepthTestOperator compareOperator) noexcept {m_depthOperator = compareOperator;}

    /**
     * @brief Get the Shader of the material
     * 
     * @return Shader* a pointer to the shader of the material
     */
    inline Shader* getShader() noexcept {return m_shader;}

    /**
     * @brief Get the Used Texture Count
     * 
     * @return uint8_t the amount of used textures
     */
    inline uint8_t getUsedTextureCount() const noexcept {return m_usedTextureCount;}

    /**
     * @brief Get the Used Textures
     * 
     * @return const Texture** a pointer to a C array that contains pointers to the textures
     */
    inline const Texture** getUsedTextures() const noexcept {return (const Texture**)m_textures;}

    /**
     * @brief Get the Used Buffer Count
     * 
     * @return uint8_t the amount of used buffers
     */
    inline uint8_t getUsedBufferCount() const noexcept {return m_usedBuffers;}

    /**
     * @brief Get the Used Buffers
     * 
     * @return const Buffer** a pointer to a C array that contains pointers to the buffers
     */
    inline const ::Buffer** getUsedBuffers() const noexcept {return (const ::Buffer**)m_buffers;}

    /**
     * @brief Get the Vertex Layout of the material
     * 
     * @return const VertexLayout& a reference to the vertex layout of the material
     */
    inline const VertexLayout& getVertexLayout() const noexcept {return m_layout;}

    /**
     * @brief Get the Settings of the material
     * 
     * @return MaterialSettings the setting bitmask of the material
     */
    inline MaterialSettings getSettings() const noexcept {return m_settings;}

    /**
     * @brief Get the Depth Test Operator
     * 
     * @return DepthTestOperator the operator that is used for depth testing
     */
    inline DepthTestOperator getDepthTestOperator() const noexcept {return m_depthOperator;}

    //define SDL / backend stuff
    #ifdef SDL_h_

    /**
     * @brief Get the Backend texture
     * 
     * @return void* a pointer to the backend texture
     */
    inline void* getBackend() noexcept {return m_material;}

    #endif //end of backend section

protected:

    //store the own shader
    Shader* m_shader = nullptr;
    //store all the textures
    Texture* m_textures[GLGE_MAX_MATERIAL_TEXTURE_BINDING] = { nullptr };
    //store the amount of used textures
    uint8_t m_usedTextureCount = 0;
    //store all the buffers
    ::Buffer* m_buffers[GLGE_MAX_MATERIAL_BUFFER_BINDING] = { nullptr };
    //store the amount of bound buffers
    uint8_t m_usedBuffers = 0;
    //store the vertex layout for the material
    VertexLayout m_layout;
    //store the backend material
    void* m_material = nullptr;
    //store the material settings
    MaterialSettings m_settings = GLGE_MATERIAL_SETTINGS_DEFAULT;
    //store the depth testing method
    DepthTestOperator m_depthOperator = MATERIAL_DEPTH_TEST_LESS;
};

#endif

#endif