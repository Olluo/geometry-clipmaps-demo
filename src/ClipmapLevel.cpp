/**
 * @file ClipmapLevel.cpp
 * @author Ollie Nicholls
 * @brief This class implements a level of the geoclipmap
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <cmath>

#include "ClipmapLevel.h"
#include "Manager.h"

namespace geoclipmap
{
  ClipmapLevel::ClipmapLevel(int _level,
                             Heightmap *_heightmap,
                             ClipmapLevel *_parent,
                             TrimLocation _trimLocation) noexcept : m_level{_level},
                                                                    m_heightmap{_heightmap},
                                                                    m_parent{_parent},
                                                                    m_trimLocation{_trimLocation}
  {
    size_t D = Manager::getInstance()->D();
    unsigned char L = Manager::getInstance()->L();

    m_texture = std::vector<ngl::Vec4>(D * D);
    m_scale = 1 << ((L - 1) - m_level);
  }

  void ClipmapLevel::setPosition(ngl::Vec2 _worldPosition,
                                 ngl::Vec2 _heightmapPosition,
                                 TrimLocation _trimLocation) noexcept
  {
    // TODO: Ideally this would only update the needed data
    // When querying the heightmap, it is assumed the heightmap is always at 0,0
    // So to get the correct pixels for this clipmaps texture we take its position
    // and loop up to D and add this value to the position, then grab the pixel
    // from the heightmap at this location adjusted for the scale
    m_worldPosition = _worldPosition;
    m_heightmapPosition = _heightmapPosition;
    m_trimLocation = _trimLocation;
    size_t D = Manager::getInstance()->D();

    // Get the integer part of the position as heightmap pixels are located at whole numbers
    int xPosInt = static_cast<int>(floor(m_heightmapPosition.m_x));
    int yPosInt = static_cast<int>(floor(m_heightmapPosition.m_y));

    for (int y = 0; y < D; y++)
    {
      for (int x = 0; x < D; x++)
      {
        // The positions to generate the pixels at must be scaled and offset based on the clipmap level
        m_texture[y * D + x] = generatePixelAt((xPosInt + x) * m_scale, (yPosInt + y) * m_scale);
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
    glActiveTexture(GL_TEXTURE0);

    if (!m_allocated)
    {
      glGenTextures(1, &m_textureName);
      m_allocated = true;
    }

    glBindBuffer(GL_TEXTURE_BUFFER, m_textureName);
    glBindTexture(GL_TEXTURE_BUFFER, m_textureName);
    glBufferData(GL_TEXTURE_BUFFER, m_texture.size() * sizeof(ngl::Vec4), &m_texture[0].m_x, GL_STATIC_DRAW);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_textureName);
  }

  // ======================================= Private methods =======================================

  ngl::Vec4 ClipmapLevel::generatePixelAt(int _x, int _y) noexcept
  {
    // for now just return the pixel in the heightmap

    // The value of the parent's pixel at this point which will be used in the shader for blending
    // ngl::Real coarsePixel = 0.0f;

    // Computation for getting the parent pixel data
    if (m_parent != nullptr)
    {
      // TODO: maybe wont get this working
    }

    // The value of the pixel for this clipmap level
    ngl::Real finePixel = m_heightmap->value(_x, _y);
    ngl::Vec3 fineColour = m_heightmap->colour(_x, _y);

    // Return a vec2 where m_x is the fine pixel value and m_y is the coarse
    return ngl::Vec4(fineColour, finePixel);
  }

} // end namespace geoclipmap