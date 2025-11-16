/**
 * @file OGL_Commands.cpp
 * @author DM8AT
 * @brief implement the execute functions for all commands
 * @version 0.1
 * @date 2025-10-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the commands
#include "OGL_Commands.h"

//add OpenGL
#include "glad/glad.h"

//add SDL3 for window stuff
#include "SDL3/SDL.h"
//include the actual window
#include "../../../Frontend/Window/Window.h"
//add the instance
#include "../../Instance.h"
#include "OGL_Instance.h"
//also OpenGL windows
#include "OGL_Window.h"
//add OpenGL materials
#include "OGL_Material.h"
//add core meshes
#include "../../../../GLGE_Core/Geometry/Surface/Mesh.h"
//add OpenGL textures
#include "OGL_Texture.h"
//add OpenGL shaders
#include "OGL_Shader.h"
//add frontend materials
#include "../../../Frontend/Material.h"
//add frontend render meshes
#include "../../../Frontend/RenderAPI/RenderMesh.h"
//add cycle buffers
#include "OGL_CycleBuffer.h"
//add compute objects
#include "../../../Frontend/Compute.h"

static GLenum getType(VertexElementDataType type) noexcept
{
    //switch over the type and return the correct enum type
    switch (type)
    {
    case VERTEX_ELEMENT_DATA_TYPE_FLOAT:
    case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC4:
        return GL_FLOAT;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_DOUBLE:
    case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC4:
        return GL_DOUBLE;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_HALF:
    case VERTEX_ELEMENT_DATA_TYPE_HALF_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_HALF_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_HALF_VEC4:
        return GL_HALF_FLOAT;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_INT8:
    case VERTEX_ELEMENT_DATA_TYPE_INT8_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_INT8_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_INT8_VEC4:
        return GL_BYTE;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_INT16:
    case VERTEX_ELEMENT_DATA_TYPE_INT16_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_INT16_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_INT16_VEC4:
        return GL_SHORT;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_INT32:
    case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC4:
        return GL_INT;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_INT64:
    case VERTEX_ELEMENT_DATA_TYPE_INT64_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_INT64_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_INT64_VEC4:
        //check if 64 bit integers are supported
        GLGE_ASSERT("64 bit integers not supported, but usage requested", 
                    !((GLGE::Graphic::Backend::OGL::Instance*)GLGE::Graphic::Backend::INSTANCE.getInstance())->getExtensions().int64);
        return GL_INT64_ARB;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_UINT8:
    case VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC4:
        return GL_UNSIGNED_BYTE;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_UINT16:
    case VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC4:
        return GL_UNSIGNED_SHORT;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_UINT32:
    case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC4:
        return GL_UNSIGNED_INT;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_UINT64:
    case VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC4:
        //check if 64 bit integers are supported
        GLGE_ASSERT("64 bit integers not supported, but usage requested", 
                    !((GLGE::Graphic::Backend::OGL::Instance*)GLGE::Graphic::Backend::INSTANCE.getInstance())->getExtensions().int64);
        return GL_UNSIGNED_INT64_ARB;
        break;
    
    default:
        return 0;
        break;
    }
}

/**
 * @brief Get the element count of a type
 * 
 * @param type the type to quarry the element count for
 * @return uint8_t the element count of the type
 */
static uint8_t getCount(VertexElementDataType type) noexcept
{
    //switch over the type to get the element count
    switch (type)
    {
    case VERTEX_ELEMENT_DATA_TYPE_FLOAT:
    case VERTEX_ELEMENT_DATA_TYPE_DOUBLE:
    case VERTEX_ELEMENT_DATA_TYPE_HALF:
    case VERTEX_ELEMENT_DATA_TYPE_INT8:
    case VERTEX_ELEMENT_DATA_TYPE_INT16:
    case VERTEX_ELEMENT_DATA_TYPE_INT32:
    case VERTEX_ELEMENT_DATA_TYPE_INT64:
    case VERTEX_ELEMENT_DATA_TYPE_UINT8:
    case VERTEX_ELEMENT_DATA_TYPE_UINT16:
    case VERTEX_ELEMENT_DATA_TYPE_UINT32:
    case VERTEX_ELEMENT_DATA_TYPE_UINT64:
        return 1;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_HALF_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_INT8_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_INT16_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_INT64_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC2:
    case VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC2:
        return 2;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_HALF_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_INT8_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_INT16_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_INT64_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC3:
    case VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC3:
        return 3;
        break;
    case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC4:
    case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC4:
    case VERTEX_ELEMENT_DATA_TYPE_HALF_VEC4:
    case VERTEX_ELEMENT_DATA_TYPE_INT8_VEC4:
    case VERTEX_ELEMENT_DATA_TYPE_INT16_VEC4:
    case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC4:
    case VERTEX_ELEMENT_DATA_TYPE_INT64_VEC4:
    case VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC4:
    case VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC4:
    case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC4:
    case VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC4:
        return 4;
        break;
    
    default:
        return 0;
        break;
    }
}

