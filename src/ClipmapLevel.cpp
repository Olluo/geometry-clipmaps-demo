#include "ClipmapLevel.h"
#include "Constants.h"

#include <cmath>

namespace terraindeformer
{
  ClipmapLevel::ClipmapLevel(int _level,
                             Heightmap *_heightmap,
                             ClipmapLevel *_parent,
                             TrimLocation _trimLocation) noexcept : m_level{_level},
                                                                    m_heightmap{_heightmap},
                                                                    m_parent{_parent},
                                                                    m_trimLocation{_trimLocation},
                                                                    m_texture(CLIPMAP_D * CLIPMAP_D),
                                                                    m_scale{1 << ((CLIPMAP_L - 1) - m_level)}
  {
  }

  void ClipmapLevel::setPosition(ngl::Vec2 _worldPosition, ngl::Vec2 _heightmapPosition, TrimLocation _trimLocation) noexcept
  {
    // When querying the heightmap, it is assumed the heightmap is always at 0,0
    // So to get the correct pixels for this clipmaps texture we take its position
    // and loop up to CLIPMAP_D and add this value to the position, then grab the pixel
    // from the heightmap at this location adjusted for the scale
    m_worldPosition = _worldPosition;
    m_heightmapPosition = _heightmapPosition;
    m_trimLocation = _trimLocation;

    // Get the integer part of the position as heightmap pixels are located at whole numbers
    int xPosInt = static_cast<int>(floor(m_heightmapPosition.m_x));
    int yPosInt = static_cast<int>(floor(m_heightmapPosition.m_y));

    for (int y = 0; y < CLIPMAP_D; y++)
    {
      for (int x = 0; x < CLIPMAP_D; x++)
      {
        // The positions to generate the pixels at must be scaled and offset based on the clipmap level
        m_texture[y * CLIPMAP_D + x] = generatePixelAt((xPosInt + x) * m_scale, (yPosInt + y) * m_scale);
      }
    }
  }

  int ClipmapLevel::scale() const noexcept
  {
    return m_scale;
  }

  const ngl::Vec2 &ClipmapLevel::position() const noexcept
  {
    return m_worldPosition;
  }

  TrimLocation ClipmapLevel::trimLocation() const noexcept
  {
    return m_trimLocation;
  }

  void ClipmapLevel::bindTextures() noexcept
  {
    // TODO: set colour or implement default textures for rock, grass, snow etc.
    glActiveTexture(GL_TEXTURE0);

    if (!m_allocated)
    {
      glGenTextures(1, &m_textureName);
      m_allocated = true;
    }

    glBindBuffer(GL_TEXTURE_BUFFER, m_textureName);
    glBindTexture(GL_TEXTURE_BUFFER, m_textureName);
    glBufferData(GL_TEXTURE_BUFFER, m_texture.size() * sizeof(ngl::Real), &m_texture[0], GL_STATIC_DRAW);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, m_textureName);
  }

  // ======================================= Private methods =======================================

  float ClipmapLevel::generatePixelAt(int _x, int _y) noexcept
  {
    // for now just return the pixel in the heightmap

    // The value of the parent's pixel at this point which will be used in the shader for blending
    float coarsePixel = 0.0f;

    // Computation for getting the parent pixel data
    if (m_parent != nullptr)
    {
      // TODO
    }

    // The value of the pixel for this clipmap level
    float finePixel = m_heightmap->value(_x, _y);

    // Return a float where the decimal part is the fine pixel value and the integer part is the coarse
    // return (coarsePixel * 512.0f + finePixel) / 512.0f;
    return finePixel;
  }

} // end namespace terraindeformer