/**
 * @file Camera.cpp
 * @author DM8AT
 * @brief implement the camera class
 * @version 0.1
 * @date 2025-11-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the camera
#include "Camera.h"

Camera::~Camera()
{

}

void Camera::setObject(Object object) noexcept
{
    //if the object is not set, store it
    if (m_obj) {return;}
    m_obj = object;
    //make enough space for a single object
    m_buffer.resize(sizeof(CameraGPUData));
    //update is required
    m_requiresUpdate = true;
}

void Camera::setFOV(float FOV) noexcept
{
    //if the field of view is not the same, store it
    if (FOV == m_data.perspective.fov) {return;}
    m_data.perspective.fov = FOV;
    m_requiresUpdate = true;
}

void Camera::setNear(float near) noexcept
{
    //if the near plane is not the same, store it
    if (near == m_data.perspective.near) {return;}
    m_data.perspective.near = near;
    m_requiresUpdate = true;
}

void Camera::setFar(float far) noexcept
{
    //if the far plane is not the same, store it
    if (far == m_data.perspective.far) {return;}
    m_data.perspective.far = far;
    m_requiresUpdate = true;
}

void Camera::setScale(const vec3& scale) noexcept
{
    //if the scale is not the same, store it
    if (scale.x == m_data.orthographic.scale.x &&
        scale.y == m_data.orthographic.scale.y && 
        scale.z == m_data.orthographic.scale.z)
    {return;}
    m_data.orthographic.scale = scale;
    m_requiresUpdate = true;
}

void Camera::setType(CameraType type) noexcept
{
    //if the types match, do nothing
    if (type == m_type) {return;}

    //if this should be perspective, initialize defaults
    if (type == GLGE_CAMERA_TYPE_PERSPECTIVE) {
        m_data.perspective = CameraData::Perspective{
            .fov = 1.570796f,
            .near = .1f,
            .far = 1000.f
        };
        //and store the type
        m_type = GLGE_CAMERA_TYPE_PERSPECTIVE;
    } else {
        //otherwise, this must be orthographic
        m_data.orthographic.scale = 1;
        //store that this is orthographic
        m_type = GLGE_CAMERA_TYPE_ORTHOGRAPHIC;
    }
    //an update is required
    m_requiresUpdate = true;
}

void Camera::update() noexcept
{
    //get the transform from the object
    Transform transf = *m_obj->get<Transform>();
    transf.pos = -transf.pos;
    //calculate the window aspect ratio
    float aspect = m_window->getSize().y / (float)m_window->getSize().x;

    //if no update is required, check if the transform changed
    if (!m_requiresUpdate) {
        //check if the aspect of the window changed
        if (m_aspect == aspect) {
            //if this is not the same as the current transform, an update is required
            if (
                //position
                transf.pos.x == m_currentTransf.pos.x && 
                transf.pos.y == m_currentTransf.pos.y && 
                transf.pos.z == m_currentTransf.pos.z &&
                //rotation
                transf.rot.w == m_currentTransf.rot.w && 
                transf.rot.i == m_currentTransf.rot.i && 
                transf.rot.j == m_currentTransf.rot.j && 
                transf.rot.k == m_currentTransf.rot.k)
            {return;}
        }
    }

    //an update is required

    //store the new transform
    m_currentTransf = transf;
    //store the new aspect
    m_aspect = aspect;

    //store the new GPU data
    CameraGPUData data;
    //re-compute the transformation matrix
    mat4 posMat = m_currentTransf.getPositionMatrix();
    mat4 rotMat = m_currentTransf.getRotationMatrix();
    data.transf = m_currentTransf.getRotationMatrix() * m_currentTransf.getPositionMatrix();
    //re-compute the inverse of the transformation matrix
    //if no inverse exists, the transformation matrix is just the identity matrix
    data.invTransf = (data.transf.hasInverse()) ? data.transf.inverse() : data.transf;

    //select which projection matrix to compute
    if (m_type == GLGE_CAMERA_TYPE_PERSPECTIVE) {
        //compute a perspective projection matrix

        //pre-compute the tangence of half the field of view
        float tfh = 1. / tan(m_data.perspective.fov / 2.f);
        //pre-compute the difference of near and far plane
        float nf = 1. / (m_data.perspective.far - m_data.perspective.near);
        //compute the full projection matrix
        data.projection = mat4(
            tfh * m_aspect, 0, 0, 0,
            0, tfh, 0, 0, 
            0, 0, -(m_data.perspective.far + m_data.perspective.near) * nf, 
                  -(2.f*m_data.perspective.far*m_data.perspective.near) * nf,
            0, 0, -1, 0);
    } else {
        //compute an orthographic projection matrix
        data.projection = mat4(
            m_data.orthographic.scale.x, 0, 0, 0,
            0, m_data.orthographic.scale.y, 0, 0, 
            0, 0, m_data.orthographic.scale.z, 0,
            0, 0, 0, 1);
    }
    //re-compute the inverse projection matrix
    data.invProjection = data.projection.inverse();

    //push the data to the GPU
    m_buffer.set(0, data);

    //say that the update is done
    m_requiresUpdate = false;
}