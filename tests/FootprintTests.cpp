#ifndef TERRAIN_TESTING
#define TERRAIN_TESTING
#endif

#include <gtest/gtest.h>
#include <ngl/NGLInit.h>

#include "Footprint.h"

namespace geoclipmap
{
  TEST(FootprintTest, ctor_width_depth)
  {
    size_t width = 2;
    size_t depth = 3;
    Footprint f(width, depth);

    EXPECT_EQ(f.m_width, width);
    EXPECT_EQ(f.m_depth, depth);

    std::vector<ngl::Vec2> expectedVertices =
        {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {1, 2}};
    std::vector<ngl::Vec2> actualVertices = f.m_vertices;
    EXPECT_EQ(actualVertices, expectedVertices);
    EXPECT_EQ(expectedVertices.size(), f.m_vertexCount);

    std::vector<GLuint> expectedIndices =
        {0, 2, 1, 3, std::numeric_limits<GLuint>::max(), 2, 4, 3, 5, std::numeric_limits<GLuint>::max()};
    std::vector<GLuint> actualIndices = f.m_indices;
    EXPECT_EQ(actualIndices, expectedIndices);
    EXPECT_EQ(expectedIndices.size(), f.m_indexCount);
  }

  TEST(FootprintTest, ctor_degenerate)
  {
    size_t width = 4;
    Footprint f(width);

    EXPECT_EQ(f.m_width, width);
    EXPECT_EQ(f.m_depth, width);

    std::vector<ngl::Vec2> expectedVertices =
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {2, 3}, {1, 3}, {0, 3}, {0, 2}, {0, 1}};
    std::vector<ngl::Vec2> actualVertices = f.m_vertices;
    EXPECT_EQ(actualVertices, expectedVertices);
    EXPECT_EQ(expectedVertices.size(), f.m_vertexCount);

    std::vector<GLuint> expectedIndices =
        {0, 1, 2, 3, 3, 4, 5, 6, 6, 7, 8, 9, 9, 10, 11, 0};
    std::vector<GLuint> actualIndices = f.m_indices;
    EXPECT_EQ(actualIndices, expectedIndices);
    EXPECT_EQ(expectedIndices.size(), f.m_indexCount);
  }

} // end namespace geoclipmap