#ifndef TERRAIN_TESTING
#define TERRAIN_TESTING
#endif

#include <gtest/gtest.h>

#include "Manager.h"
#include "Terrain.h"

namespace geoclipmap
{
  TEST(TerrainTest, ctor)
  {
    Manager *manager = Manager::getInstance();
    std::vector<ngl::Vec3> heightmapData{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    ngl::Real hmWidth = 4;
    ngl::Real hmHeight = 4;
    Heightmap *heightmap = new Heightmap(hmWidth, hmHeight, heightmapData);

    Terrain t(heightmap);

    // Check internal data
    EXPECT_EQ(t.m_heightmap, heightmap);
    EXPECT_EQ(t.m_footprints.size(), 6);
    EXPECT_EQ(t.m_position, ngl::Vec2{});
    EXPECT_EQ(t.m_clipmaps.size(), manager->L());
    EXPECT_EQ(t.m_activeFinest, manager->L() - 1);
    EXPECT_EQ(t.m_activeCoarsest, 0);
    EXPECT_EQ(t.m_locations.size(), 25);

    // Check all footprints initialised correctly
    EXPECT_TRUE(t.m_footprints[static_cast<int>(FootprintType::Block)] != nullptr);
    EXPECT_TRUE(t.m_footprints[static_cast<int>(FootprintType::FixupHorizontal)] != nullptr);
    EXPECT_TRUE(t.m_footprints[static_cast<int>(FootprintType::FixupVertical)] != nullptr);
    EXPECT_TRUE(t.m_footprints[static_cast<int>(FootprintType::InteriorTrimHorizontal)] != nullptr);
    EXPECT_TRUE(t.m_footprints[static_cast<int>(FootprintType::InteriorTrimVertical)] != nullptr);
    EXPECT_TRUE(t.m_footprints[static_cast<int>(FootprintType::OuterDegenerateRing)] != nullptr);

    // Check locations initialised correctly
    for (auto location : t.m_locations)
    {
      EXPECT_TRUE(location != nullptr);
    }

    // Check clipmaps initialised correctly
    for (auto clipmap : t.m_clipmaps)
    {
      EXPECT_TRUE(clipmap != nullptr);
    }
  }
} // end namespace geoclipmap