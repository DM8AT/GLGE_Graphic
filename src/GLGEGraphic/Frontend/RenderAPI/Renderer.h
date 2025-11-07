/**
 * @file Renderer.h
 * @author DM8AT
 * @brief define a structure that holds multiple render mesh and can be attached to an object. 
 * This structure is used to render render meshes at the transformation specified by the object and in context of the scene the object belongs to. 
 * 
 * @version 0.1
 * @date 2025-11-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_RENDER_API_RENDERER_
#define _GLGE_GRAPHIC_FRONTEND_RENDER_API_RENDERER_

//add strings for names
#include "../../../GLGE_BG/CBinding/String.h"

//add render meshes
#include "RenderMesh.h"

//class is only available for C++
#if __cplusplus

//add vectors to store collections of meshes
#include <vector>
//add unordered maps to store a mapping from mesh names to vector indices
#include <unordered_map>
//add sync stuff
#include <mutex>
#include <shared_mutex>
#include <atomic>

/**
 * @brief Stores a collection of render meshes. 
 * 
 * The class itself is only a collection object. If it is attached to an object, it also marks this object as "to render". 
 * All objects that have this component attached will get rendered (if the `shown` bool is true). 
 */
class Renderer
{
public:

    /**
     * @brief Construct a new Renderer
     */
    Renderer() = default;

    /**
     * @brief Construct a new Renderer
     * 
     * @param meshes a list of meshes to create the renderer from
     */
    Renderer(std::initializer_list<RenderMesh> meshes) noexcept;

    /**
     * @brief Construct a new Renderer
     * 
     * @param meshes a pointer to a C array of meshes
     * @param meshCount the amount of meshes in the C array
     */
    Renderer(const RenderMesh* meshes, size_t meshCount) noexcept;

    /**
     * @brief Set if the renderer is shown
     * 
     * @param shown true : the renderer is shown | false : the renderer is not shown
     */
    inline void setShown(bool shown) noexcept {m_shown = shown;}

    /**
     * @brief get if the renderer is shown
     * 
     * @return true : the renderer is shown
     * @return false : the renderer is not shown
     */
    inline bool isShown() const noexcept {return m_shown;}

protected:

    //store if the object is shown
    bool m_shown = true;
    //store the actual render meshes
    std::vector<RenderMesh> m_meshes;
    //store the mapping from names to the indices
    std::unordered_map<String, std::atomic_size_t> m_indexMap;
    //store a mutex to limit map access
    std::shared_mutex m_mutex;

};

#endif

#endif