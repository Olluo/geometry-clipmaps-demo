/**
 * @file WindowParams.h
 * @author Ollie Nicholls
 * @brief Stores the windows parameters
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef WINDOW_PARAMS_H_
#define WINDOW_PARAMS_H_

struct WinParams
{
  // The previous X mouse value
  int origX = 0;
  // The previous Y mouse value
  int origY = 0;
  // Window width
  int width = 1024;
  // Window height
  int height = 720;
  // Whether in wireframe mode
  bool wireframe = false;
  // Whether in fullscreen mode
  bool fullscreen = false;
  // Whether the help text should be displayed
  bool showHelp = false;
  // The fov of the window
  float m_fov = 90.0f;
  // The near plane
  float m_near = 0.05f;
  // The far plane
  float m_far = 5000.0f;
  // The movement speed of the terrain
  float m_moveSpeed = 10.0f;
};
#endif // !WINDOW_PARAMS_H_
