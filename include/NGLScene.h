#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include <ngl/Mat4.h>
#include <ngl/Text.h>
#include <ngl/Transformation.h>
#include <ngl/Vec3.h>

#include "ClipmapLevel.h"
#include "Constants.h"
#include "Camera.h"
#include "Footprint.h"
#include "Heightmap.h"
#include "Terrain.h"
#include "WindowParams.h"
#include <QElapsedTimer>
#include <QOpenGLWindow>
#include <QSet>
#include <memory>

//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

namespace terraindeformer
{
  class NGLScene : public QOpenGLWindow
  {
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene(std::string _fname);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we resize
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h) override;

  private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the windows params such as mouse and rotations etc
    //----------------------------------------------------------------------------------------------------------------------
    WinParams m_win;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the global mouse transforms
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_mouseGlobalTX;
    ngl::Mat4 m_projection;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our Camera
    //----------------------------------------------------------------------------------------------------------------------
    Camera m_cam;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model transformation
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Transformation m_transform;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the keys being pressed
    //----------------------------------------------------------------------------------------------------------------------
    QSet<Qt::Key> m_keysPressed;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief vao created from the image
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_vaoID;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief number of verts to draw
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_vertSize;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief name of the image to load
    //----------------------------------------------------------------------------------------------------------------------
    std::string m_imageName;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event) override;
    void keyReleaseEvent(QKeyEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent(QMouseEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent(QMouseEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent(QMouseEvent *_event) override;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent(QWheelEvent *_event) override;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief build our VAO grid
    //----------------------------------------------------------------------------------------------------------------------
    void buildVAO();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief generate our grid points in x,y (width / depth)
    /// @param [in] _width the physical width size of our grid
    /// @param [in] _depth the physical depth size of our grid
    //----------------------------------------------------------------------------------------------------------------------
    void generateTerrain(ngl::Real _width, ngl::Real _depth);
    Terrain *m_terrain;
    std::string m_shaderProgram;
    ngl::Real m_terrainX = 0;
    ngl::Real m_terrainY = 0;
    float m_fov = 90.0f;
    float m_near = 0.05f;
    float m_far = 1000.0f;
  };

} // end namespace terraindeformer

#endif