/**
 * @brief Get the OpenGL compare operation enum for the specific operation
 * 
 * @param cmp the compare operation to get the enum for
 * @return GLenum the enum corresponding to the depth test
 */
static GLenum __getCompareOp(DepthTestOperator cmp) noexcept {
    //switch over the possible cases and return the correct value
    switch (cmp)
    {
    case MATERIAL_DEPTH_TEST_LESS:
        return GL_LESS;
        break;
    case MATERIAL_DEPTH_TEST_LESS_EQUALS:
        return GL_LEQUAL;
        break;
    case MATERIAL_DEPTH_TEST_GREATER:
        return GL_GREATER;
        break;
    case MATERIAL_DEPTH_TEST_GREATER_EQUALS:
        return GL_GEQUAL;
        break;
    case MATERIAL_DEPTH_TEST_EQUALS:
        return GL_EQUAL;
        break;
    case MATERIAL_DEPTH_TEST_NOT_EQUAL:
        return GL_NOTEQUAL;
        break;
    
    default:
        return 0;
        break;
    }
}

void GLGE::Graphic::Backend::OGL::Command_Custom::execute() noexcept
{
    //just run the function
    (*func)(userData);
}

void GLGE::Graphic::Backend::OGL::Command_Clear::execute() noexcept
{
    //just run the clear command
    glClearNamedFramebufferfv(fbuff, buffType, buffId, &r);
}

static void __bindMaterial(::Material* mat, GLGE::Graphic::Backend::OGL::Material* material) noexcept {
    //check if the material's VAO is valid
    if (material->getVAO() == 0) {
        //if not, create the new VAO
        glCreateVertexArrays(1, &material->getVAO());
        glVertexArrayVertexBuffer(material->getVAO(), 0, 
                                  ((GLGE::Graphic::Backend::OGL::Buffer*)GLGE::Graphic::Backend::INSTANCE.getInstance()->getVertexBuffer()
                                    ->getBuffer())->getBuffer(),
                                  0, material->getMaterial()->getVertexLayout().getVertexSize());
        //bind the index buffer
        glVertexArrayElementBuffer(material->getVAO(), ((GLGE::Graphic::Backend::OGL::Buffer*)GLGE::Graphic::Backend::INSTANCE.getInstance()
            ->getIndexBuffer()->getBuffer())->getBuffer());
        //iterate over all elements of the vertex layout
        for (size_t i = 0; i < VERTEX_ELEMENT_TYPE_COUNT; ++i) {
            //get the informatin about the current element
            VertexElement element = mat->getVertexLayout().m_elements[i];
            //if a undefined element is found, skip it
            if (element.data == VERTEX_ELEMENT_DATA_TYPE_UNDEFINED) {continue;}
            uint64_t offset = mat->getVertexLayout().getOffsetOf(i);
            //convert it to the information format required for OpenGL
            GLenum type = getType(element.data);
            uint8_t elements = getCount(element.data);
            //setup the vertex element attribute format
            glVertexArrayAttribFormat(material->getVAO(), i, elements, type, GL_FALSE, mat->getVertexLayout().getOffsetOf(i));
            //bind to the VBO for the element
            glVertexArrayAttribBinding(material->getVAO(), i, 0);
            //activate the element
            glEnableVertexArrayAttrib(material->getVAO(), i);
        }
    }

    //get the frontend material settings
    MaterialSettings settings = material->getMaterial()->getSettings();
    //set the depth test state correctly
    if (settings & MATERIAL_SETTING_ENABLE_DEPTH_TEST) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    //set the depth write state correctly
    glDepthMask((settings & MATERIAL_SETTING_ENABLE_DEPTH_WRITE) ? GL_TRUE : GL_FALSE);
    //set the depth test function correctly
    glDepthFunc(__getCompareOp(material->getMaterial()->getDepthTestOperator()));

    //bind the VAO
    glBindVertexArray(material->getVAO());
    //bind the shader
    glUseProgram(((GLGE::Graphic::Backend::OGL::Shader*)mat->getShader()->getBackend())->getProgram());
    //iterate over all textures of the material
    for (uint8_t i = 0; i < mat->getUsedTextureCount(); ++i) {
        //bind the texture to the current unit
        glBindTextureUnit(i, ((GLGE::Graphic::Backend::OGL::Texture*)((::Texture*) mat->getUsedTextures()[i])->getBackend())->getTexture());
    }
    //store how many buffers of a specific type are bound
    uint8_t uboCount = 0;
    uint8_t ssboCount = 0;
    //iterate over all buffers of the material
    for (uint8_t i = 0; i < mat->getUsedBufferCount(); ++i) {
        //check if the buffer is a special buffer
        if (mat->getUsedBuffers()[i] == GLGE_SKIP_SLOT_SHADER_STORAGE_BUFFER) 
        {++ssboCount; continue;}
        if (mat->getUsedBuffers()[i] == GLGE_SKIP_SLOT_UNIFORM_BUFFER)
        {++uboCount; continue;}

        //bind the correct buffer type
        if (mat->getUsedBuffers()[i]->getType() == GLGE_BUFFER_TYPE_UNIFORM) {
            //handle the buffer as a uniform buffer
            glBindBufferBase(GL_UNIFORM_BUFFER, uboCount++, 
                ((GLGE::Graphic::Backend::API::CycleBuffer*)mat->getUsedBuffers()[i]->getBackend())
                    ->getCurrentGPUBackend<GLGE::Graphic::Backend::OGL::CycleBufferBackend>()
                    ->getBuffer());
        } else {
            //handle the buffer as a shader storage buffer
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssboCount++, 
                ((GLGE::Graphic::Backend::API::CycleBuffer*)mat->getUsedBuffers()[i]->getBackend())
                    ->getCurrentGPUBackend<GLGE::Graphic::Backend::OGL::CycleBufferBackend>()
                    ->getBuffer());
        }
    }
}

