#include "NGLScene.h"
#include <QMouseEvent>

namespace geoclipmap
{
  void NGLScene::mouseMoveEvent(QMouseEvent *_event)
  {
    // note the method buttons() is the button state when event was called
    // that is different from button() which is used to check which button was
    // pressed when the mousePress/Release event is generated
    if (_event->buttons() == Qt::LeftButton)
    {
      int deltaX = _event->x() - m_win.origX;
      int deltaY = _event->y() - m_win.origY;
      m_win.origX = _event->x();
      m_win.origY = _event->y();
      m_cam.orbit(deltaX, -deltaY);
      update();
    }

    if (_event->buttons() == Qt::RightButton)
    {
      int deltaY = _event->y() - m_win.origY;
      m_win.origY = _event->y();
      m_cam.dolly(deltaY);
      update();
    }

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

  void NGLScene::mouseReleaseEvent(QMouseEvent *_event)
  {
  }

  void NGLScene::wheelEvent(QWheelEvent *_event)
  {
  }
} // end namespace geoclipmap