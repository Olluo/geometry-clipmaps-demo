#include "Camera.h"
#include <iostream>
#include <ngl/Quaternion.h>
#include <ngl/Util.h>

namespace terraindeformer
{
  Camera::Camera(ngl::Vec3 _eye, ngl::Vec3 _look, ngl::Vec3 _up) noexcept : m_eye{_eye},
                                                                            m_eyeInit{_eye},
                                                                            m_look{_look},
                                                                            m_lookInit{_look},
                                                                            m_up{_up}
  {
    updateViewMatrix();
  }

  void Camera::move(ngl::Real _deltaX, ngl::Real _deltaY) noexcept
  {
    // Middle mouse moves camera and lookat the same amount
    ngl::Quaternion rotate;
    rotate.fromAxisAngle(m_up, m_yaw);
    ngl::Vec3 move = (rotate * ngl::Vec4{_deltaX, _deltaY, 0.0f}).toVec3();

    m_eye += move * m_move_speed;
    m_look += move * m_move_speed;

    updateViewMatrix();
  }

  void Camera::orbit(ngl::Real _deltaX, ngl::Real _deltaY) noexcept
  {
    // Left mouse moves camera around lookat keeping same distance, left and up change
    _deltaX *= m_sensitivity;
    _deltaY *= m_sensitivity;

    if (m_pitch + _deltaY > 89.0f)
    {
      _deltaY = 0;
    }
    if (m_pitch + _deltaY < -89.0f)
    {
      _deltaY = 0;
    }

    ngl::Vec4 position{m_eye};
    ngl::Vec4 pivot{m_look};

    ngl::Quaternion rotateX;
    rotateX.fromAxisAngle(m_up, _deltaX);
    position = (rotateX * (position - pivot)) + pivot;

    ngl::Quaternion rotateY;
    rotateY.fromAxisAngle(m_right, _deltaY);
    position = (rotateY * (position - pivot)) + pivot;

    m_eye = position.toVec3();

    m_yaw += _deltaX;
    m_pitch += _deltaY;

    // std::cout << "Yaw: " << m_yaw << ", pitch: " << m_pitch << std::endl;

    updateViewMatrix();
  }

  void Camera::dolly(ngl::Real _deltaZ) noexcept
  {
    // Right mouse moves camera towards lookat, distance > 0
    ngl::Vec3 dolly = m_look - m_eye;

    // If the dolly length is greater than 1, or if it is less than 1 the deltaZ must be negative and moving the camera out
    if (dolly.lengthSquared() > 1 || _deltaZ < 0)
    {
      m_eye += _deltaZ * m_dolly_speed * dolly;

      updateViewMatrix();
    }
  }

  void Camera::reset() noexcept
  {
    m_eye = m_eyeInit;
    m_look = m_lookInit;
    m_yaw = m_yawInit;
    m_pitch = m_pitchInit;
    m_right = ngl::Vec3{1.0f, 0.0f, 0.0f};
    updateViewMatrix();
  }

  void Camera::updateViewMatrix() noexcept
  {
    m_right = (m_eye - m_look).cross(m_up);
    m_right.normalize();
    m_view = ngl::lookAt(m_eye, m_look, m_up);
  }

  ngl::Mat4 Camera::view() const noexcept
  {
    return m_view;
  }

} // end namespace terraindeformer