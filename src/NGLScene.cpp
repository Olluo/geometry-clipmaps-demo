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

  void NGLScene::genGridPoints(ngl::Real _width, ngl::Real _depth)
  {
    // load our image and get size
    QImage image(m_imageName.c_str());
    int imageWidth = image.size().width();
    int imageHeight = image.size().height();
    std::cout << "image size " << imageWidth << " " << imageHeight << "\n";

    std::vector<ngl::Vec3> gridPoints;

    for (int z = 0; z < imageHeight; z++)
    {
      for (int x = 0; x < imageWidth; x++)
      {
        QColor c(image.pixel(x, z));
        gridPoints.push_back(ngl::Vec3(c.redF(), c.greenF(), c.blueF()));
      }
    }

    m_terrain = new Terrain(imageWidth, imageHeight, gridPoints);
    m_terrain->initialize();
    configureFootprints(m_terrain->footprints());
    configureClipmaps(m_terrain->clipmaps());
  }

  void NGLScene::initializeGL()
  {
    // we must call that first before any other GL commands to load and link the
    // gl commands from the lib, if that is not done program will crash
    ngl::NGLInit::initialize();

    // uncomment this line to make ngl less noisy with debug info
    // ngl::NGLInit::instance()->setCommunicationMode( ngl::CommunicationMode::NULLCONSUMER);
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f); // Grey Background

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
    ngl::ShaderLib::createShaderProgram(m_shaderProgram);

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
    ngl::Vec3 from(10, 10, 40);
    ngl::Vec3 to(0, 0, 0);
    ngl::Vec3 up(0, 1, 0);

    m_view = ngl::lookAt(from, to, up);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_project = ngl::perspective(45, 720.0f / 576.0f, 0.001f, 150);

    // ngl::ShaderLib::printRegisteredUniforms(m_shaderProgram);
    genGridPoints(64, 64);
  }

  void NGLScene::paintGL()
  {
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_win.width, m_win.height);
    // Rotation based on the mouse position for our global transform
    ngl::Mat4 rotX;
    ngl::Mat4 rotY;
    // create the rotation matrices
    rotX.rotateX(m_win.spinXFace);
    rotY.rotateY(m_win.spinYFace);
    // multiply the rotations
    m_mouseGlobalTX = rotY * rotX;
    // add the translations
    m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
    m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
    m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

    ngl::ShaderLib::use(m_shaderProgram);

    ngl::Mat4 MVP;
    MVP = m_project * m_view * m_mouseGlobalTX;
    ngl::ShaderLib::setUniform("MVP", MVP);

    auto clipmaps = m_terrain->clipmaps();
    for (int l = CLIPMAP_L - 1; l >= 0; l--)
    // for (int l = 0; l >= 0; l--)
    {
      auto currentLevel = clipmaps[l];
      // int x = 1;
      // auto currentLevel = clipmaps[x];

      // set colour

      // TODO: choose which footprint locations to use based on whether the current level is oriented in one of these ways
      // left bottom
      // right bottom
      // left top
      // right top
      int selection = 0;

      // set up some variables that i dont quite understand
      // float levelXPos = (CLIPMAP_N * (l + 1)) / 2;
      // float levelZPos = (CLIPMAP_N * (l + 1)) / 2;
      // float levelXPos = (CLIPMAP_SIZE * (x + 1)) / 2;
      // float levelZPos = (CLIPMAP_SIZE * (x + 1)) / 2;
      ngl::Vec2 levelPosition = currentLevel->position();

      for (auto location : m_terrain->selectLocations(selection))
      {
        auto footprint = location->footprint;
        ngl::Vec4 scaleFactor{static_cast<ngl::Real>(currentLevel->scale()),
                              static_cast<ngl::Real>(currentLevel->scale()),
                              static_cast<ngl::Real>(location->x - levelPosition.m_x),
                              static_cast<ngl::Real>(location->z - levelPosition.m_y)};
        ngl::ShaderLib::setUniform("scaleFactor", scaleFactor);

        ngl::Vec4 fineBlockOrigin{static_cast<ngl::Real>(footprint->widthX()),
                                  static_cast<ngl::Real>(footprint->widthZ()),
                                  static_cast<ngl::Real>(0),
                                  static_cast<ngl::Real>(0)};
        ngl::ShaderLib::setUniform("fineBlockOrigin", fineBlockOrigin);
        drawFootprint(footprint);
      }
    }
  }

  void NGLScene::configureFootprints(std::vector<Footprint *> _footprints)
  {
    for (auto footprint : _footprints)
    {
      glGenVertexArrays(1, &footprint->vao());
      glBindVertexArray(footprint->vao());

      glGenBuffers(1, &footprint->vbo());
      glBindBuffer(GL_ARRAY_BUFFER, footprint->vbo());
      glBufferData(GL_ARRAY_BUFFER, footprint->vertexCount() * sizeof(ngl::Vec2), &footprint->vertices()[0].m_x, GL_STATIC_DRAW);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ngl::Vec2), 0);
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glGenBuffers(1, &footprint->ibo());
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, footprint->ibo());
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, footprint->indexCount() * sizeof(GLuint), &footprint->indices()[0], GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
  }

  void NGLScene::configureClipmaps(std::vector<Clipmap *> _clipmaps)
  {
    for (auto clipmap : _clipmaps)
    {
      glGenTextures(1, &clipmap->textureName());
      glBindTexture(GL_TEXTURE_2D, clipmap->textureName());
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(CLIPMAP_N), static_cast<GLsizei>(CLIPMAP_N), 0, GL_RGB, GL_UNSIGNED_BYTE, &clipmap->texture());

      glGenerateMipmap(GL_TEXTURE_2D); //  Allocate the mipmaps
      glBindTexture(GL_TEXTURE_2D, 0);
    }
  }

  void NGLScene::drawFootprint(Footprint *_footprint)
  {
    // std::cout << "Drawing footprint (" << _footprint->widthX() << ',' << _footprint->widthZ() << ")\n";
    glBindVertexArray(_footprint->vao());
    glBindBuffer(GL_ARRAY_BUFFER, _footprint->vbo());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _footprint->ibo());
    glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(_footprint->indexCount()), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void NGLScene::resizeGL(int _w, int _h)
  {
    m_project = ngl::perspective(45.0f, static_cast<float>(_w) / _h, 0.05f, 350.0f);
    m_win.width = static_cast<int>(_w * devicePixelRatio());
    m_win.height = static_cast<int>(_h * devicePixelRatio());
  }
  //----------------------------------------------------------------------------------------------------------------------

  void NGLScene::keyPressEvent(QKeyEvent *_event)
  {
    // this method is called every time the main window recives a key event.
    // we then switch on the key value and set the camera in the GLWindow
    switch (_event->key())
    {
    // escape key to quite
    case Qt::Key_Escape:
      QGuiApplication::exit(EXIT_SUCCESS);
      break;
    // turn on wireframe rendering
    case Qt::Key_W:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    // turn off wire frame
    case Qt::Key_S:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    // show full screen
    case Qt::Key_F:
      showFullScreen();
      break;
    // show windowed
    case Qt::Key_N:
      showNormal();
      break;
    default:
      break;
    }
    // finally update the GLWindow and re-draw
    //if (isExposed())
    update();
  }
} // end namespace terraindeformer