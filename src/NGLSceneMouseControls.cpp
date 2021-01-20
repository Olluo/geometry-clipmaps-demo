/**
 * @file NGLSceneMouseControls.cpp
 * @author Ollie Nicholls - original from Jon Macey
 * @brief Contains the computation for the mouse controls associated with the
 * main NGLScene
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <QMouseEvent>

#include "NGLScene.h"

namespace geoclipmap
{
  void NGLScene::mouseMoveEvent(QMouseEvent *_event)
  {
    // LMB orbits camera
    if (_event->buttons() == Qt::LeftButton)
    {
      int deltaX = _event->x() - m_win.origX;
      int deltaY = _event->y() - m_win.origY;
      m_win.origX = _event->x();
      m_win.origY = _event->y();
      m_cam.orbit(deltaX, -deltaY);
      update();
    }

    // RMB dollies camera
    if (_event->buttons() == Qt::RightButton)
    {
      int deltaY = _event->y() - m_win.origY;
      m_win.origY = _event->y();
      m_cam.dolly(deltaY);
      update();
    }

    // MMB pedestals camera
    if (_event->buttons() == Qt::MiddleButton)
    {
      int deltaY = _event->y() - m_win.origY;
      m_win.origY = _event->y();
      m_cam.pedestal(deltaY);
      update();
    }
  }

  void NGLScene::mousePressEvent(QMouseEvent *_event)
  {
    if (_event->button() == Qt::LeftButton)
    {
      m_win.origX = _event->x();
      m_win.origY = _event->y();
    }

    if (_event->button() == Qt::RightButton)
    {
      m_win.origY = _event->y();
    }
    
    if (_event->button() == Qt::MiddleButton)
    {
      m_win.origY = _event->y();
    }
  }
} // end namespace geoclipmap