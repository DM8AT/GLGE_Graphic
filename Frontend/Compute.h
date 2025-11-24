/**
 * @file Compute.h
 * @author DM8AT
 * @brief define a simple API that can be used to run compute shaders
 * @version 0.1
 * @date 2025-11-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_COMPUTE_
#define _GLGE_GRAPHIC_FRONTEND_COMPUTE_

//add buffers
#include "Buffer.h"
//add textures
#include "Texture.h"
//add shaders
#include "Shader.h"

//define some limits
//limit the amount of possible buffer bindings
#define GLGE_MAX_COMPUTE_SHADER_COMBINED_BUFFERS 20
//limit the amount of uniform buffer bindings
#define GLGE_MAX_COMPUTE_SHADER_UNIFORM_BUFFERS 12
//limit the amount of shader storage buffer bindings
#define GLGE_MAX_COMPUTE_SHADER_STORAGE_BUFFERS 8
//limit the amount of textures
#define GLGE_MAX_COMPUTE_SHADER_TEXTURES 16

//class is only available for C++
#if __cplusplus

/**
 * @brief a class that combines data with a shader that operates on that data
 */
class Compute {
public:

    /**
     * @brief Construct a new Compute object
     * 
     * @param shader the shader the compute object dispatches
     * @param buffers a list of pointer to buffers the compute object operates on
     * @param textures a list of pointer to textures the compute object can read from
     */
    Compute(Shader& shader, std::initializer_list<Buffer*> buffers, std::initializer_list<Texture*> textures)
     : Compute(shader, buffers.begin(), buffers.size(), textures.begin(), textures.size())
    {}

    /**
     * @brief Construct a new Compute object
     * 
     * @param shader the shader the compute object dispatches
     * @param buffers the start of a C array of buffer pointers
     * @param bufferCount the amount of buffer pointers in the C array
     * @param textures the start of a C array of texture pointers
     * @param textureCount the amount of texture pointers in the C array
     */
    Compute(Shader& shader, Buffer* const* buffers, uint8_t bufferCount, Texture* const* textures, uint8_t textureCount);

    /**
     * @brief Get the Shader of the compute object
     * 
     * @return Shader* a pointer to the used shader
     */
    inline Shader* getShader() const noexcept {return m_shader;}

    /**
     * @brief Get the amount of buffers used by the compute object
     * 
     * @return uint8_t the amount of used buffers
     */
    inline uint8_t getBufferCount() const noexcept {return m_bufferCount;}

    /**
     * @brief Get a specific buffer used by the compute object
     * 
     * @warning the index is not checked before accessing the buffer array
     * 
     * @param idx the index of the buffer to quarry
     * @return Buffer* a pointer to the specific buffer
     */
    inline Buffer* getBuffer(uint8_t idx) const noexcept {return m_buffers[idx];}

    /**
     * @brief Get the amount of textures used by the compute object
     * 
     * @return uint8_t the amount of used textures
     */
    inline uint8_t getTextureCount() const noexcept {return m_textureCount;}

    /**
     * @brief Get a specific texture used by the compute object
     * 
     * @warning the index is not checked before accessing the texture array
     * 
     * @param idx the index of the texture to quarry
     * @return Texture* a pointer to the specific texture
     */
    inline Texture* getTexture(uint8_t idx) const noexcept {return m_textures[idx];}

protected:

    //store the shader
    Shader* m_shader = nullptr;
    //store the buffers
    Buffer* m_buffers[GLGE_MAX_COMPUTE_SHADER_COMBINED_BUFFERS] = { nullptr };
    //store how many buffers are used
    uint8_t m_bufferCount = 0;
    //store the textures
    Texture* m_textures[GLGE_MAX_COMPUTE_SHADER_TEXTURES] = { nullptr };
    //store how many textures are used
    uint8_t m_textureCount = 0;

};

#endif

#endif 