#include <QGuiApplication>
#include <QMouseEvent>

#include "NGLScene.h"

#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>

namespace terraindeformer
{
  NGLScene::NGLScene(std::string _fname)
  {
    setTitle("Qt5 Simple NGL Demo");
    m_imageName = _fname;
  }

  NGLScene::~NGLScene()
  {
    std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
  }

  void NGLScene::resizeGL(int _w, int _h)
  {
    m_projection = ngl::perspective(m_fov, static_cast<float>(_w) / _h, m_near, m_far);
    m_win.width = static_cast<int>(_w * devicePixelRatio());
    m_win.height = static_cast<int>(_h * devicePixelRatio());
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

    // and make it active ready to load values
    ngl::ShaderLib::use(m_shaderProgram);

    // Now we will create a basic Camera from the graphics library
    // This is a static camera so it only needs to be set once
    // First create Values for the camera position
    ngl::Vec3 from(0, 100, 50);
    ngl::Vec3 to(0, 100, 0);
    ngl::Vec3 up(0, 1, 0);

    m_cam = Camera(from, to, up);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_projection = ngl::perspective(m_fov, m_win.width / m_win.height, m_near, m_far);

    // ngl::ShaderLib::printRegisteredUniforms(m_shaderProgram);
    // TODO: the 64, 64 here is unused
    generateTerrain(64, 64);
    // m_terrain->move(64, 64);
  }

  void NGLScene::paintGL()
  {
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_win.width, m_win.height);

    ngl::ShaderLib::use(m_shaderProgram);
    m_transform.setRotation(90.0f, 0.0f, 0.0f);

    ngl::Mat4 MVP;
    MVP = m_projection * m_cam.view() * m_transform.getMatrix();
    ngl::ShaderLib::setUniform("MVP", MVP);

    // m_terrain->setActiveMin(static_cast<size_t>(m_cam.height() / 50));
    // m_terrain->setActiveMax(m_terrain->activeMin() + 4);

    auto clipmaps = m_terrain->clipmaps();

    for (int l = static_cast<int>(CLIPMAP_L - 1); l >= 0; l--)
    {
      auto currentLevel = clipmaps[l];
      currentLevel->bindTextures();

      for (auto location : m_terrain->selectLocations(currentLevel->trimLocation()))
      {
        auto footprint = location->footprint;

        ngl::ShaderLib::setUniform("footprintLocalPos", static_cast<ngl::Real>(location->x), static_cast<ngl::Real>(location->y));
        ngl::ShaderLib::setUniform("clipmapOffsetPos", currentLevel->position());
        ngl::ShaderLib::setUniform("clipmapScale", static_cast<ngl::Real>(currentLevel->scale()));
        ngl::ShaderLib::setUniform("clipmapD", static_cast<ngl::Real>(CLIPMAP_D));

        // ngl::ShaderLib::setUniform("colour", static_cast<ngl::Real>(footprint->m_width + footprint->m_depth));

        footprint->draw();
      }

      glBindTexture(GL_TEXTURE_BUFFER, 0);
    }
  }

  void NGLScene::generateTerrain(ngl::Real _width, ngl::Real _depth)
  {
    // load our image and get size
    QImage image(m_imageName.c_str());

    // Convert image to 16-bit colour depth
    image = image.convertToFormat(QImage::Format_RGB16);

    int imageWidth = image.size().width();
    int imageHeight = image.size().height();
    std::cout << "Loading height map " << m_imageName << ", size " << imageWidth << "x" << imageHeight << "\n";

    std::vector<ngl::Vec3> gridPoints;

    for (int y = 0; y < imageHeight; y++)
    {
      for (int x = 0; x < imageWidth; x++)
      {
        QColor c(image.pixel(x, y));
        gridPoints.push_back(ngl::Vec3(c.redF(), c.greenF(), c.blueF()));
      }
    }

    m_terrain = new Terrain(new Heightmap(imageWidth, imageHeight, gridPoints));
  }

  //----------------------------------------------------------------------------------------------------------------------

  void NGLScene::keyPressEvent(QKeyEvent *_event)
  {
    // add to our keypress set the values of any keys pressed
    m_keysPressed += static_cast<Qt::Key>(_event->key());
    // // that method is called every time the main window recives a key event.
    // // we then switch on the key value and set the camera in the GLWindow
    // auto setLight = [](std::string _num, bool _mode) {
    //   ngl::ShaderLib::use("PBR");
    //   if (_mode == true)
    //   {
    //     ngl::Vec3 colour = {255.0f, 255.0f, 255.0f};
    //     ngl::ShaderLib::setUniform(_num, colour);
    //   }
    //   else
    //   {
    //     ngl::Vec3 colour = {0.0f, 0.0f, 0.0f};
    //     ngl::ShaderLib::setUniform(_num, colour);
    //   }
    // };
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
    case Qt::Key_Space:
      m_cam.reset();
      break;
    case Qt::Key_Left:
      m_terrain->move(m_moveSpeed, 0);
      m_terrainX += m_moveSpeed;
      break;
    case Qt::Key_Up:
      m_terrain->move(0, m_moveSpeed);
      m_terrainY += m_moveSpeed;
      break;
    case Qt::Key_Right:
      if (m_terrainX > 0)
      {
        m_terrain->move(-m_moveSpeed, 0);
        m_terrainX -= m_moveSpeed;
      }
      break;
    case Qt::Key_Down:
      if (m_terrainY > 0)
      {
        m_terrain->move(0, -m_moveSpeed);
        m_terrainY -= m_moveSpeed;
      }
      break;
    default:
      break;
    }
    update();
  }

  void NGLScene::keyReleaseEvent(QKeyEvent *_event)
  {
    // remove from our key set any keys that have been released
    m_keysPressed -= static_cast<Qt::Key>(_event->key());
  }
} // end namespace terraindeformer