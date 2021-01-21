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

    m_texture = std::vector<ngl::Vec2>(D * D);
    m_scale = 1 << ((L - 1) - m_level);
  }

  void ClipmapLevel::setPosition(ngl::Vec2 _worldPosition,
                                 ngl::Vec2 _heightmapPosition,
                                 TrimLocation _trimLocation) noexcept
  {
    m_worldPosition = _worldPosition;
    m_heightmapPosition = _heightmapPosition;
    m_trimLocation = _trimLocation;
  }

  void ClipmapLevel::updateTexture() noexcept
  {
    // TODO: Ideally this would only update the needed data
    // When querying the heightmap, it is assumed the heightmap is always at 0,0
    // So to get the correct pixels for this clipmaps texture we take its position
    // and loop up to D and add this value to the position, then grab the pixel
    // from the heightmap at this location adjusted for the scale
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
    if (!m_allocated)
    {
      // Generate the buffer and the texture object
      glGenBuffers(1, &m_tbo);
      glGenTextures(1, &m_tboTex);
      m_allocated = true;
    }

    // Bind the buffer, then buffer the data
    glBindBuffer(GL_TEXTURE_BUFFER, m_tbo);
    glBufferData(GL_TEXTURE_BUFFER, m_texture.size() * sizeof(ngl::Vec2), &m_texture[0].m_x, GL_STATIC_DRAW);

    // Set the active texture, then bind the texture to the written data to be read in the shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, m_tboTex);

    // Attach our texture buffer with RG32F format
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, m_tbo);
  }

  void ClipmapLevel::unbindTextures() noexcept
  {
    glBindTexture(GL_TEXTURE_BUFFER, 0);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);
  }

  ngl::Real ClipmapLevel::getPixelAt(int _x, int _y) noexcept
  {
    size_t D = Manager::getInstance()->D();

    // Get the integer part of the position as heightmap pixels are located at whole numbers
    int xPosInt = static_cast<int>(floor(m_heightmapPosition.m_x));
    int yPosInt = static_cast<int>(floor(m_heightmapPosition.m_y));

    int xLoc = _x - xPosInt;
    int yLoc = _y - yPosInt;

    if (xLoc < 0 || xLoc > D - 1 || yLoc < 0 || yLoc > D - 1)
    {
      return 0.0f;
    }

    // Only want to return fine pixel data
    return m_texture[(yLoc) * D + (xLoc)].m_x;
  }

  // ======================================= Private methods =======================================

  ngl::Vec2 ClipmapLevel::generatePixelAt(int _x, int _y) noexcept
  {
    // The value of the parent's pixel at this point which will be used in the shader for blending
    ngl::Real coarsePixel{0.0f};

    // Computation for getting the parent pixel data
    if (m_parent != nullptr)
    {
      if (_x % 2 == 0)
      {
        // X is even
        if (_y % 2 == 0)
        {
          // Y is even
          // TODO: This causes an issue because the parent clipmap is not in the right position when this is called so it cant
          // TODO: get the correct value
          // TODO: Just a thought, why not artificially make the value here as we have access to the entire heightmap anyway
          coarsePixel = m_parent->getPixelAt(static_cast<int>(0.5f * _x), static_cast<int>(0.5f * _y));
        }
        else
        {
          // Y is odd
          coarsePixel = m_parent->getPixelAt(static_cast<int>(0.5f * _x), static_cast<int>(floor(0.5f * _y))) +
                        m_parent->getPixelAt(static_cast<int>(0.5f * _x), static_cast<int>(ceil(0.5f * _y)));
          coarsePixel *= 0.5; // Average of two values
        }
      }
      else
      {
        // X is odd
        if (_y % 2 == 0)
        {
          // Y is even
          coarsePixel = m_parent->getPixelAt(static_cast<int>(floor(0.5f * _x)), static_cast<int>(0.5f * _y)) +
                        m_parent->getPixelAt(static_cast<int>(ceil(0.5f * _x)), static_cast<int>(0.5f * _y));
          coarsePixel *= 0.5; // Average of two values
        }
        else
        {
          // Y is odd
          coarsePixel = m_parent->getPixelAt(static_cast<int>(floor(0.5f * _x)), static_cast<int>(floor(0.5f * _y))) +
                        m_parent->getPixelAt(static_cast<int>(ceil(0.5f * _x)), static_cast<int>(ceil(0.5f * _y))) +
                        m_parent->getPixelAt(static_cast<int>(floor(0.5f * _x)), static_cast<int>(ceil(0.5f * _y))) +
                        m_parent->getPixelAt(static_cast<int>(ceil(0.5f * _x)), static_cast<int>(floor(0.5f * _y)));
          coarsePixel *= 0.25; // Average of four values
        }
      }
    }

    // The value of the pixel for this clipmap level
    ngl::Real finePixel = m_heightmap->value(_x, _y);

    // Return a vec2 where r fine pixel and g is the coarse pixel
    return ngl::Vec2{finePixel, coarsePixel};
  }

} // end namespace geoclipmap