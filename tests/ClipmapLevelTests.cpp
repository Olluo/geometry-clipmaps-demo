#include <gtest/gtest.h>

#include "ClipmapLevel.h"
#include "Constants.h"

namespace terraindeformer
{
  // TEST(ClipmapTest, ctor)
  // {
  //   std::vector<float> heightMap{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  //   size_t hmWidth = 4;
  //   size_t hmHeight = 4;
  //   ClipmapLevel *parent = nullptr;

  //   ClipmapLevel c(0, hmWidth, hmHeight, heightMap, parent);

  //   std::vector<float> texture = c.texture();
  //   EXPECT_EQ(texture.size(), CLIPMAP_D * CLIPMAP_D);

  //   EXPECT_EQ(c.scale(), 1);
  // }

  // TEST(ClipmapTest, setPosition)
  // {
  //   std::vector<float> heightMap{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  //   size_t hmWidth = 4;
  //   size_t hmHeight = 4;
  //   ClipmapLevel *parent = nullptr;

  //   ClipmapLevel c(0, hmWidth, hmHeight, heightMap, parent);

  //   ngl::Vec2 position{10, 6};
  //   c.setPosition(position);
  //   EXPECT_EQ(c.position(), position);
  //   // Expect correct parts of texture to equal the heightmap
  //   // EXPECT_EQ(c.texture(), heightmap)
  // }

  // TEST(ClipmapTest, bindTextureName)
  // {
  //   std::vector<float> heightMap{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  //   size_t hmWidth = 4;
  //   size_t hmHeight = 4;
  //   ClipmapLevel *parent = nullptr;

  //   ClipmapLevel c(0, hmWidth, hmHeight, heightMap, parent);

  //   GLuint &textureName = c.textureName();
  //   // Equivalent of binding texture
  //   textureName = 10;

  //   EXPECT_EQ(c.textureName(), textureName);
  // }

} // namespace terraindeformer