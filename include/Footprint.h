/**
 * @file Footprint.h
 * @author Ollie Nicholls
 * @brief A class to represent a footprint as seen in https://developer.nvidia.com/gpugems/gpugems2/part-i-geometric-complexity/chapter-2-terrain-rendering-using-gpu-based-geometry
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef FOOTPRINT_H_
#define FOOTPRINT_H_

#include <ngl/Vec2.h>
#include <ngl/Vec3.h>

#include "FootprintVAO.h"

namespace geoclipmap
{
  enum class FootprintType
  {
    Block,
    FixupHorizontal,
    FixupVertical,
    InteriorTrimHorizontal,
    InteriorTrimVertical,
    OuterDegenerateRing
  };

  class Footprint
  {
  public:
    /**
     * @brief Construct a new Footprint object
     * 
     * @param _width The width of the Footprint
     * @param _depth The depth of the Footprint
     */
    Footprint(size_t _width, size_t _depth) noexcept;
    /**
     * @brief Construct a degenerate triangle ring Footprint
     * 
     * @param _width The width of the ring
     */
    Footprint(size_t _width) noexcept;
    /**
     * @brief Destroy the Footprint object and remove the VAO (if bound)
     * 
     */
    ~Footprint() noexcept;
    /**
     * @brief Draw the footprint (bind to correct buffer and draw)
     * 
     */
    void draw() noexcept;

  private:
    // The width of the Footprint
    size_t m_width;
    // The depth of the Footprint
    size_t m_depth;
    // The vertices representing the Footprint
    std::vector<ngl::Vec2> m_vertices;
    // The indices representing the Footprint
    std::vector<GLuint> m_indices;
    // The number of vertices
    size_t m_vertexCount;
    // The number of indices
    size_t m_indexCount;
    // Whether the VAO has been bound
    bool m_vaoBound = false;
    // Whether the footprint vertex data has been bound
    bool m_dataBound = false;
    // A pointer to this footprints VAO
    std::unique_ptr<FootprintVAO> m_vao;

    /**
     * @brief Calculate the 2D vertices for the Footprint
     */
    void calculate2DVertices() noexcept;
    /**
     * @brief Calculate the 2D vertices for a degenerate triangle ring Footprint
     */
    void calculate2DVerticesDegenerate() noexcept;
    /**
     * @brief Calculate the indices for the Footprint.
     * 
     * See https://www.learnopengles.com/tag/triangle-strips/ which shows Java
     * for how to do this which I adapted to my problem. It also seems their 
     * code is technically wrong as yes it works for square sets but not
     * rectangular as they mixed up which width they used when trying to find 
     * items from the next row, so that is fixed here.
     */
    void calculateIndices() noexcept;
    /**
     * @brief Calculate the indices for a degenerate triangle ring Footprint
     */
    void calculateIndicesDegenerate() noexcept;

#ifdef TERRAIN_TESTING
#include <gtest/gtest.h>
    FRIEND_TEST(FootprintTest, ctor_width_depth);
    FRIEND_TEST(FootprintTest, ctor_degenerate);
#endif
  };

  struct FootprintLocation
  {
    // The position in x
    int x;
    // The position in y
    int y;
    // The footprint at (x, y)
    Footprint *footprint;
    /**
     * @brief Construct a new Footprint Location object that represents where 
     * the footprint could be
     * 
     * @param _x The position in x
     * @param _y The position in y
     * @param _footprint A reference to a footprint that will be at this 
     * location
     */
    FootprintLocation(int _x,
                      int _y,
                      Footprint *_footprint) : x{_x},
                                               y{_y},
                                               footprint{_footprint}
    {
    }
  };
} // end namespace geoclipmap
#endif // !FOOTPRINT_H_