#ifndef WINDOWPARAMS_H_
#define WINDOWPARAMS_H_

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
  bool wireframe = false;
  bool fullscreen = false;
  bool showHelp = false;
};

#endif
