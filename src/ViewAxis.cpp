#include "ViewAxis.h"

#include <ngl/ShaderLib.h>
#include <ngl/Util.h>
#include <ngl/VAOFactory.h>

namespace geoclipmap
{

  ViewAxis::ViewAxis(const ngl::Mat4 &_sceneView) noexcept : m_sceneView{_sceneView},
                                                             m_view{ngl::lookAt(ngl::Vec3::in(), ngl::Vec3::zero(), ngl::Vec3::up())},
                                                             m_vertices{ngl::Vec3::zero(),
                                                                        ngl::Vec3::right(),
                                                                        ngl::Vec3::zero(),
                                                                        ngl::Vec3::up(),
                                                                        ngl::Vec3::zero(),
                                                                        ngl::Vec3::in()},
                                                             m_model(),
                                                             m_projection(),
                                                             m_vao{}
  {
  }

  void ViewAxis::initialise() noexcept
  {
    m_vao = ngl::VAOFactory::createVAO("simpleVAO", GL_LINES);
    m_vao->bind();
    m_vao->setData(ngl::AbstractVAO::VertexData(m_vertices.size() * sizeof(ngl::Vec3), m_vertices[0].m_x));
    m_vao->setNumIndices(m_vertices.size());
    m_vao->setVertexAttributePointer(0, 3, GL_FLOAT, 0, 0);
    m_vao->unbind();

    constexpr auto vertexShader = "AxisVertex";
    constexpr auto fragmentShader = "AxisFragment";
    constexpr auto geometryShader = "AxisGeometry";

    // create the shader program
    ngl::ShaderLib::createShaderProgram(m_shaderProgram, ngl::ErrorExit::OFF);

    // now we are going to create empty shaders for Frag and Vert
    ngl::ShaderLib::attachShader(vertexShader, ngl::ShaderType::VERTEX);
    ngl::ShaderLib::attachShader(fragmentShader, ngl::ShaderType::FRAGMENT);
    ngl::ShaderLib::attachShader(geometryShader, ngl::ShaderType::GEOMETRY);

    // attach the source
    ngl::ShaderLib::loadShaderSource(vertexShader, "shaders/axis.vert.glsl");
    ngl::ShaderLib::loadShaderSource(fragmentShader, "shaders/axis.frag.glsl");
    ngl::ShaderLib::loadShaderSource(geometryShader, "shaders/axis.geometry.glsl");

    // compile the shaders
    ngl::ShaderLib::compileShader(vertexShader);
    ngl::ShaderLib::compileShader(fragmentShader);
    ngl::ShaderLib::compileShader(geometryShader);

    // add them to the program
    ngl::ShaderLib::attachShaderToProgram(m_shaderProgram, vertexShader);
    ngl::ShaderLib::attachShaderToProgram(m_shaderProgram, fragmentShader);
    ngl::ShaderLib::attachShaderToProgram(m_shaderProgram, geometryShader);

    // now we have associated that data we can link the shader
    ngl::ShaderLib::linkProgramObject(m_shaderProgram);
  }

  void ViewAxis::resize(float _aspect) noexcept
  {
    float zoom = 20.0f;
    m_projection = ngl::ortho(-_aspect * zoom, _aspect * zoom, -zoom, zoom, 0.1f, 1000.0f);

    auto pos = ngl::Vec3(1.5f - (_aspect * zoom), 1.5f - zoom, 0.0f);
    m_model.translate(pos.m_x, pos.m_y, pos.m_z);
  }

  void ViewAxis::loadShader() noexcept
  {
    ngl::ShaderLib::use(m_shaderProgram);

    ngl::Mat4 rot = m_sceneView;
    rot.translate(0.0f, 0.0f, 0.0f);
    auto MVP = m_projection * m_view * m_model * rot;

    ngl::ShaderLib::setUniform("MVP", MVP);
  }

  void ViewAxis::draw() noexcept
  {
    loadShader();
    m_vao->bind();
    m_vao->draw();
    m_vao->unbind();
  }
} // end namespace geoclipmap
