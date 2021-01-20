/**
 * @file Camera.h
 * @author Ollie Nicholls
 * @brief This class implements a basic arcball camera like the one used in Maya
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CAMERA_H_
#define CAMERA_H_

#include <ngl/Mat4.h>
#include <ngl/Vec3.h>

namespace geoclipmap
{
  class Camera
  {
  public:
    /**
     * @brief Construct a default Camera object
     * 
     */
    Camera() = default;
    /**
     * @brief Construct a new Camera object
     * 
     * @param _eye The location of the camera
     * @param _look The location of what the camera is looking at
     * @param _up The up vector of the camera
     */
    Camera(ngl::Vec3 _eye, ngl::Vec3 _look, ngl::Vec3 _up) noexcept;
    /**
     * @brief Move the camera in the Y plane (pedestal) but don't change 
     * rotation
     * 
     * @param _deltaY The change in Y 
     */
    void pedestal(ngl::Real _deltaY) noexcept;
    /**
     * @brief Orbit the camera around the look at position (arcball camera).
     * This maintains the distance to the look at position.
     * 
     * @param _deltaX The change in X
     * @param _deltaY The change in Y
     */
    void orbit(ngl::Real _deltaX, ngl::Real _deltaY) noexcept;
    /**
     * @brief Dolly the camera along the Z axis (not the same as zoom)
     * 
     * @param _deltaZ The change in Z
     */
    void dolly(ngl::Real _deltaZ) noexcept;
    /**
     * @brief Return the view matrix of the camera
     * 
     * @return ngl::Mat4 The view matrix of the camera
     */
    const ngl::Mat4 &view() const noexcept;
    /**
     * @brief Reset the camera to the initial position
     * 
     */
    void reset() noexcept;
    /**
     * @brief Return the height of the camera above sea level
     * 
     * @return ngl::Real The height of the camera
     */
    ngl::Real height() noexcept;

  private:
    /**
     * @brief Recalculate the view matrix of the camera based on eye, look, and 
     * up
     * 
     */
    void updateViewMatrix() noexcept;

    // The view matrix of the camera
    ngl::Mat4 m_view;

    // The location of the camera
    ngl::Vec3 m_eye;
    ngl::Vec3 m_eyeInit;
    // The location of what the camera is looking at
    ngl::Vec3 m_look;
    ngl::Vec3 m_lookInit;
    // The up vector of the camera
    ngl::Vec3 m_up;
    // The right vector of the camera
    ngl::Vec3 m_right{1.0f, 0.0f, 0.0f};

    // The current yaw (X rotation) angle between camera and look at
    ngl::Real m_yaw = 0.0f;
    ngl::Real m_yawInit = 0.0f;
    // The current pitch (Y rotation) angle between camera and look at
    ngl::Real m_pitch = 0.0f;
    ngl::Real m_pitchInit = 0.0f;

    // The speed to move the camera
    ngl::Real m_move_speed = 0.2f;
    // The speed to rotate the camera
    ngl::Real m_sensitivity = 0.5f;
    // The speed to dolly the camera
    ngl::Real m_dolly_speed = 0.005f;

#ifdef TERRAIN_TESTING
#include <gtest/gtest.h>
    FRIEND_TEST(CameraTest, ctor);
    FRIEND_TEST(CameraTest, reset_camera);
    FRIEND_TEST(CameraTest, pedestal_camera);
    FRIEND_TEST(CameraTest, orbit_camera);
    FRIEND_TEST(CameraTest, dolly_camera);
#endif
  };
} // end namespace geoclipmap

#endif // !CAMERA_H_