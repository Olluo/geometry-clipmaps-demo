/**
 * @file ViewAxis.h
 * @author Ollie Nicholls - original from Constantinos Glynos
 * @brief Displays the view axis in the bottom left like maya - modified from Cos' demo at https://github.com/NCCA/MayaNGL
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef VIEW_AXIS_H_
#define VIEW_AXIS_H_

#include <ngl/Mat4.h>
#include <ngl/Vec3.h>
#include <ngl/AbstractVAO.h>

namespace geoclipmap
{
  class ViewAxis
  {
  public:
    ViewAxis() = default;
    ViewAxis(const ngl::Mat4 &_sceneView) noexcept;
    void initialise() noexcept;
    void resize(float _aspect) noexcept;
    void loadShader() noexcept;
    void draw() noexcept;
    

  private:
    const ngl::Mat4& m_sceneView;
    ngl::Mat4 m_model;
    ngl::Mat4 m_view;
    ngl::Mat4 m_projection;

    std::vector<ngl::Vec3> m_vertices;

    std::unique_ptr<ngl::AbstractVAO> m_vao;

    std::string m_shaderProgram = "Axis";
  };
} // end namespace geoclipmap

#endif // !VIEW_AXIS_H_