/**
 * @file Texture.h
 * @author DM8AT
 * @brief define textures for GLGE
 * @version 0.1
 * @date 2025-10-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_TEXTURE_
#define _GLGE_GRAPHIC_FRONTEND_TEXTURE_

//add simple types
#include "../../GLGE_Core/Types.h"

//add vector types for color storage
#include "../../GLGE_Math/GLGEMath.h"
//add bools for C
#include <stdbool.h>
//add filtering
#include "FilterMode.h"

/**
 * @brief define the type of texture that is used
 * 
 * Only the GPU side layout, the texture may be stored differently on the CPU
 */
typedef enum e_TextureType {
    //invalid / error value
    GLGE_TEXTURE_UNDEFINED = 0,
    //the texture has 1 channel and uses uint8_t for the channel
    GLGE_TEXTURE_R,
    //the texture has 2 channels and uses uint8_t per channel (or uint16_t per texel)
    GLGE_TEXTURE_RG,
    //the texture has 3 channels and uses uint8_t per channel (or uint24_t per texel)
    GLGE_TEXTURE_RGB,
    //the texture has 4 channels and uses uint8_t per channel (or uint32_t per texel)
    GLGE_TEXTURE_RGBA,

    //the texture has 1 channel and uses a 32-bit float for the channel
    GLGE_TEXTURE_R_F,
    //the texture has 2 channels and uses a 32-bit float per channel
    GLGE_TEXTURE_RG_F,
    //the texture has 3 channels and uses a 32-bit float per channel
    GLGE_TEXTURE_RGB_F,
    //the texture has 4 channels and uses a 32-bit float per channel
    GLGE_TEXTURE_RGBA_F,

    //the texture has 1 channel and uses a 16-bit float (half) for the channel
    GLGE_TEXTURE_R_H,
    //the texture has 2 channels and uses a 16-bit float (half) per channel
    GLGE_TEXTURE_RG_H,
    //the texture has 3 channels and uses a 16-bit float (half) per channel
    GLGE_TEXTURE_RGB_H,
    //the texture has 4 channels and uses a 16-bit float (half) per channel
    GLGE_TEXTURE_RGBA_H,

    //the texture stores high quality depth data
    GLGE_TEXTURE_DEPTH_32,
    //the texture stores medium quality depth data
    GLGE_TEXTURE_DEPTH_24,
    //the texture stores low quality depth data
    GLGE_TEXTURE_DEPTH_16, 

    //the texture stores medium quality depth data and stencil bits
    GLGE_TEXTURE_DEPTH_24_STENCIL_8,
    //the texture stores high quality depth data and stencil bits
    GLGE_TEXTURE_DEPTH_32_STENCIL_8
} TextureType;

//define an enum to define how many samples to use for the texture
typedef enum e_TextureMultiSample {
    //take one sample per pixel (default value)
    GLGE_TEXTURE_SAMPLE_X1 = 1,
    //take on sample per pixel, but force multi-sampling
    GLGE_TEXTURE_SAMPLE_X1_FORCE = 0xFF01,
    //take two samples per pixel
    GLGE_TEXTURE_SAMPLE_X2 = 2,
    //take four samples per pixel
    GLGE_TEXTURE_SAMPLE_X4 = 4,
    //take 8 samples per pixel (may default to 4 samples per pixels if it is not supported)
    GLGE_TEXTURE_SAMPLE_X8 = 8,
    //take 16 samples per pixel (may default to 4 samples per pixel if it is not supported)
    GLGE_TEXTURE_SAMPLE_X16 = 16,
} TextureMultiSample;

//define a holding type for a uint8_t[3] (uint24_t) to use if needed
//thanks to MSVC to make this more complicated than it needs to be
#if defined(_MSC_VER)
    #pragma pack(push, 1)
    typedef struct {
        uint8_t bytes[3];
    } uint24_t;
    #pragma pack(pop)
#else
    typedef struct __attribute__((packed)) {
        uint8_t bytes[3];
    } uint24_t;
#endif

/**
 * @brief define a union that holds the data for a texture
 */
typedef union u_TextureData {
    //storage of a 1 channel not-hdr image
    uint8_t*  n_hdr_1;
    //storage of a 2 channel not-hdr image
    uint16_t* n_hdr_2;
    //storage of a 3 channel not-hdr image
    uint24_t* n_hdr_3;
    //storage of a 4 channel not-hdr image
    uint32_t* n_hdr_4;
    //storage of a 1 channel hdr image
    float*    hdr_1;
    //storage of a 2 channel hdr image
    vec2*     hdr_2;
    //storage of a 3 channel hdr image
    vec3*     hdr_3;
    //storage of a 4 channel hdr image
    vec4*     hdr_4;
} TextureData;

/**
 * @brief define a structure to store texture data on the CPU
 */
