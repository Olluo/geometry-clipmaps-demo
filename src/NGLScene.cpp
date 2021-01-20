/**
 * @file NGLScene.cpp
 * @author Ollie Nicholls - original from Jon Macey
 * @brief A modified version of Jon's NGLScene that inherits from the Qt 
 * OpenGLWindow and allows the use of NGL to draw OpenGL
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <QGuiApplication>
#include <QMouseEvent>

#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>

#include "NGLScene.h"

namespace geoclipmap
{
  NGLScene::NGLScene(std::string _fname)
  {
    std::string title = fmt::format("Geometry Clipmap Demo - {}", _fname);
    setTitle(QString::fromStdString(title));
    m_imageName = _fname;
  }

  NGLScene::~NGLScene()
  {
    std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
  }

  void NGLScene::resizeGL(int _w, int _h)
  {
    m_projection = ngl::perspective(m_win.m_fov, static_cast<float>(_w) / _h, m_win.m_near, m_win.m_far);
    m_win.width = static_cast<int>(_w * devicePixelRatio());
    m_win.height = static_cast<int>(_h * devicePixelRatio());
    m_viewAxis->resize(static_cast<float>(_w) / _h);
    m_text->setScreenSize(_w, _h);
  }

  void NGLScene::initializeGL()
  {
    // we must call that first before any other GL commands to load and link the
    // gl commands from the lib, if that is not done program will crash
    ngl::NGLInit::initialize();

    // uncomment this line to make ngl less noisy with debug info
    // ngl::NGLInit::instance()->setCommunicationMode( ngl::CommunicationMode::NULLCONSUMER);
    glClearColor(135.0f / 256.0f, 206.0f / 256.0f, 235.0f / 256.0f, 1.0f); // Sky blue Background

    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    // enable multisampling for smoother drawing
    glEnable(GL_MULTISAMPLE);
    // Used for drawing triangle strips
    glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);

    // now to load the shader and set the values
    // grab an instance of shader manager
    // in the code create some constexpr
    m_shaderProgram = "Terrain";
    constexpr auto vertexShader = "TerrainVertex";
    constexpr auto fragShader = "TerrainFragment";

    // create the shader program
    ngl::ShaderLib::createShaderProgram(m_shaderProgram, ngl::ErrorExit::OFF);

    // now we are going to create empty shaders for Frag and Vert
    ngl::ShaderLib::attachShader(vertexShader, ngl::ShaderType::VERTEX);
    ngl::ShaderLib::attachShader(fragShader, ngl::ShaderType::FRAGMENT);

    // attach the source
    ngl::ShaderLib::loadShaderSource(vertexShader, "shaders/terrain.vert.glsl");
    ngl::ShaderLib::loadShaderSource(fragShader, "shaders/terrain.frag.glsl");

    // compile the shaders
    ngl::ShaderLib::compileShader(vertexShader);
    ngl::ShaderLib::compileShader(fragShader);

    // add them to the program
    ngl::ShaderLib::attachShaderToProgram(m_shaderProgram, vertexShader);
    ngl::ShaderLib::attachShaderToProgram(m_shaderProgram, fragShader);

    // now we have associated that data we can link the shader
    ngl::ShaderLib::linkProgramObject(m_shaderProgram);

    // Construct an arcball camera object
    m_cam = Camera({0.0f, 100.0f, 50.0f}, {0.0f, 100.0f, 0.0f}, {0.0f, 1.0f, 0.0f});

    // Rotate so looking down on the terrain
    m_cam.orbit(0.0f, -45.0f);

    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_projection = ngl::perspective(m_win.m_fov, m_win.width / m_win.height, m_win.m_near, m_win.m_far);

    // Get an instance of the Geoclipmap-Constant Manager
    m_manager = Manager::getInstance();

    // Initialise the view axis
    m_viewAxis = new ViewAxis(m_cam.view());
    m_viewAxis->initialise();

    // Initialise the text
    // TODO: There is an issue here with text breaking my height texturing
    // m_text = std::make_unique<ngl::Text>("fonts/Arial.ttf", 18);
    // m_text->setScreenSize(1024, 720);

    // Finally generate the terrain
    generateTerrain();
  }

  void NGLScene::paintGL()
  {
    // TODO: Colour terrain based on clipmap level?
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_win.width, m_win.height);

    // Set the correct shader program for terrain drawing
    ngl::ShaderLib::use(m_shaderProgram);
    m_transform.setRotation(90.0f, 0.0f, 0.0f);

    // Calculate the MVP matrix
    ngl::Mat4 MVP;
    MVP = m_projection * m_cam.view() * m_transform.getMatrix();
    ngl::ShaderLib::setUniform("MVP", MVP);

    // Set the active LoD levels based on the camera height
    m_terrain->setActiveLevels(m_cam.height());

    auto clipmaps = m_terrain->clipmaps();

    // Loop through each of the active levels
    for (int l = static_cast<int>(m_terrain->activeFinest()); l >= static_cast<int>(m_terrain->activeCoarsest()); l--)
    {
      auto currentLevel = clipmaps[l];
      currentLevel->bindTextures();

      // Loop through each of the footprint locations of the current clipmap level
      for (auto location : m_terrain->selectLocations(currentLevel->trimLocation()))
      {
        auto footprint = location->footprint;

        ngl::ShaderLib::setUniform("footprintLocalPos", static_cast<ngl::Real>(location->x), static_cast<ngl::Real>(location->y));
        ngl::ShaderLib::setUniform("clipmapOffsetPos", currentLevel->position());
        ngl::ShaderLib::setUniform("clipmapScale", static_cast<ngl::Real>(currentLevel->scale()));
        ngl::ShaderLib::setUniform("clipmapD", static_cast<ngl::Real>(m_manager->D()));

        footprint->draw();
      }

      glBindTexture(GL_TEXTURE_BUFFER, 0);
    }

    // Draw axis
    m_viewAxis->draw();

    // Draw text
    // drawText();
  }

  void NGLScene::generateTerrain()
  {
    QImage image(m_imageName.c_str());

    // Convert image to 16-bit colour depth
    image = image.convertToFormat(QImage::Format_RGB16);

    // Get the image sizes and output them to console
    int imageWidth = image.size().width();
    int imageHeight = image.size().height();
    std::cout << "Loading height map " << m_imageName << ", size " << imageWidth << "x" << imageHeight << "\n";

    std::vector<ngl::Vec3> gridPoints;

    // Loop through all pixels of image and add them to the list of grid points
    for (int y = 0; y < imageHeight; y++)
    {
      for (int x = 0; x < imageWidth; x++)
      {
        QColor c(image.pixel(x, y));
        gridPoints.push_back(ngl::Vec3(c.redF(), c.greenF(), c.blueF()));
      }
    }

    // Create a heightmap from the image data
    m_heightmap = new Heightmap(imageWidth, imageHeight, gridPoints);

    // Then generate a terrain from that heightmap
    m_terrain = new Terrain(m_heightmap);

    // Now move the terrain so it is centred on the camera
    m_terrainX = imageWidth / 2;
    m_terrainY = imageHeight / 2;
    m_terrain->move(m_terrainX, m_terrainY);
  }

  void NGLScene::regenerateTerrain()
  {
    m_terrain = new Terrain(m_heightmap);
  }

  void NGLScene::drawText()
  {
    int textPos = 700;

    m_text->setColour(1.0f, 1.0f, 0.0f);

    if (!m_win.showHelp)
    {
      m_text->renderText(10, 700, "Press 'h' for controls...");
    }

    if (m_win.showHelp)
    {
      m_text->renderText(10, 700, "===== CONTROLS =====");
      m_text->renderText(10, (textPos-=19), "= 'arrow keys' - move terrain (always follows world axes)");
      m_text->renderText(10, (textPos-=19), "= '[' - reduce LoD, ']' - increase LoD (K)");
      m_text->renderText(10, (textPos-=19), "= '-' - reduce clipmap count, '=' - increase clipmap count (L)");
      m_text->renderText(10, (textPos-=19), "= '9' - reduce clipmap range, '0' - increase clipmap range (R)");
      m_text->renderText(10, (textPos-=19), "= 'LMB' - orbit camera, 'MMB' - pedestal camera (up/down), 'RMB' - dolly camera (in/out)");
      m_text->renderText(10, (textPos-=19), "= 'spacebar' - reset camera");
      m_text->renderText(10, (textPos-=19), "= 'F11' - toggle fullscreen");
      m_text->renderText(10, (textPos-=19), "= 'Esc' - quit");
      m_text->renderText(10, (textPos-=19), "= 'w' - toggle wireframe");
      m_text->renderText(10, (textPos-=19), "= 'h' - to hide these controls");
      m_text->renderText(10, (textPos-=19), "====================");
    }
    textPos -= 19;

    std::string text = fmt::format("Current values: K={}, L={}, R={}", m_manager->K(), m_manager->L(), m_manager->R());
    m_text->renderText(10, (textPos-=19), text);
  }

  void NGLScene::keyPressEvent(QKeyEvent *_event)
  {
    switch (_event->key())
    {
    // escape key to quit
    case Qt::Key_Escape:
      QGuiApplication::exit(EXIT_SUCCESS);
      break;
    // Toggle wireframe
    case Qt::Key_W:
      if (m_win.wireframe)
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
      else
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      m_win.wireframe = !m_win.wireframe;
      break;
    // Toggle fullscreen
    case Qt::Key_F11:
      if (m_win.fullscreen)
      {
        showNormal();
      }
      else
      {
        showFullScreen();
      }
      m_win.fullscreen = !m_win.fullscreen;
      break;
    // Toggle help
    case Qt::Key_H:
      m_win.showHelp = !m_win.showHelp;
      break;
    // Reset camera position
    case Qt::Key_Space:
      m_cam.reset();
      break;
    // Terrain movement
    case Qt::Key_Left:
      m_terrain->move(m_win.m_moveSpeed, 0);
      m_terrainX += m_win.m_moveSpeed;
      break;
    case Qt::Key_Up:
      m_terrain->move(0, m_win.m_moveSpeed);
      m_terrainY += m_win.m_moveSpeed;
      break;
    case Qt::Key_Right:
      if (m_terrainX > 0)
      {
        m_terrain->move(-m_win.m_moveSpeed, 0);
        m_terrainX -= m_win.m_moveSpeed;
      }
      break;
    case Qt::Key_Down:
      if (m_terrainY > 0)
      {
        m_terrain->move(0, -m_win.m_moveSpeed);
        m_terrainY -= m_win.m_moveSpeed;
      }
      break;
    // K adjustment
    case Qt::Key_BracketLeft:
      m_manager->setK(m_manager->K() - 1);
      regenerateTerrain();
      m_terrain->move(m_terrainX, m_terrainY);
      break;
    case Qt::Key_BracketRight:
      m_manager->setK(m_manager->K() + 1);
      regenerateTerrain();
      m_terrain->move(m_terrainX, m_terrainY);
      break;
    // L adjustment
    case Qt::Key_Minus:
      m_manager->setL(m_manager->L() - 1);
      regenerateTerrain();
      m_terrain->move(m_terrainX, m_terrainY);
      break;
    case Qt::Key_Equal:
      m_manager->setL(m_manager->L() + 1);
      regenerateTerrain();
      m_terrain->move(m_terrainX, m_terrainY);
      break;
    // R adjustment
    case Qt::Key_9:
      m_manager->setR(m_manager->R() - 1);
      regenerateTerrain();
      m_terrain->move(m_terrainX, m_terrainY);
      break;
    case Qt::Key_0:
      m_manager->setR(m_manager->R() + 1);
      regenerateTerrain();
      m_terrain->move(m_terrainX, m_terrainY);
      break;
    default:
      break;
    }
    update();
  }
} // end namespace geoclipmap