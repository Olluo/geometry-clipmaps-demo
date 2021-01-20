#ifndef TERRAIN_TESTING
#define TERRAIN_TESTING
#endif

#include <gtest/gtest.h>

#include "ClipmapLevel.h"
#include "Manager.h"

namespace geoclipmap
{
  TEST(ClipmapTest, ctor)
  {
    Manager *manager = Manager::getInstance();
    std::vector<ngl::Vec3> heightmapData{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    ngl::Real hmWidth = 4;
    ngl::Real hmHeight = 4;
    Heightmap *heightmap = new Heightmap(hmWidth, hmHeight, heightmapData);
    ClipmapLevel *parent = nullptr;

    ClipmapLevel c(0, heightmap, parent);

    EXPECT_EQ(c.m_heightmap, heightmap);
    EXPECT_EQ(c.m_level, 0);
    EXPECT_EQ(c.scale(), 1 << ((manager->L() - 1) - 0));
    EXPECT_EQ(c.position(), ngl::Vec2{});

    std::vector<ngl::Vec4> texture = c.m_texture;
    EXPECT_EQ(texture.size(), manager->D() * manager->D());
    // EXPECT_TRUE(c.left());
    // EXPECT_TRUE(c.bottom());
  } 

  // TEST(ClipmapTest, setPosition)
  // {
    // std::vector<ngl::Vec3> heightmapData{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    // ngl::Real hmWidth = 4;
    // ngl::Real hmHeight = 4;
    // Heightmap *heightmap = new Heightmap(hmWidth, hmHeight, heightmapData);
    // ClipmapLevel *parent = nullptr;

    // ClipmapLevel c(0, heightmap, parent);

    // ngl::Vec2 position{10, 6};
    // c.setPosition(position);
    // EXPECT_EQ(c.position(), position);
    // position = {-2, -2};
    // c.setPosition(position);
    // EXPECT_EQ(c.position(), position);

    // Expect correct parts of texture to equal the heightmap
    // for (int y = 0; y < CLIPMAP_D; y++)
    // {
    //   for (int x = 0; x < CLIPMAP_D; x++)
    //   {
    //     if (x < hmWidth && y < hmHeight)
    //     {
    //       EXPECT_EQ(c.m_texture[y * CLIPMAP_D + x], heightmapData[static_cast<int>(y * hmWidth + x)].lengthSquared());
    //     }
    //     else
    //     {
    //       EXPECT_EQ(c.m_texture[y * CLIPMAP_D + x], 0.0f);
    //     }
    //   }
    // }
    
  // }
} // namespace geoclipmap