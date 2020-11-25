#include "gtest/gtest.h"

#include "Terrain.h"

namespace terraindeformer
{
  TEST(TerrainTest, ctor_widthX_widthZ)
  {
    size_t widthX = 64;
    size_t widthZ = 64;
    Terrain t(widthX, widthZ);

    EXPECT_EQ(t.widthX(), widthX);
    EXPECT_EQ(t.widthZ(), widthZ);

    for (int z = 0; z < t.widthZ(); z++)
    {
      for (int x = 0; x < t.widthX(); x++)
      {
        auto height = t.height(x, z);
        auto colour = t.colour(x, z);
        EXPECT_EQ(height, 0.0f);
        EXPECT_EQ(colour, ngl::Vec3(0.9f));
      }
    }
  }

  TEST(TerrainTest, ctor_heightmap)
  {
    std::vector<ngl::Vec3> heightMap{
      ngl::Vec3(0, 0, 0), ngl::Vec3(0, 1, 0), ngl::Vec3(0, 2, 0), ngl::Vec3(0, 3, 0),
      ngl::Vec3(0, 4, 0), ngl::Vec3(0, 5, 0), ngl::Vec3(0, 6, 0), ngl::Vec3(0, 7, 0),
      ngl::Vec3(0, 8, 0), ngl::Vec3(0, 9, 0), ngl::Vec3(0, 10, 0), ngl::Vec3(0, 11, 0),
      ngl::Vec3(0, 12, 0), ngl::Vec3(0, 13, 0), ngl::Vec3(0, 14, 0), ngl::Vec3(0, 15, 0),
    };
    size_t hmWidth = 4;
    size_t hmHeight = 4;
    Terrain t(hmWidth, hmHeight, heightMap);

    EXPECT_EQ(t.widthX(), hmWidth);
    EXPECT_EQ(t.widthZ(), hmHeight);

    for (int z = 0; z < hmWidth; z++)
    {
      for (int x = 0; x < hmHeight; x++)
      {
        auto height = t.height(x, z);
        auto colour = t.colour(x, z);
        EXPECT_EQ(height, heightMap[z * hmWidth + x].m_r);
        EXPECT_EQ(colour, ngl::Vec3(0.9f));
      }
    }
  }

  TEST(TerrainTest, setHeight_valid)
  {
    size_t widthX = 64;
    size_t widthZ = 64;
    Terrain t(widthX, widthZ);

    EXPECT_TRUE(t.setHeight(10, 10, 10.0f));

    EXPECT_EQ(t.height(10, 10), 10.0f);
  }

  TEST(TerrainTest, setHeight_invalid)
  {
    size_t widthX = 64;
    size_t widthZ = 64;
    Terrain t(widthX, widthZ);

    EXPECT_FALSE(t.setHeight(100, 100, 10.0f));
  }

  TEST(TerrainTest, setColour_valid)
  {
    size_t widthX = 64;
    size_t widthZ = 64;
    Terrain t(widthX, widthZ);

    EXPECT_TRUE(t.setColour(10, 10, ngl::Vec3(1.0f)));

    EXPECT_EQ(t.colour(10, 10), ngl::Vec3(1.0f));
  }

  TEST(TerrainTest, setColour_invalid)
  {
    size_t widthX = 64;
    size_t widthZ = 64;
    Terrain t(widthX, widthZ);

    EXPECT_FALSE(t.setColour(100, 100, ngl::Vec3(1.0f)));
  }

  TEST(TerrainTest, resetHeight_valid)
  {
    size_t widthX = 64;
    size_t widthZ = 64;
    Terrain t(widthX, widthZ);

    t.setHeight(10, 10, 10.0f);
    EXPECT_TRUE(t.resetHeight(10, 10));
    EXPECT_EQ(t.height(10, 10), 0.0f);
  }

  TEST(TerrainTest, resetColour_valid)
  {
    size_t widthX = 64;
    size_t widthZ = 64;
    Terrain t(widthX, widthZ);

    t.setColour(10, 10, ngl::Vec3(1.0f));
    EXPECT_TRUE(t.resetColour(10, 10));
    EXPECT_EQ(t.colour(10, 10), ngl::Vec3(0.9f));
  }
} // end namespace terraindeformer