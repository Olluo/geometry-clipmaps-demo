#include <gtest/gtest.h>
#include <ngl/NGLInit.h>

#include "Constants.h"
#include "Footprint.h"

namespace terraindeformer
{
  TEST(FootprintTest, ctor_width_depth)
  {
    size_t width = 2;
    size_t depth = 3;
    Footprint f(width, depth);

    EXPECT_EQ(f.width(), width);
    EXPECT_EQ(f.depth(), depth);

    std::vector<ngl::Vec2> expectedVertices =
        {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {1, 2}};
    std::vector<ngl::Vec2> actualVertices = f.vertices();
    EXPECT_EQ(actualVertices, expectedVertices);
    EXPECT_EQ(expectedVertices.size(), f.vertexCount());

    std::vector<GLuint> expectedIndices =
        {0, 2, 1, 3, 3, 2, 2, 4, 3, 5};
    std::vector<GLuint> actualIndices = f.indices();
    EXPECT_EQ(actualIndices, expectedIndices);
    EXPECT_EQ(expectedIndices.size(), f.indexCount());
  }

  TEST(FootprintTest, ctor_degenerate)
  {
    size_t width = 4;
    Footprint f(width);

    EXPECT_EQ(f.width(), width);
    EXPECT_EQ(f.depth(), width);

    std::vector<ngl::Vec2> expectedVertices =
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {2, 3}, {1, 3}, {0, 3}, {0, 2}, {0, 1}};
    std::vector<ngl::Vec2> actualVertices = f.vertices();
    EXPECT_EQ(actualVertices, expectedVertices);
    EXPECT_EQ(expectedVertices.size(), f.vertexCount());

    std::vector<GLuint> expectedIndices =
        {0, 1, 2, 3, 3, 4, 5, 6, 6, 7, 8, 9, 9, 10, 11, 0};
    std::vector<GLuint> actualIndices = f.indices();
    EXPECT_EQ(actualIndices, expectedIndices);
    EXPECT_EQ(expectedIndices.size(), f.indexCount());
  }

  TEST(FootprintTest, bindings)
  {
    size_t width = 4;
    Footprint f(width);

    GLuint &vao = f.vao();
    vao = 10; // Equivalent of binding texture
    EXPECT_EQ(f.vao(), vao);

    GLuint &vbo = f.vbo();
    vbo = 11; // Equivalent of binding texture
    EXPECT_EQ(f.vbo(), vbo);

    GLuint &ibo = f.ibo();
    ibo = 12; // Equivalent of binding texture
    EXPECT_EQ(f.ibo(), ibo);
  }

} // end namespace terraindeformer