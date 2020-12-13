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
    m_projection = ngl::perspective(45.0f, static_cast<float>(_w) / _h, 0.05f, 350.0f);
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
    ngl::Vec3 from(0, 0, 40);
    ngl::Vec3 to(0, 0, 0);
    ngl::Vec3 up(0, 1, 0);

    m_cam = Camera(from, to, up);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_projection = ngl::perspective(45.0f, m_win.width / m_win.height, 0.05f, 10000.0f);

    // ngl::ShaderLib::printRegisteredUniforms(m_shaderProgram);
    generateTerrain(64, 64);
    // m_terrain->move(10, 10);
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

    auto clipmaps = m_terrain->clipmaps();
    updateClipmaps(clipmaps);
    for (int l = CLIPMAP_L - 1; l >= 0; l--)
    // for (int l = 0; l >= 0; l--)
    // for (int l = 1; l >= 0; l--)
    {
      auto currentLevel = clipmaps[l];
      glBindTexture(GL_TEXTURE_BUFFER, currentLevel->textureName());
      // TODO: set colour

      FootprintSelection selection;
      if (l == 0)
      {
        selection = FootprintSelection::All;
      }
      else if (currentLevel->bottom())
      {
        if (currentLevel->left())
        {
          selection = FootprintSelection::BottomLeft;
        }
        else
        {
          selection = FootprintSelection::BottomRight;
        }
      }
      else
      {
        if (currentLevel->left())
        {
          selection = FootprintSelection::TopLeft;
        }
        else
        {
          selection = FootprintSelection::TopRight;
        }
      }

      // Get the position in world of the current clipmap level
      ngl::Vec2 levelPosition = currentLevel->position();

      // Get decimal part of the clipmap position, multiply by 2 and subtract 1 to get offset inside parent
      // if the decimal part is less than 0.5 this will result in a negative number, if it is greater, a positive
      // number. As each clipmap level's position is half of the parent position, this results in an alternating
      // placement for each clipmap where in the finest clipmap will be centred around the viewer
      float xPos = (levelPosition.m_x - static_cast<long>(levelPosition.m_x)) * 2.0f - 1.0f;
      float yPos = (levelPosition.m_y - static_cast<long>(levelPosition.m_y)) * 2.0f - 1.0f;

      for (auto location : m_terrain->selectLocations(selection))
      {
        auto footprint = location->footprint;
        ngl::Vec4 scaleFactor{
            static_cast<ngl::Real>(location->x + xPos),                    // location->x is the footprints local coords, xPos is the offset for the clipmap level
            static_cast<ngl::Real>(location->y + yPos),                    // location->y is the footprints local coords, xPos is the offset for the clipmap level
            static_cast<ngl::Real>(currentLevel->scale()),                 // This is the scale of the clipmap level
            static_cast<ngl::Real>(1 / static_cast<ngl::Real>(CLIPMAP_D)), // This is to scale the texture - texture is DxD and is clamped to 0-1 so need to do same with coords somehow
        };
        ngl::ShaderLib::setUniform("scaleFactor", scaleFactor);

        ngl::Vec4 fineBlockOrigin{
            static_cast<ngl::Real>(location->x),
            static_cast<ngl::Real>(location->y),
            static_cast<ngl::Real>(CLIPMAP_D),
            static_cast<ngl::Real>(0)};
        ngl::ShaderLib::setUniform("fineBlockOrigin", fineBlockOrigin);
        drawFootprint(footprint);
      }

      glBindTexture(GL_TEXTURE_BUFFER, 0);
    }
  }

  void NGLScene::generateTerrain(ngl::Real _width, ngl::Real _depth)
  {
    // load our image and get size
    QImage image(m_imageName.c_str());
    int imageWidth = image.size().width();
    int imageHeight = image.size().height();
    std::unique_ptr<GLfloat[]> data = std::make_unique<GLfloat[]>(imageWidth);
    auto i = data.get();
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
    configureFootprints(m_terrain->footprints());
    configureClipmaps(m_terrain->clipmaps());
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

  void NGLScene::configureClipmaps(std::vector<ClipmapLevel *> _clipmaps)
  {
    for (auto clipmap : _clipmaps)
    {
      glActiveTexture(GL_TEXTURE0);
      glGenTextures(1, &clipmap->textureName());
      glBindBuffer(GL_TEXTURE_BUFFER, clipmap->textureName());
      glBindTexture(GL_TEXTURE_BUFFER, clipmap->textureName());

      glBufferData(GL_TEXTURE_BUFFER, clipmap->texture().size() * sizeof(ngl::Real), &clipmap->texture()[0], GL_STATIC_DRAW);
      glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, clipmap->textureName());

      glBindTexture(GL_TEXTURE_BUFFER, 0);
    }
  }

  void NGLScene::updateClipmaps(std::vector<ClipmapLevel *> _clipmaps)
  {
    for (auto clipmap : _clipmaps)
    {
      glActiveTexture(GL_TEXTURE0);
      glBindBuffer(GL_TEXTURE_BUFFER, clipmap->textureName());
      glBindTexture(GL_TEXTURE_BUFFER, clipmap->textureName());

      glBufferData(GL_TEXTURE_BUFFER, clipmap->texture().size() * sizeof(ngl::Real), &clipmap->texture()[0], GL_STATIC_DRAW);
      glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, clipmap->textureName());

      glBindTexture(GL_TEXTURE_BUFFER, 0);
    }
  }

  void NGLScene::drawFootprint(Footprint *_footprint)
  {
    // std::cout << "Drawing footprint (" << _footprint->width() << ',' << _footprint->depth() << ")\n";
    glBindVertexArray(_footprint->vao());
    glBindBuffer(GL_ARRAY_BUFFER, _footprint->vbo());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _footprint->ibo());
    glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(_footprint->indexCount()), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  //----------------------------------------------------------------------------------------------------------------------

  void NGLScene::keyPressEvent(QKeyEvent *_event)
  {
    // add to our keypress set the values of any keys pressed
    m_keysPressed += static_cast<Qt::Key>(_event->key());
    // that method is called every time the main window recives a key event.
    // we then switch on the key value and set the camera in the GLWindow
    auto setLight = [](std::string _num, bool _mode) {
      ngl::ShaderLib::use("PBR");
      if (_mode == true)
      {
        ngl::Vec3 colour = {255.0f, 255.0f, 255.0f};
        ngl::ShaderLib::setUniform(_num, colour);
      }
      else
      {
        ngl::Vec3 colour = {0.0f, 0.0f, 0.0f};
        ngl::ShaderLib::setUniform(_num, colour);
      }
    };
    switch (_event->key())
    {
    // escape key to quit
    case Qt::Key_Escape:
      QGuiApplication::exit(EXIT_SUCCESS);
      break;
    // wireframe
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
    case Qt::Key_Space:
      m_cam.reset();
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