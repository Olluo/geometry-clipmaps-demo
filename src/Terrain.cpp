#include <iostream>

#include "Terrain.h"

namespace terraindeformer
{

  size_t Terrain::calculateAdjustedWidth(size_t _width) const
  {
    // This formula rounds up the width to the nearest tile size then adds 1
    // e.g. TILE_SIZE = 16, _width = 7, m_width = 7 + 16 - (7 % 16) + 1 = 7 + 16 - 7 + 1 = 17
    // 2 other cases are when the width is the tile size already (return width + 1), or width is
    // tile size + 1 already (return width)
    size_t remainder = _width % TILE_SIZE;
    switch (remainder)
    {
    case 0:
      return _width + 1;
    case 1:
      return _width;
    default:
      return _width + TILE_SIZE - remainder + 1;
    }
  }

  Terrain::Terrain(size_t _widthX,
                   size_t _widthZ)
      : m_widthX{calculateAdjustedWidth(_widthX)},
        m_widthZ{calculateAdjustedWidth(_widthZ)},
        m_yValues(m_widthX * m_widthZ, DEFAULT_HEIGHT),
        m_colours(m_widthX * m_widthZ, ngl::Vec3(DEFAULT_COLOUR))
  {
    // Center the grid on (0, 0, 0) by making the initial xz values start at -w/2 + 0.5
    float xPos = -(m_widthX / 2.0f) + 0.5f;
    float zPos = -(m_widthZ / 2.0f) + 0.5f;

    for (int z = 0; z < m_widthZ; z++)
    {
      for (int x = 0; x < m_widthX; x++)
      {
        m_xzValues.push_back(ngl::Vec2(xPos, zPos));

        // Increment x position by 1
        xPos++;
      }
      // Increment z position by 1
      zPos++;
      // Reset x position to initial
      xPos = -(m_widthX / 2.0f);
    }
  }

  Terrain::Terrain(size_t _hmWidth, size_t _hmHeight, std::vector<ngl::Vec3> _heightMap)
      : m_widthX{calculateAdjustedWidth(_hmWidth)},
        m_widthZ{calculateAdjustedWidth(_hmHeight)}
  {
    // Center the grid on (0, 0, 0) by making the initial xz values start at -w/2 + 0.5
    float xPos = -(m_widthX / 2.0f) + 0.5f;
    float zPos = -(m_widthZ / 2.0f) + 0.5f;

    for (int z = 0; z < m_widthZ; z++)
    {
      for (int x = 0; x < m_widthX; x++)
      {
        m_xzValues.push_back(ngl::Vec2(xPos, zPos));
        if (x < _hmWidth && z < _hmHeight)
        {
          m_yValues.push_back(_heightMap[z * _hmWidth + x].m_r);
          m_colours.push_back(_heightMap[z * _hmWidth + x]);
        }
        else
        {
          m_yValues.push_back(DEFAULT_HEIGHT);
          m_colours.push_back(DEFAULT_COLOUR);
        }

        // Increment x position by 1
        xPos++;
      }
      // Increment z position by 1
      zPos++;
      // Reset x position to initial
      xPos = -(m_widthX / 2.0f);
    }
  }

  size_t Terrain::widthX() const
  {
    return m_widthX;
  }

  size_t Terrain::widthZ() const
  {
    return m_widthZ;
  }

  float Terrain::height(int _x, int _z) const
  {
    return m_yValues[_z * m_widthX + _x];
  }

  ngl::Vec3 Terrain::colour(int _x, int _z) const
  {
    return m_colours[_z * m_widthX + _x];
  }

  bool Terrain::setHeight(int _x, int _z, float _height)
  {
    if (_x > m_widthX || _z > m_widthZ)
    {
      return false;
    }

    m_yValues[_z * m_widthX + _x] = _height;
    return true;
  }

  bool Terrain::resetHeight(int _x, int _z)
  {
    return setHeight(_x, _z, DEFAULT_HEIGHT);
  }

  bool Terrain::setColour(int _x, int _z, ngl::Vec3 _colour)
  {
    if (_x > m_widthX || _z > m_widthZ)
    {
      return false;
    }

    m_colours[_z * m_widthX + _x] = _colour;
    return true;
  }

  bool Terrain::resetColour(int _x, int _z)
  {
    return setColour(_x, _z, ngl::Vec3(DEFAULT_COLOUR));
  }

  void Terrain::print()
  {
    std::cout << "Printing terrain\n";
    for (int z = 0; z < m_widthZ; z++)
    {
      for (int x = 0; x < m_widthX; x++)
      {
        std::cout << height(x, z) << ' ';
      }
      std::cout << '\n';
    }
    std::cout << "End terrain\n";
  }

} // end namespace terraindeformer