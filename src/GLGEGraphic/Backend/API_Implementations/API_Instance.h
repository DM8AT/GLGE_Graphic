/**
 * @file API_Instance.h
 * @author DM8AT
 * @brief define the base class for all graphic instances
 * @version 0.1
 * @date 2025-10-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_API_INSTANCE_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_API_INSTANCE_

//only available for C++
#if __cplusplus

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief define the base class for all graphic APIs
 */
class Instance
{
public:

    /**
     * @brief Construct a new Instance
     */
    Instance() {}

    /**
     * @brief Destroy the Instance
     */
    virtual ~Instance() {}

};

};

#endif

#endif