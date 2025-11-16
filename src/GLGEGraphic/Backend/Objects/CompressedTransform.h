/**
 * @file CompressedTransform.h
 * @author DM8AT
 * @brief define a structure that is a compressed version of a transform
 * @version 0.1
 * @date 2025-11-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_OBJECTS_COMPRESSED_TRANSFORM_
#define _GLGE_GRAPHIC_BACKEND_OBJECTS_COMPRESSED_TRANSFORM_

//add types
#include "../../../../GLGE_Core/Types.h"
//add normal transforms
#include "../../../../GLGE_Core/Geometry/Structure/Transform.h"

//create a simple compressed transform structure
//only available for C++
#if __cplusplus

//add math
#include <cmath>

//use a custom namespace for the backend: GLGE::Graphic::Backend
namespace GLGE::Graphic::Backend {

/**
 * @brief a simple utility function to compress a float (values in range -1 to 1) to an unsigned 16 bit integer
 * 
 * @param value the floating point value to compress
 * @return uint16_t the result of the compression
 */
inline uint16_t compressFloat(float value) noexcept 
{return (uint16_t)(std::fmod(value * 0.5 + 0.5, 1) * UINT16_MAX);}

/**
 * @brief a simple utility function to reverse the compression of a float (range -1 to 1) to an unsigned 16 bit integer
 * 
 * @param value the compressed float value to restore
 * @return float the de-compressed value
 */
inline float decompressFloat(uint16_t value) noexcept 
{return (value / (float)UINT16_MAX) * 2.f - 1.f;}

/**
 * @brief a structure that stores a more compact form of a transform structure
 * 
 * This structure is defined to be 32 bytes. 
 */
struct CompressedTransform {
    //store the position as plain old floats
    vec3 pos;
    //store the version handle in the first 10 bits. The 16th bit is used as the sign of the w component of the quaternion
    uint16_t version_sign_w = 0;
    //store the compressed i component of the quaternion
    uint16_t quat_i = 0;
    //store the compressed j component of the quaternion
    uint16_t quat_j = 0;
    //store the compressed k component of the quaternion
    uint16_t quat_k = 0;
    //store the scale of the object
    vec3 scale;

    /**
     * @brief Construct a new Compressed Transform
     * 
     * @warning everything is just zeroed out
     */
    CompressedTransform() = default;

    /**
     * @brief Construct a new Compressed Transform
     * 
     * @param transform the transform to compress
     * @param version the version of the object handle the transform belongs to
     */
    CompressedTransform(const Transform& transform, uint16_t version = 0) noexcept 
    //just copy over the scale and position
     : pos(transform.pos), 
       version_sign_w(((uint16_t)(transform.rot.w > 0.f) << 15) | (version & 0x3FF)),
       quat_i(compressFloat(transform.rot.i)),
       quat_j(compressFloat(transform.rot.j)),
       quat_k(compressFloat(transform.rot.k)),
       scale(transform.scale)
    {}
};


}

#endif

#endif