/**
 * @file ViewAxis.h
 * @author Ollie Nicholls - original from Constantinos Glynos
 * @brief Displays the view axis in the bottom left like maya - modified from 
 * Cos' demo at https://github.com/NCCA/MayaNGL
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef VIEW_AXIS_H_
#define VIEW_AXIS_H_

#include <ngl/AbstractVAO.h>
#include <ngl/Mat4.h>
#include <ngl/Vec3.h>

namespace geoclipmap
{
  class ViewAxis
  {
  public:
    /**
     * @brief The default constructor for ViewAxis
     * 
     */
    ViewAxis() = default;
    /**
     * @brief Construct a new View Axis object
     * 
     * @param _sceneView The view matrix of the scene
     */
    ViewAxis(const ngl::Mat4 &_sceneView) noexcept;
    /**
     * @brief Initialise the view axis by binding data
     * 
     */
    void initialise() noexcept;
    /**
     * @brief Called when the window resizes
     * 
     * @param _aspect The aspect ratio of the window (width/height)
     */
    void resize(float _aspect) noexcept;
    /**
     * @brief Draw the view axis
     * 
     */
    void draw() noexcept;

  private:
    /**
     * @brief Load and set the shader as the current shader
     * 
     */
    void loadShader() noexcept;

    // A reference to the scene view - used for calculating orientation of axis
    const ngl::Mat4 &m_sceneView;
    // The model matrix of the axis
    ngl::Mat4 m_model;
    // The view matrix of the axis
    ngl::Mat4 m_view;
    // The orthographic projection of the axis
    ngl::Mat4 m_projection;
    // The vertex data
    std::vector<ngl::Vec3> m_vertices;
    // The VAO the data is buffered to
    std::unique_ptr<ngl::AbstractVAO> m_vao;
    // The name of the shader program
    std::string m_shaderProgram = "Axis";
  };
} // end namespace geoclipmap
#endif // !VIEW_AXIS_H_