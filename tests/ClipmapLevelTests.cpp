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

    EXPECT_EQ(c.m_level, 0);
    EXPECT_EQ(c.m_heightmap, heightmap);
    EXPECT_EQ(c.m_parent, parent);

    std::vector<ngl::Vec2> texture = c.m_texture;
    EXPECT_EQ(texture.size(), manager->D() * manager->D());

    EXPECT_EQ(c.scale(), 1 << ((manager->L() - 1) - 0));
    EXPECT_EQ(c.position(), ngl::Vec2{});
    EXPECT_EQ(c.trimLocation(), TrimLocation::TopRight);
  }

  TEST(ClipmapTest, ctor_specify_trimlocation)
  {
    Manager *manager = Manager::getInstance();
    std::vector<ngl::Vec3> heightmapData{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    ngl::Real hmWidth = 4;
    ngl::Real hmHeight = 4;
    Heightmap *heightmap = new Heightmap(hmWidth, hmHeight, heightmapData);
    ClipmapLevel *parent = nullptr;
    TrimLocation trimLocation = TrimLocation::All;

    ClipmapLevel c(0, heightmap, parent, trimLocation);

    EXPECT_EQ(c.m_level, 0);
    EXPECT_EQ(c.m_heightmap, heightmap);
    EXPECT_EQ(c.m_parent, parent);

    std::vector<ngl::Vec2> texture = c.m_texture;
    EXPECT_EQ(texture.size(), manager->D() * manager->D());

    EXPECT_EQ(c.scale(), 1 << ((manager->L() - 1) - 0));
    EXPECT_EQ(c.position(), ngl::Vec2{});
    EXPECT_EQ(c.trimLocation(), trimLocation);
  }

  TEST(ClipmapTest, setPosition)
  {
    Manager *manager = Manager::getInstance();
    std::vector<ngl::Vec3> heightmapData{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    ngl::Real hmWidth = 4;
    ngl::Real hmHeight = 4;
    Heightmap *heightmap = new Heightmap(hmWidth, hmHeight, heightmapData);
    ClipmapLevel *parent = nullptr;

    ClipmapLevel c(0, heightmap, parent);

    ngl::Vec2 worldPosition{1.0f, 2.0f};
    ngl::Vec2 heightmapPosition{3.0f, 4.0f};
    TrimLocation trimLocation = TrimLocation::TopLeft;

    c.setPosition(worldPosition, heightmapPosition, trimLocation);

    EXPECT_EQ(c.position(), worldPosition);
    EXPECT_EQ(c.m_heightmapPosition, heightmapPosition);
    EXPECT_EQ(c.trimLocation(), trimLocation);
  }
} // end namespace geoclipmap