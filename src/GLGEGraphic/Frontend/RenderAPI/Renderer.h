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
#include "RenderMeshRegistry.h"

//define a structure that combins a render mesh handle and a material
typedef struct s_RenderObject {
    //store the handle to the render mesh to draw
    RenderMeshHandle handle;
    //store a pointer to the material to handle
    ::Material* material;
} RenderObject;

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
     * @param objs a initializer list of render objects (Render Mesh - material combinations) to draw at the transform of the object
     */
    Renderer(std::initializer_list<RenderObject> objs) noexcept : Renderer(objs.begin(), objs.size()) {}

    /**
     * @brief Construct a new Renderer
     * 
     * @param objs a pointer to a C-Array of RenderObjects (Render Mesh - material combinations) to draw at the transform of the object
     * @param objCount the amount of render objects in the C array
     */
    Renderer(const RenderObject* objs, size_t objCount) noexcept;

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

    /**
     * @brief Get the Element count
     * 
     * @return size_t the amount of render objects the renderer contains
     */
    inline size_t getElementCount() const noexcept {return m_objs.size();}

    /**
     * @brief Get a specific object from the renderer
     * 
     * @warning this function is not safe and may index out of bounds
     * 
     * @param i the index to quarry the object from
     * @return const RenderObject& a constant reference to the object stored at that index
     */
    inline const RenderObject& getObject(size_t i) const noexcept {return m_objs[i];}

protected:

    //store if the object is shown
    bool m_shown = true;
    //store the actual render objects
    std::vector<RenderObject> m_objs{};

};

#endif

#endif