typedef struct s_TextureStorage {
    //store the dimensions of the texture
    uivec2 extent;
    //store if the texture is in high dynamic range or not
    bool isHDR;
    //store the amount of used channels (must be in range 1 to 4)
    uint8_t channels;
    //store the actual texture data
    TextureData data;
} TextureStorage;

//define the texture
#if __cplusplus
    class Texture;
#else
    /**
     * @brief a simple opaque structure for a texture
     */
    typedef struct s_Texture { byte opaque; } Texture;
#endif

//add the class only for C++ and use an opaque container for C
#if __cplusplus

/**
 * @brief store the user handle for a texture
 */
class Texture
{
public:

    /**
     * @brief Construct a new Texture
     * 
     * @param storage the storeage for the CPU-Side data for the texture
     * @param type the type for the GPU-Side texture (this may NOT match with the CPU-Side type!)
     * @param filterMode define the filter mode used for the texture (default is linear sampling)
     * @param anisotropy define the level of anisotropic sampling to use, or the closes supported (default is no anisotropy)
     * @param samples the amount of samples to take per pixel (default is 1 sample per pixel)
     */
    Texture(const TextureStorage& storage, TextureType type, FilterMode filterMode = GLGE_FILTER_MODE_LINEAR, float anisotropy = 0.f,
            TextureMultiSample samples = GLGE_TEXTURE_SAMPLE_X1);

    /**
     * @brief Get the texture data storage of the texture
     * 
     * @return const TextureStorage& the storage of the texture object
     */
    inline const TextureStorage& getData() const noexcept {return m_texStorage;}

    /**
     * @brief Get the Type for the GPU data
     * 
     * @return const TextureType& a reference to the GPU data
     */
    inline const TextureType& getType() const noexcept {return m_type;}

    /**
     * @brief change the size of the texture
     * 
     * This is often used for resizing render textures
     * 
     * @warning this is free to re-create the backend texture
     * 
     * @param size the new size of the texture
     */
    void resizeAndClear(const uivec2& size) noexcept;

    /**
     * @brief Set the Filter Mode of the texture
     * 
     * @param mode the new filtering mode for the texture
     */
    void setFilterMode(FilterMode mode) noexcept;

    /**
     * @brief Get the Filter Mode of the texture
     * 
     * @return TextureFilterMode the current filter mode of the texture
     */
    FilterMode getFilterMode() const noexcept;

    /**
     * @brief Set the maximum anisotropy level for the texture
     * 
     * @param anisotropy the new maximum anisotropy level for the texture
     */
    void setAnisotropy(float anisotropy) noexcept;

    /**
     * @brief Get the maximum level of Anisotropy from the texture
     * 
     * @return float the maximum level of anisotropy that may be used to filter the texture
     */
    float getAnisotropy() const noexcept;

    /**
     * @brief Set the amount of samples to take per pixel for the texture
     * 
     * @param samples the amount of samples to take
     */
    void setMultiSample(TextureMultiSample samples);

    /**
     * @brief Get the amount of samples the texture takes per pixel
     * 
     * @return TextureMultiSample the amount of samples taken per pixel
     */
    TextureMultiSample getMultiSample();

    /**
     * @brief check if the texture is of a color type
     * 
     * @return true : the texture is of a color type
     * @return false : the texture is not of a color type
     */
    bool isColor() const noexcept;

    /**
     * @brief check if the texture is a depth stencil texture
     * 
     * @return true : the texture is a depth stencil texture
     * @return false : the texture is not a depth stencil texture
     */
    bool isDepthStencil() const noexcept;

    /**
     * @brief check if the texture is a depth texture
     * 
     * @return true : the texture is a depth texture
     * @return false : the texture is a depth texture
     */
    bool isDepth() const noexcept;

    /**
     * @brief check if the texture is a stencil texture
     * 
     * @return true : the texture is a stencil texture
     * @return false : the texture is not a stencil texture
     */
    bool isStencil() const noexcept;

    /**
     * @brief print the texture into an output stream
     * 
     * @param os the output stream to print to
     * @param tex the texture to print
     * @return std::ostream& the filled output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Texture& tex) noexcept;

    //define SDL / backend stuff
    #ifdef SDL_h_

    /**
     * @brief Get the Backend texture
     * 
     * @return void* a pointer to the backend texture
     */
    inline void* getBackend() noexcept {return m_tex;}

    #endif //end of backend section

protected:

    /**
     * @brief store the storage for the texture
     */
    TextureStorage m_texStorage = {
        .extent = uivec2(0,0),
        .isHDR = false,
        .channels = 0,
        .data{.n_hdr_1{0}}
    };

    /**
     * @brief store the GPU side type of the texture
     * 
     * GLGE_TEXTURE_UNDEFINED means that the texture is CPU-Only
     */
    TextureType m_type = GLGE_TEXTURE_UNDEFINED;
    /**
     * @brief store a pointer to the backend texture
     */
    void* m_tex = nullptr;
};

#endif

#endif