#include "ClipmapLevel.h"
#include "Constants.h"

namespace terraindeformer
{

  float ClipmapLevel::generatePixelAt(ngl::Real _x, ngl::Real _y) noexcept
  {
    // for now just return the pixel in the heightmap
    // in future we will want to get an adjusted height that is between the coarse
    // and fine level to avoid holes in the terrain

    return m_heightmap->value(_x, _y);
  }

  ClipmapLevel::ClipmapLevel(int _level, Heightmap *_heightmap, ClipmapLevel *_parent) noexcept : m_level{_level},
                                                                                                  m_heightmap{_heightmap},
                                                                                                  m_parent{_parent},
                                                                                                  m_texture(CLIPMAP_D * CLIPMAP_D),
                                                                                                  //  m_scale{CLIPMAP_D * (1 << m_level)}
                                                                                                  m_scale{(1 << m_level)}
  {
  }

  void ClipmapLevel::setPosition(ngl::Vec2 _position) noexcept
  {
    // TODO: caching - get data from parent
    m_position = _position;

    int halfD = CLIPMAP_D / 2;
    int xStart = static_cast<int>(m_position.m_x) - halfD;
    int yStart = static_cast<int>(m_position.m_y) - halfD;

    for (int y = 0; y < CLIPMAP_D; y++)
    {
      xStart = static_cast<int>(m_position.m_x) - halfD;
      for (int x = 0; x < CLIPMAP_D; x++)
      {
        int xPos = static_cast<int>(xStart * m_scale);
        int yPos = static_cast<int>(yStart * m_scale);
        m_texture[y * CLIPMAP_D + x] = generatePixelAt(static_cast<ngl::Real>(xPos), static_cast<ngl::Real>(yPos));
        xStart++;
      }
      yStart++;
    }
  }

  int ClipmapLevel::scale() const noexcept
  {
    return m_scale;
  }

  const ngl::Vec2 &ClipmapLevel::position() const noexcept
  {
    return m_position;
  }

  bool ClipmapLevel::left() const noexcept
  {
    // Get fractional part of the x value and compare to 0.5
    // The positions of each sub clipmap are half the position of the parent
    // this means that it will alternate between left and right for each clipmap
    return m_position.m_x - static_cast<long>(m_position.m_x) < 0.5f;
  }

  bool ClipmapLevel::bottom() const noexcept
  {
    // Get fractional part of the y value and compare to 0.5
    // The positions of each sub clipmap are half the position of the parent
    // this means that it will alternate between bottom and top for each clipmap
    return m_position.m_y - static_cast<long>(m_position.m_y) < 0.5f;
  }

  void ClipmapLevel::bindTextures() noexcept
  {
    // TODO: set colour
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

} // end namespace terraindeformer