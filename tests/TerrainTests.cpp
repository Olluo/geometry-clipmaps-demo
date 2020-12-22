#ifndef TERRAIN_TESTING
#define TERRAIN_TESTING
#endif

#include <gtest/gtest.h>

#include "Constants.h"
#include "Terrain.h"

namespace terraindeformer
{
  // TEST(TerrainTest, ctor_width_depth_tilesizeplus1)
  // {
  //   size_t width = 16;
  //   size_t depth = 16;
  //   Terrain t(width, depth);
  //   // t.print();
  //   // EXPECT_TRUE(false);

  //   EXPECT_EQ(t.width(), width);
  //   EXPECT_EQ(t.depth(), depth);

  //   for (int y = 0; y < t.depth(); y++)
  //   {
  //     for (int x = 0; x < t.width(); x++)
  //     {
  //       auto height = t.height(x, y);
  //       auto colour = t.colour(x, y);
  //       EXPECT_EQ(height, 0.0f);
  //       EXPECT_EQ(colour, ngl::Vec3(0.9f));
  //     }
  //   }
  // }

  // TEST(TerrainTest, ctor_heightmap)
  // {
  //   std::vector<ngl::Vec3> heightMap{
  //       ngl::Vec3(0, 0, 0),
  //       ngl::Vec3(1, 0, 0),
  //       ngl::Vec3(2, 0, 0),
  //       ngl::Vec3(3, 0, 0),
  //       ngl::Vec3(4, 0, 0),
  //       ngl::Vec3(5, 0, 0),
  //       ngl::Vec3(6, 0, 0),
  //       ngl::Vec3(7, 0, 0),
  //       ngl::Vec3(8, 0, 0),
  //       ngl::Vec3(9, 0, 0),
  //       ngl::Vec3(10, 0, 0),
  //       ngl::Vec3(11, 0, 0),
  //       ngl::Vec3(12, 0, 0),
  //       ngl::Vec3(13, 0, 0),
  //       ngl::Vec3(14, 0, 0),
  //       ngl::Vec3(15, 0, 0),
  //   };
  //   size_t hmWidth = 4;
  //   size_t hmHeight = 4;
  //   Terrain t(hmWidth, hmHeight, heightMap);

  //   EXPECT_EQ(t.width(), hmWidth);
  //   EXPECT_EQ(t.depth(), hmHeight);

  //   for (int y = 0; y < hmHeight; y++)
  //   {
  //     for (int x = 0; x < hmWidth; x++)
  //     {
  //       auto height = t.height(x, y);
  //       auto colour = t.colour(x, y);
  //       if (x < hmWidth && y < hmHeight)
  //       {
  //         EXPECT_EQ(height, heightMap[y * hmWidth + x].m_r);
  //         EXPECT_EQ(colour, heightMap[y * hmWidth + x]);
  //       }
  //       else
  //       {
  //         EXPECT_EQ(height, DEFAULT_HEIGHT);
  //         EXPECT_EQ(colour, ngl::Vec3(DEFAULT_COLOUR));
  //       }
  //     }
  //   }
  // }

  // TEST(TerrainTest, setHeight_valid)
  // {
  //   size_t width = 64;
  //   size_t depth = 64;
  //   Terrain t(width, depth);

  //   EXPECT_TRUE(t.setHeight(10, 10, 10.0f));

  //   EXPECT_EQ(t.height(10, 10), 10.0f);
  // }

  // TEST(TerrainTest, setHeight_invalid)
  // {
  //   size_t width = 64;
  //   size_t depth = 64;
  //   Terrain t(width, depth);

  //   EXPECT_FALSE(t.setHeight(100, 100, 10.0f));
  // }

  // TEST(TerrainTest, setColour_valid)
  // {
  //   size_t width = 64;
  //   size_t depth = 64;
  //   Terrain t(width, depth);

  //   EXPECT_TRUE(t.setColour(10, 10, ngl::Vec3(1.0f)));

  //   EXPECT_EQ(t.colour(10, 10), ngl::Vec3(1.0f));
  // }

  // TEST(TerrainTest, setColour_invalid)
  // {
  //   size_t width = 64;
  //   size_t depth = 64;
  //   Terrain t(width, depth);

  //   EXPECT_FALSE(t.setColour(100, 100, ngl::Vec3(1.0f)));
  // }

  // TEST(TerrainTest, resetHeight_valid)
  // {
  //   size_t width = 64;
  //   size_t depth = 64;
  //   Terrain t(width, depth);

  //   t.setHeight(10, 10, 10.0f);
  //   EXPECT_TRUE(t.resetHeight(10, 10));
  //   EXPECT_EQ(t.height(10, 10), 0.0f);
  // }

  // TEST(TerrainTest, resetColour_valid)
  // {
  //   size_t width = 64;
  //   size_t depth = 64;
  //   Terrain t(width, depth);

  //   t.setColour(10, 10, ngl::Vec3(1.0f));
  //   EXPECT_TRUE(t.resetColour(10, 10));
  //   EXPECT_EQ(t.colour(10, 10), ngl::Vec3(0.9f));
  // }
} // end namespace terraindeformer