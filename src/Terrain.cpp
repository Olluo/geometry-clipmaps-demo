#include "Terrain.h"

#include <iostream>

namespace terraindeformer
{

  Terrain::Terrain(size_t _widthX,
                   size_t _widthZ)
      : m_widthX{_widthX},
        m_widthZ{_widthZ},
        m_colours(m_widthX * m_widthZ, ngl::Vec3(0.9f))
  {
    // Center the grid on (0, 0, 0) by making the initial xz values start at -w/2 + 0.5
    float xPos = -(m_widthX / 2.0f) + 0.5f;
    float zPos = -(m_widthZ / 2.0f) + 0.5f;

    for (int z = 0; z < m_widthZ; z++)
    {
      for (int x = 0; x < m_widthX; x++)
      {
        m_xzValues.push_back(ngl::Vec2(xPos, zPos));
        m_yValues.push_back(0.0f);

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
      : m_widthX{_hmWidth},
        m_widthZ{_hmHeight},
        m_colours(m_widthX * m_widthZ, ngl::Vec3(0.9f))
  {
    // Center the grid on (0, 0, 0) by making the initial xz values start at -w/2 + 0.5
    float xPos = -(m_widthX / 2.0f) + 0.5f;
    float zPos = -(m_widthZ / 2.0f) + 0.5f;

    for (int z = 0; z < m_widthZ; z++)
    {
      for (int x = 0; x < m_widthX; x++)
      {
        m_xzValues.push_back(ngl::Vec2(xPos, zPos));
        m_yValues.push_back(_heightMap[z * m_widthX + x].m_r);
        m_colours.push_back(_heightMap[z * m_widthX + x]);

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
    return setHeight(_x, _z, 0.0f);
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
    return setColour(_x, _z, ngl::Vec3(0.9f));
  }

} // end namespace terraindeformer