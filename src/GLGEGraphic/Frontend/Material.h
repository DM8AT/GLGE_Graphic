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
//add the vertex layout
#include "../../GLGE_Core/Geometry/Surface/VertexLayout.h"

//define how many textures are allowed maximum
#define GLGE_MAX_MATERIAL_TEXTURE_BINDING 48

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
     * @param shader a pointer to the shader for the material
     * @param textures a pointer to an array of texture pointers for the material
     * @param textureCount store the amount of texture pointers in the `textures` array
     * @param layout the expected vertex layout
     */
    Material(Shader* shader, Texture** textures, uint8_t textureCount, const VertexLayout& layout) noexcept;

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
     * @brief Get the Vertex Layout of the material
     * 
     * @return const VertexLayout& a reference to the vertex layout of the material
     */
    inline const VertexLayout& getVertexLayout() const noexcept {return m_layout;}

protected:

    //store the own shader
    Shader* m_shader = nullptr;
    //store all the textures
    Texture* m_textures[GLGE_MAX_MATERIAL_TEXTURE_BINDING] = { nullptr };
    //store the amount of used textures
    uint8_t m_usedTextureCount = 0;
    //store the vertex layout for the material
    VertexLayout m_layout;
};

#endif

#endif