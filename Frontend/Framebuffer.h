/**
 * @file Framebuffer.h
 * @author DM8AT
 * @brief define an API for framebuffers
 * @version 0.1
 * @date 2025-11-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_FRAMEBUFFER_
#define _GLGE_GRAPHIC_FRONTEND_FRAMEBUFFER_

//define the maximum amount of textures per framebuffer
#define GLGE_FRAMEBUFFER_MAX_TEXTURES 9
//define the maximum amount of color textures for framebuffers
#define GLGE_FRAMEBUFFER_MAX_COLOR_ATTACHMENTS 8
//define the maximum amount of depth textures for framebuffers
#define GLGE_FRAMEBUFFER_MAX_DEPTH_ATTACHMENTS 1
//define the maximum amount of stencil textures for framebuffers
#define GLGE_FRAMEBUFFER_MAX_STENCIL_ATTACHMENTS 1

//add textures
#include "Texture.h"

//class only available for C++
#if __cplusplus

//a vector is required for storing lists
#include <vector>

/**
 * @brief store a framebuffer
 */
class Framebuffer {
public:

    /**
     * @brief Construct a new Framebuffer
     * 
     * @param textures an array of texture pointers to create the framebuffer from
     * @param textureCount the amount of texture pointers in the array
     */
    Framebuffer(Texture* const* textures, uint8_t textureCount) noexcept;

    /**
     * @brief Construct a new Framebuffer
     * 
     * @param textures a vector containing pointers to the textures
     */
    Framebuffer(const std::vector<Texture*>& textures) noexcept
     : Framebuffer(textures.data(), textures.size())
    {}

    /**
     * @brief Destroy the Framebuffer
     */
    ~Framebuffer();

    /**
     * @brief check if the framebuffer has at least one color attachment
     * 
     * @return true : the framebuffer has at least one color attachment
     * @return false : the framebuffer has no color attachments
     */
    inline bool hasColor() const noexcept {return m_hasFlags & HAS_COLOR;}

    /**
     * @brief check if the framebuffer has a depth attachment
     * 
     * @return true : the framebuffer has a depth attachment
     * @return false : the framebuffer has no depth attachment
     */
    inline bool hasDepth() const noexcept {return m_hasFlags & HAS_DEPTH;}

    /**
     * @brief check if the framebuffer has a stencil attachment
     * 
     * @return true : the framebuffer has a stencil attachment
     * @return false : the framebuffer has no stencil attachment
     */
    inline bool hasStencil() const noexcept {return m_hasFlags & HAS_STENCIL;}

    /**
     * @brief Get the amount of textures
     * 
     * @return uint8_t the amount of textures
     */
    inline uint8_t getTextureCount() const noexcept {return m_texCount;}

    /**
     * @brief Get the Texture array of the framebuffer
     * 
     * @return const Texture** the array of textures
     */
    inline Texture* const* getTextures() const noexcept {return m_textures;}

    /**
     * @brief re-create the backend. Do this if the color attachments changed (e.g. in size)
     * 
     * @warning this will invalidate all render pipelines that use this framebuffer
     */
    void recreate() noexcept;

    //SDL / backend stuff
    #ifdef SDL_h_

    /**
     * @brief get the backend implementation of the framebuffer
     * 
     * @return void* a pointer to the backend implementation
     */
    inline void* getAPI() const noexcept {return m_api;}

    #endif

protected:

    //store the textures for the framebuffer
    Texture* m_textures[GLGE_FRAMEBUFFER_MAX_TEXTURES];
    //store the amount of stored textures
    uint8_t m_texCount = 0;

    //store flags if the texture has some specific elements
    uint8_t m_hasFlags = 0;

    //store the backend implementation
    void* m_api = nullptr;

    //store if the texture has a color attachment
    inline static constexpr const uint8_t HAS_COLOR = 0b1;
    //store if the texture has a depth attachment
    inline static constexpr const uint8_t HAS_DEPTH = 0b10;
    //store if the texture has a stencil attachment
    inline static constexpr const uint8_t HAS_STENCIL = 0b100;

};

#endif

#endif