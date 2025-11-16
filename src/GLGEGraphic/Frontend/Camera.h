/**
 * @file Camera.h
 * @author DM8AT
 * @brief define the API for a simple camera
 * @version 0.1
 * @date 2025-11-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_CAMERA_
#define _GLGE_GRAPHIC_FRONTEND_CAMERA_

//add windows
#include "Window/Window.h"
//add transforms
#include "../../GLGE_Core/Geometry/Structure/Transform.h"
//add the scene system
#include "../../GLGE_Core/Geometry/Structure/ECS/Scene.h"
//add buffers
#include "StructuredBuffer.h"

/**
 * @brief define what type of camera is used
 */
typedef enum e_CameraType {
    //the camera is an orthographic camera
    GLGE_CAMERA_TYPE_ORTHOGRAPHIC = 0,
    //the camera is a perspective camera
    GLGE_CAMERA_TYPE_PERSPECTIVE
} CameraType;

//the class is only available for C++
#if __cplusplus

/**
 * @brief define a simple orthographic or perspective camera
 */
class Camera {
public:

    /**
     * @brief Construct a new Camera
     * 
     * @param fov the field of view for the camera in radians
     * @param near the near clipping plane of the camera in world units
     * @param far the far clipping plane of the camera in world units
     * @param window the window the camera renders to
     */
    inline Camera(float fov, float near, float far, Window* window) noexcept
        : m_type(GLGE_CAMERA_TYPE_PERSPECTIVE), m_data{.perspective{
            .fov = fov,
            .near = near,
            .far = far
        }}, m_window(window), m_buffer(nullptr, 0, GLGE_BUFFER_TYPE_UNIFORM, CAMERA_BUFFER_COUNT)
    {}

    /**
     * @brief Construct a new Camera
     * 
     * @param window a pointer to the window the camera renders to
     */
    inline Camera(Window* window) noexcept
        : m_type(GLGE_CAMERA_TYPE_ORTHOGRAPHIC), m_data{.orthographic{
            .scale = vec3(1)
        }}, m_window(window), m_buffer(nullptr, 0, GLGE_BUFFER_TYPE_UNIFORM, CAMERA_BUFFER_COUNT)
    {}


    /**
     * @brief Destroy the Camera
     */
    ~Camera();

    /**
     * @brief Set the Object the camera belongs to
     * 
     * @warning DO NOT CALL! this is called by the scene creation automatically
     * 
     * @param object the object the camera belongs to
     */
    void setObject(Object object) noexcept;

    /**
     * @brief set the field of view for the camera
     * 
     * @param FOV the field of view to use in radians
     */
    void setFOV(float FOV) noexcept;

    /**
     * @brief get the field of view used by the camera
     * 
     * @return float the field of view of the camera in radians
     */
    inline float getFOV() const noexcept {return m_data.perspective.fov;}

    /**
     * @brief Set the Near clipping plane of the camera
     * 
     * @param near the near clipping plane in world units
     */
    void setNear(float near) noexcept;

    /**
     * @brief Get the Near clipping plane of the camera
     * 
     * @return float the near clipping plane in world units
     */
    inline float getNear() const noexcept {return m_data.perspective.near;}

    /**
     * @brief Set the Far clipping plane of the camera
     * 
     * @param far the far clipping plane in world units
     */
    void setFar(float far) noexcept;

    /**
     * @brief Get the Far clipping plane of the camera
     * 
     * @return float the far clipping plane in world units
     */
    inline float getFar() const noexcept {return m_data.perspective.far;}

    /**
     * @brief Set the Scale of the camera's view box
     * 
     * @param scale the scale for the view box in world units
     */
    void setScale(const vec3& scale) noexcept;

    /**
     * @brief Get the Scale of the camera's view box
     * 
     * @return const vec3& the scale of the view box in world units
     */
    inline const vec3& getScale() const noexcept {return m_data.orthographic.scale;}

    /**
     * @brief update the camera data
     */
    void update() noexcept;

    /**
     * @brief Get the Buffer with the camera data
     * 
     * @return Buffer* a pointer to the camera buffer
     */
    inline Buffer* getBuffer() noexcept {return &m_buffer;}

    /**
     * @brief Get the Type of camera this is
     * 
     * @return CameraType the camera type
     */
    inline CameraType getType() const noexcept {return m_type;}

    /**
     * @brief Set the Type of the camera
     * 
     * @param type the type of camera this should be
     */
    void setType(CameraType type) noexcept;

protected:

    //define how many buffers are used for the camera data
    inline static constexpr uint8_t CAMERA_BUFFER_COUNT = 1;

    //store the data for both types of camera
    union CameraData {
        //store the data for a perspective camera
        struct Perspective {
            //the field of view in radians
            float fov;
            //the near clipping plane in world units
            float near;
            //the far clipping plane in world units
            float far;
        } perspective;
        //store the data for an orthographic camera
        struct Orthographic {
            //store the scaling to apply for all 3 axis
            vec3 scale;
        } orthographic;
    } m_data;

    //store the camera type
    CameraType m_type;
    //store if data was changed
    bool m_requiresUpdate = true;
    //store the window the camera renders to
    Window* m_window = nullptr;
    //store the current window aspect
    float m_aspect = 1.f;

    //store the object the camera belongs to
    Object m_obj = nullptr;
    //store the currently known transform
    Transform m_currentTransf;

    //store the data to push to the GPU
    struct CameraGPUData {
        //store the transformation matrix
        mat4 transf;
        //store the inverse transformation matrix
        mat4 invTransf;
        //store the projection matrix
        mat4 projection;
        //store the inverse projection matrix
        mat4 invProjection;
    };

    //store a buffer to hold the data
    StructuredBuffer<CameraGPUData> m_buffer;

};

#endif

#endif