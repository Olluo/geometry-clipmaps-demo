#ifndef TERRAIN_TESTING
#define TERRAIN_TESTING
#endif

#include <cmath>

#include <gtest/gtest.h>

#include "Heightmap.h"

namespace geoclipmap
{
  TEST(HeightmapTest, ctor)
  {
    std::vector<ngl::Vec3> data = {
        {0},
        {1},
        {2},
        {3},
        {4},
        {5},
        {6},
        {7},
        {8},
        {9},
        {10},
        {11},
        {12},
        {13},
        {14},
        {15},
    };
    size_t width = 4;
    size_t height = 4;
    Heightmap h(static_cast<ngl::Real>(width), static_cast<ngl::Real>(height), data);

    EXPECT_EQ(h.width(), width);
    EXPECT_EQ(h.depth(), height);

    for (size_t y = 0; y < height; y++)
    {
      for (size_t x = 0; x < width; x++)
      {
        EXPECT_EQ(
            h.value(static_cast<int>(x), static_cast<int>(y)),
            data[y * width + x].lengthSquared());
        EXPECT_EQ(
            h.colour(static_cast<int>(x), static_cast<int>(y)),
            data[y * width + x]);
      }
    }

    EXPECT_EQ(h.highestPoint(), pow(15.0f, 2) * 3);
  }
} // end namespace geoclipmap