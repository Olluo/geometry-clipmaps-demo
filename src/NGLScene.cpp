#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/NGLStream.h>
#include <iostream>

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("FirstNGL");
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}



void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}


void NGLScene::initializeGL()
{
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();
  glClearColor(0.8f,0.8f, 0.8f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  ngl::ShaderLib::loadShader("ColourShader","shaders/Vertex.glsl","shaders/Fragment.glsl");
  m_view = ngl::lookAt({2,2,2},{0,0,0},{0,1,0});
  m_project = ngl::perspective(45.0f,static_cast<float>(width())/height(),0.1f,200.0f);
  std::cout<<m_view<<'\n';
  startTimer(10);
}


void NGLScene::loadMatrixToShader(const ngl::Mat4 &_tx)
{
  ngl::ShaderLib::setUniform("MVP", m_project * m_view * _tx);
}



void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  ngl::ShaderLib::use("ColourShader");
  ngl::Mat4 tx;
  ngl::Mat4 scale;
  ngl::Mat4 position;
  ngl::Mat4 yrot;
  static float r=0.0f;
  yrot.rotateY(++r);
  position.translate(1.0f,0.0,0);
  tx = position *yrot;

  loadMatrixToShader(tx);
  ngl::VAOPrimitives::draw("teapot");
  position.translate(1.0f,0.0,0);
  tx = position * scale;
  loadMatrixToShader(tx);

  ngl::VAOPrimitives::draw("troll");
  scale.scale(0.1f,0.1f,0.1f);
  position.translate(-1.0f,0.0,-1.0);
  tx = position * scale;
  loadMatrixToShader(tx);

  ngl::VAOPrimitives::draw("dragon");

}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());

  break;
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}

void NGLScene::timerEvent(QTimerEvent *)
{
  update();
}