void GLGE::Graphic::Backend::OGL::Command_BindMaterial::execute() noexcept
{
    //get the frontend material
    ::Material* mat = material->getMaterial();

    //bind the material
    __bindMaterial(mat, material);
}

void GLGE::Graphic::Backend::OGL::Command_DrawMesh::execute() noexcept
{
    //run the draw command
    glDrawElementsBaseVertex(GL_TRIANGLES, rMesh->getIndexPointer().size / sizeof(index_t), GL_UNSIGNED_INT, (void*)rMesh->getIndexPointer().startIdx, 
                             rMesh->getVertexPointer().startIdx/rMesh->getRenderMesh()->getMesh()->getVertexLayout().getVertexSize());
}

static void __dispatchCompute(Compute* cmp, uint32_t x, uint32_t y, uint32_t z) noexcept {
    //bind all the textures
    for (uint8_t i = 0; i < cmp->getTextureCount(); ++i) {
        //bind the texture
        glBindTextureUnit(i, ((GLGE::Graphic::Backend::OGL::Texture*)((::Texture*)cmp->getTexture(i))->getBackend())->getTexture());
    }
    //bind all the buffers
    //store how many buffers of a specific type are bound
    uint8_t uboCount = 0;
    uint8_t ssboCount = 0;
    //iterate over all buffers of the material
    for (uint8_t i = 0; i < cmp->getBufferCount(); ++i) {
        //check if the buffer is a special buffer
        if (cmp->getBuffer(i) == GLGE_SKIP_SLOT_SHADER_STORAGE_BUFFER) 
        {++ssboCount; continue;}
        if (cmp->getBuffer(i) == GLGE_SKIP_SLOT_UNIFORM_BUFFER)
        {++uboCount; continue;}
        
        //bind the correct buffer type
        if (cmp->getBuffer(i)->getType() == GLGE_BUFFER_TYPE_UNIFORM) {
            //handle the buffer as a uniform buffer
            glBindBufferBase(GL_UNIFORM_BUFFER, uboCount++, 
                ((GLGE::Graphic::Backend::API::CycleBuffer*)cmp->getBuffer(i)->getBackend())
                    ->getCurrentGPUBackend<GLGE::Graphic::Backend::OGL::CycleBufferBackend>()
                    ->getBuffer());
        } else {
            //handle the buffer as a shader storage buffer
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssboCount++, 
                ((GLGE::Graphic::Backend::API::CycleBuffer*)cmp->getBuffer(i)->getBackend())
                    ->getCurrentGPUBackend<GLGE::Graphic::Backend::OGL::CycleBufferBackend>()
                    ->getBuffer());
        }
    }
    //bind the actual shader
    glUseProgram(((GLGE::Graphic::Backend::OGL::Shader*)cmp->getShader()->getBackend())->getProgram());
    //dispatch the compute shader
    glDispatchCompute(x,y,z);
}

void GLGE::Graphic::Backend::OGL::Command_DispatchCompute::execute() noexcept
{
    //extract the compute object
    Compute* cmp = (Compute*)compute;

    //dispatch the compute shader
    __dispatchCompute(cmp, x, y, z);
}

void GLGE::Graphic::Backend::OGL::Command_MemoryBarrier::execute() noexcept
{
    //just run a memory barrier
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void GLGE::Graphic::Backend::OGL::Command_DrawMeshesIndirect::execute() noexcept
{
    //calculate the amount of compute shaders to dispatch
    //it is assumed that the batch size of the compute shader is 64
    uint64_t invoke = (uint64_t)std::ceil(meshCount / 64.);
    //iterate over all compute shader to run
    for (size_t i = 0; i < shaders.size(); ++i) {
        //bind the buffers at the pre-determined indices
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, batchBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, drawBuffer);
        //run the compute shader
        __dispatchCompute((Compute*)shaders[i], invoke, 1, 1);
    }

    //bind the material
    __bindMaterial(material->getMaterial(), material);

    //bind the indirect buffer
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawBuffer);

    //run the actual draw call
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, meshCount, 0);
}