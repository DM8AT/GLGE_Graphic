/**
 * @file Instance.cpp
 * @author DM8AT
 * @brief implement the functionality for graphic instances
 * @version 0.1
 * @date 2025-10-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include graphic instances
#include "Instance.h"
//SDL3 is required
#include ""

//locally use the backend namespace
using namespace GLGE::Graphic::Backend;

//implement the instance

Instance::Instance() {
    //
}




//initialize the global graphic instance
Instance INSTANCE = Instance();

/**
 * @brief store a direct mapping from API enum value to 
 */
static bool GLGE_SUPPORTED_API_MAP[] = {
    false,
    false,
    false,
    false
};

bool glge_graphic_IsAPISupported(InstanceAPI api) {
    //simply return the value from the support map (for now)
    return GLGE_SUPPORTED_API_MAP[(int)api];
}