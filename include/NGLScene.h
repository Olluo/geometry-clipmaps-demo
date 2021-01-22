/**
 * @file NGLScene.h
 * @author Ollie Nicholls - original from Jon Macey
 * @brief A modified version of Jon's NGLScene that inherits from the Qt 
 * OpenGLWindow and allows the use of NGL to draw OpenGL
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef NGLSCENE_H_
#define NGLSCENE_H_

#include <memory>

#include <ngl/Mat4.h>
#include <ngl/Text.h>
#include <ngl/Transformation.h>
#include <ngl/Vec3.h>

#include <QElapsedTimer>
#include <QOpenGLWindow>
#include <QSet>

#include "Camera.h"
#include "ClipmapLevel.h"
#include "Footprint.h"
#include "Heightmap.h"
#include "Manager.h"
#include "Terrain.h"
#include "ViewAxis.h"
#include "WindowParams.h"

namespace geoclipmap
{
  class NGLScene : public QOpenGLWindow
  {
  public:
    /**
     * @brief Construct a new NGLScene object
     * 
     * @param _fname The heightmap file to load in and display
     */
    NGLScene(std::string _fname);
    /**
     * @brief Destroy the NGLScene object, close down NGL and releas OpenGL 
     * resources
     * 
     */
    ~NGLScene() override;
    /**
     * @brief Initialise all class data before drawing. Has an OpenGL context so
     *  any OpenGL computation can be done in here.
     * 
     */
    void initializeGL() override;
    /**
     * @brief Contains any computation for drawing the scene
     * 
     */
    void paintGL() override;
    /**
     * @brief Used to resize the OpenGL window
     * 
     * @param _w The new width
     * @param _h The new height
     */
    void resizeGL(int _w, int _h) override;

  private:
    /**
     * @brief A Qt Event that is called whenever a key is pressed
     * 
     * @param _event the QKeyEvent used to query data about the keys pressed
     */
    void keyPressEvent(QKeyEvent *_event) override;
    /**
     * @brief A Qt Event that is called whenever the mouse is moved
     * 
     * @param _event the QMouseEvent used to query data about the mouse movement
     */
    void mouseMoveEvent(QMouseEvent *_event) override;
    /**
     * @brief A Qt Event that is called whenever a mouse button is clicked
     * 
     * @param _event the QMouseEvent used to query data about the mouse buttons
     */
    void mousePressEvent(QMouseEvent *_event) override;
    /**
     * @brief Loads the heightmap file into a terrain and sets up other terrain
     * related computations
     * 
     */
    void generateTerrain();
    /**
     * @brief Regenerates the terrain using any new settings in the Manager
     * 
     */
    void regenerateTerrain();
    /**
     * @brief Draw the help text to the screen
     * 
     */
    void drawText();

    // Contains window constants
    WinParams m_win;
    // A reference to the Clipmap Constant Manager
    Manager *m_manager;
    // The heightmap image file to be loaded in
    std::string m_imageName;
    // The heightmap that the image is loaded into
    Heightmap *m_heightmap;
    // The generated terrain
    Terrain *m_terrain;
    // The location of the terrain in X
    ngl::Real m_terrainX = 0;
    // The location of the terrain in Y
    ngl::Real m_terrainY = 0;
    // The view axis that shows orientation of the world
    ViewAxis *m_viewAxis;
    // Camera object for viewing the scene
    Camera *m_cam;
    // The terrain shader program
    std::string m_shaderProgram;
    // The help text
    std::unique_ptr<ngl::Text> m_text;
    // The projection matrix of the scene
    ngl::Mat4 m_projection;
    // The transformation matrix of the scene
    ngl::Transformation m_transform;
  };

} // end namespace geoclipmap

#endif
