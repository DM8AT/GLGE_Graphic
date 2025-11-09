/**
 * @file OGL_Instance.cpp
 * @author DM8AT
 * @brief implement the OpenGL instance
 * @version 0.1
 * @date 2025-10-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the OpenGL instance
#include "OGL_Instance.h"

//use the OpenGL implementation namespace
using namespace GLGE::Graphic::Backend::OGL;

//first, include SDL3 to get access to SDL
#include "SDL3/SDL.h"
//include the frontend windows
#include "../../../Frontend/Window/Window.h"
//add glad
#include "glad/glad.h"

//add the frontend material to sanity check the validity
#include "../../../Frontend/Material.h"

// Debug callback function for OpenGL
void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                    GLsizei length, const GLchar* message, const void* userParam) {
    // Only handle the error messages
    if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM || type == GL_DEBUG_TYPE_PERFORMANCE || type == GL_DEBUG_TYPE_ERROR) {
        std::cerr << "OpenGL Debug Message: " << message << "\n";
        std::cerr << "Source: " << source << ", Type: " << type << ", Severity: " << severity << "\n";
    }
}

/**
 * @brief Get the Open GL Constant as a string
 * 
 * @param constantName the symbolic name of the constant
 * @return std::string the string for the constant integer value
 */
static inline std::string __getOpenGLConstantAsString_I(GLenum constantName) noexcept {
    int32_t val = 0;
    glGetIntegerv(constantName, &val);
    return std::to_string(val);
}

Instance::Instance(Window* window)
 : GLGE::Graphic::Backend::API::Instance(&m_vertexBuffer, &m_indexBuffer)
{
    //set some values for the context (4.6 Core)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //create the gl context
    m_glContext = SDL_GL_CreateContext(window->getSDLWindow());
    GLGE_ASSERT((std::string("Failed to create OpenGL context, info: ") + SDL_GetError() + " |"), !m_glContext);
    //no vsync by default
    SDL_GL_SetSwapInterval(0);

    //initialize GLEW
    GLGE_ASSERT("Failed to initialize GLEW", gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0);

    //when debugging is enabled, do some special setup
    #if GLGE_BG_DEBUG
    //print some debug information
    printf("[DEBUG] OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("[DEBUG] Renderer: %s\n", glGetString(GL_RENDERER));
    printf("[DEBUG] UBO Bindings - Max vertex shader: %s\n", __getOpenGLConstantAsString_I(GL_MAX_VERTEX_UNIFORM_BLOCKS).c_str());
    printf("[DEBUG] UBO Bindings - Max fragment shader: %s\n", __getOpenGLConstantAsString_I(GL_MAX_FRAGMENT_UNIFORM_BLOCKS).c_str());
    printf("[DEBUG] SSBO Bindings - Max vertex shader: %s\n", __getOpenGLConstantAsString_I(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS).c_str());
    printf("[DEBUG] SSBO Bindings - Max fragment shader: %s\n", __getOpenGLConstantAsString_I(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS).c_str());
    printf("[DEBUG] Max UBO size: %s\n", __getOpenGLConstantAsString_I(GL_MAX_UNIFORM_BLOCK_SIZE).c_str());
    printf("[DEBUG] Max combined UBO bindings: %s\n", __getOpenGLConstantAsString_I(GL_MAX_UNIFORM_BUFFER_BINDINGS).c_str());
    printf("[DEBUG] Max SSBO size: %s\n", __getOpenGLConstantAsString_I(GL_MAX_SHADER_STORAGE_BLOCK_SIZE).c_str());
    printf("[DEBUG] Max combined SSBO bindings: %s\n", __getOpenGLConstantAsString_I(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS).c_str());

    //enable debugging
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OpenGLDebugCallback, nullptr);
    #endif

    //supported
    m_extensions.int64 = GLAD_GL_ARB_gpu_shader_int64;

    //sanity-check the GPU
    GLint units = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &units);
    if (units < GLGE_MAX_MATERIAL_TEXTURE_BINDING) {
        //print a warning
        std::cerr << "[WARNING] GLGE expexts a GPU to support " << GLGE_MAX_MATERIAL_TEXTURE_BINDING << " texture units. The current GPU supports only " << units << " texture units. Some pipelines may not work correctly.\n"; 
    }

    //force the vertex and index buffer to create itself
    ((OGL::Buffer*)m_vertexBuffer.getBuffer())->forceCreate();
    ((OGL::Buffer*)m_indexBuffer.getBuffer())->forceCreate();
}

Instance::~Instance()
{
    //clean up the OpenGL context
    if (m_glContext) {
        //clean up the OpenGL context
        SDL_GL_DestroyContext((SDL_GLContext)m_glContext);
        m_glContext = nullptr;
    }
}

uint32_t Instance::getWindowFlags() noexcept
{
    //return the OpenGL flag
    return SDL_WINDOW_OPENGL;
}