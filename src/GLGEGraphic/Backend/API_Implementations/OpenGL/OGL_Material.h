/**
 * @file OGL_Material.h
 * @author DM8AT
 * @brief define the OpenGL overload for the material API
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_MATERIAL_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_MATERIAL_

//only available for C++
#if __cplusplus

//add the API
#include "../API_Material.h"
//add types
#include <stdint.h>

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

/**
 * @brief define the material implementation for OpenGL
 */
class Material final : public API::Material
{
public:

    /**
     * @brief Construct a new Material
     * 
     * @param material a pointer to the frontend material to create from
     */
    Material(::Material* material) noexcept : API::Material(material) {}

    /**
     * @brief Destroy the Material
     */
    virtual ~Material();

    /**
     * @brief bind the material as the currently active one
     * 
     * @param cmdBuff a pointer to the command buffer to queue the command into
     */
    virtual void bind(API::CommandBuffer* cmdBuff) noexcept override;

    /**
     * @brief get the VAO of the material
     * 
     * @return uint32_t& a reference to the stored VAO
     */
    inline uint32_t& getVAO() noexcept {return m_vao;}

protected:

    /**
     * @brief store the VAO of the material
     */
    uint32_t m_vao = 0;

};

}

#endif

#endif