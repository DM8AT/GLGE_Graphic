/**
 * @file API_Shader.cpp
 * @author DM8AT
 * @brief implement the static compile function for the shader API
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add the shader API
#include "API_Shader.h"

void GLGE::Graphic::Backend::API::Shader::compileAll() noexcept
{
    //early return if there are no shader to compile
    if (!isCompileRequired()) {return;}

    //else, lock the mutex so the vector cannot move during compiling
    std::unique_lock lock(m_compileMutex);

    //then, iterate over all queued up shader and compile them
    for (auto shader : m_compileQueue) 
    {shader->compile();}

    //lastly, clean up the queue
    m_compileQueue.clear();
}