#ifndef TERRAIN_TESTING
#define TERRAIN_TESTING
#endif

#include <gtest/gtest.h>

#include "Camera.h"

#include <ngl/Util.h>

namespace geoclipmap
{
  TEST(CameraTest, ctor)
  {
    // Set values for camera
    ngl::Vec3 eye{0.0f, 1.0f, 5.0f};
    ngl::Vec3 look{0.0f, 1.0f, 0.0f};
    ngl::Vec3 up{0.0f, 1.0f, 0.0f};

    ngl::Vec3 right = (eye - look).cross(up);
    right.normalize();

    ngl::Mat4 view = ngl::lookAt(eye, look, up);

    // Construct camera object
    Camera camera(eye, look, up);

    // Test all values
    EXPECT_EQ(camera.m_eye, eye);
    EXPECT_EQ(camera.m_eyeInit, eye);
    EXPECT_EQ(camera.m_look, look);
    EXPECT_EQ(camera.m_lookInit, look);
    EXPECT_EQ(camera.m_up, up);
    EXPECT_EQ(camera.m_right, right);

    EXPECT_EQ(camera.m_yaw, 0.0f);
    EXPECT_EQ(camera.m_yawInit, 0.0f);
    EXPECT_EQ(camera.m_pitch, 0.0f);
    EXPECT_EQ(camera.m_pitchInit, 0.0f);

    EXPECT_EQ(camera.m_view, view);
    EXPECT_EQ(camera.view(), view);

    EXPECT_EQ(camera.height(), eye.m_y);
    EXPECT_EQ(camera.position(), ngl::Vec2(eye.m_x, eye.m_z));
  }

  TEST(CameraTest, reset_camera)
  {
    // Set values for camera
    ngl::Vec3 eye{0.0f, 1.0f, 5.0f};
    ngl::Vec3 look{0.0f, 1.0f, 0.0f};
    ngl::Vec3 up{0.0f, 1.0f, 0.0f};

    ngl::Vec3 right = (eye - look).cross(up);
    right.normalize();

    ngl::Mat4 view = ngl::lookAt(eye, look, up);

    ngl::Real yaw = 0.0f;
    ngl::Real pitch = 0.0f;

    // Construct camera object
    Camera camera(eye, look, up);

    // Modify camera using each of the modification methods
    camera.pedestal(100);
    camera.orbit(100, 100);
    camera.dolly(100);

    // Check all values have changed
    EXPECT_NE(camera.m_yaw, yaw);
    EXPECT_NE(camera.m_pitch, pitch);
    // ngl::mat4 doesn't implement != so need to do a boolean comp here
    EXPECT_FALSE(camera.view() == view);
    EXPECT_NE(camera.height(), eye.m_y);
    EXPECT_NE(camera.position(), ngl::Vec2(eye.m_x, eye.m_z));

    // Initial values shouldn't have changed
    EXPECT_EQ(camera.m_eyeInit, eye);
    EXPECT_EQ(camera.m_lookInit, look);
    EXPECT_EQ(camera.m_yawInit, yaw);
    EXPECT_EQ(camera.m_pitchInit, pitch);

    // Reset the camera to initial values
    camera.reset();

    // Check all values are back to initial
    EXPECT_EQ(camera.m_eye, eye);
    EXPECT_EQ(camera.m_look, look);
    EXPECT_EQ(camera.m_up, up);

    EXPECT_EQ(camera.m_yaw, yaw);
    EXPECT_EQ(camera.m_pitch, pitch);
    EXPECT_EQ(camera.view(), view);
    EXPECT_EQ(camera.height(), eye.m_y);
    EXPECT_EQ(camera.position(), ngl::Vec2(eye.m_x, eye.m_z));
  }

  TEST(CameraTest, pedestal_camera)
  {
    // Set values for camera
    ngl::Vec3 eye{0.0f, 1.0f, 5.0f};
    ngl::Vec3 look{0.0f, 1.0f, 0.0f};
    ngl::Vec3 up{0.0f, 1.0f, 0.0f};

    ngl::Vec3 right = (eye - look).cross(up);
    right.normalize();

    ngl::Real yaw = 0.0f;
    ngl::Real pitch = 0.0f;

    // Construct camera object
    Camera camera(eye, look, up);

    // Pedestal the camera
    camera.pedestal(10.0f);

    eye.m_y += 2.0f;
    look.m_y += 2.0f;

    // Check values
    EXPECT_EQ(camera.m_eye.m_y, eye.m_y);
    EXPECT_EQ(camera.m_look.m_y, look.m_y);
    // Right shouldn't have changed
    EXPECT_EQ(camera.m_right, right);
    // Neither should yaw or pitch
    EXPECT_EQ(camera.m_yaw, yaw);
    EXPECT_EQ(camera.m_pitch, pitch);

    ngl::Mat4 view = ngl::lookAt(eye, look, up);
    EXPECT_EQ(camera.view(), view);
    EXPECT_EQ(camera.height(), eye.m_y);
    EXPECT_EQ(camera.position(), ngl::Vec2(eye.m_x, eye.m_z));
  }

  TEST(CameraTest, orbit_camera)
  {
    // Set values for camera
    ngl::Vec3 eye{0.0f, 1.0f, 5.0f};
    ngl::Vec3 look{0.0f, 1.0f, 0.0f};
    ngl::Vec3 up{0.0f, 1.0f, 0.0f};

    ngl::Mat4 view = ngl::lookAt(eye, look, up);

    ngl::Real yaw = 0.0f;
    ngl::Real pitch = 0.0f;

    // Construct camera object
    Camera camera(eye, look, up);

    // Orbit the camera
    // First test values that are out of range
    camera.orbit(0.0f, 1000.0f);
    EXPECT_EQ(camera.view(), view);
    camera.orbit(0.0f, -1000.0f);
    EXPECT_EQ(camera.view(), view);

    // Now test in range value
    ngl::Real xOrbit = 5.0f;
    ngl::Real yOrbit = 10.0f;
    camera.orbit(xOrbit, yOrbit);

    // Multiply by sensitivity
    xOrbit *= 0.5f;
    yOrbit *= 0.5f;

    EXPECT_EQ(camera.m_yaw, yaw + xOrbit);
    EXPECT_EQ(camera.m_pitch, pitch + yOrbit);

    // How to check values without just writing out the same code as the method?
  }

  TEST(CameraTest, dolly_camera)
  {
    // Set bad values for camera
    ngl::Vec3 eye{0.0f, 1.0f, 5.0f};
    ngl::Vec3 look{0.0f, 1.0f, 5.01f};
    ngl::Vec3 up{0.0f, 1.0f, 0.0f};

    ngl::Mat4 view = ngl::lookAt(eye, look, up);

    // Construct camera object
    Camera camera(eye, look, up);

    // Dolly the camera using bad values
    camera.dolly(1.0f);
    EXPECT_EQ(camera.view(), view);

    // Now test in range value
    look = ngl::Vec3{0.0f, 1.0f, 0.0f};
    view = ngl::lookAt(eye, look, up);
    camera = Camera(eye, look, up);

    camera.dolly(1.0f);

    EXPECT_EQ(camera.m_eye.m_z, eye.m_z - 0.005f * 5.0f);
  }
} // end namespace geoclipmap