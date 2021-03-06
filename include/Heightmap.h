/**
 * @file Heightmap.h
 * @author Ollie Nicholls
 * @brief This class is used to store a heightmap to be used in Geoclipmapping
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef HEIGHTMAP_H_
#define HEIGHTMAP_H_

#include <ngl/Vec3.h>

namespace geoclipmap
{
  class Heightmap
  {
  public:
    /**
     * @brief Construct a new Heightmap object from data
     * 
     * @param _width The width of the heightmap
     * @param _depth The height of the heightmap
     * @param _data The data of the heightmap
     */
    Heightmap(ngl::Real _width,
              ngl::Real _depth,
              std::vector<ngl::Vec3> _data) noexcept;
    /**
     * @brief Get the width of the heightmap
     * 
     * @return ngl::Real 
     */
    ngl::Real width() noexcept;
    /**
     * @brief Get the depth of the heightmap
     * 
     * @return ngl::Real 
     */
    ngl::Real depth() noexcept;
    /**
     * @brief Get the float value which is the height at _x, _y in the heightmap
     * 
     * @param _x X coord of the heightmap
     * @param _y Y coord of the heightmap
     * @return ngl::Real 
     */
    ngl::Real value(int _x, int _y) noexcept;
    /**
     * @brief Get the colour at _x, _y in the heightmap
     * 
     * @param _x X coord of the heightmap
     * @param _y Y coord of the heightmap
     * @return ngl::Vec3 
     */
    ngl::Vec3 colour(int _x, int _y) noexcept;
    /**
     * @brief Return the highest point in the heightmap
     * 
     * @return ngl::Real 
     */
    ngl::Real highestPoint() noexcept;

  private:
    // The width of the heightmap (x axis)
    ngl::Real m_width;
    // The depth of the heightmap (y axis)
    ngl::Real m_depth;
    // The heightmap data
    std::vector<ngl::Vec3> m_data;
    // The highest point in the clipmap
    ngl::Real m_highestPoint;
  };
} // end namespace geoclipmap
#endif // !HEIGHTMAP_H_