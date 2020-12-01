#include <iostream>

#include "Constants.h"
#include "Terrain.h"

namespace terraindeformer
{
  void Terrain::generateFootprints()
  {
    m_footprints[static_cast<int>(FootprintType::Block)] = new Footprint(CLIPMAP_M, CLIPMAP_M);
    m_footprints[static_cast<int>(FootprintType::FixupHorizontal)] = new Footprint(CLIPMAP_M, 3);
    m_footprints[static_cast<int>(FootprintType::FixupVertical)] = new Footprint(3, CLIPMAP_M);
    m_footprints[static_cast<int>(FootprintType::InteriorTrimHorizontal)] = new Footprint((2 * CLIPMAP_M) + 1, 2);
    m_footprints[static_cast<int>(FootprintType::InteriorTrimVertical)] = new Footprint(2, (2 * CLIPMAP_M) + 1);
    m_footprints[static_cast<int>(FootprintType::OuterDegenerateRing)] = new Footprint((4 * CLIPMAP_M) - 1);
  }

  void Terrain::generateLocations()
  {
    // This is used for the bottom left corner displacement
    int neg2M = static_cast<int>(-2 * (CLIPMAP_M) + 1);
    int m = static_cast<int>(CLIPMAP_M);

    // See https://developer.nvidia.com/sites/all/modules/custom/gpugems/books/GPUGems2/elementLinks/02_clipmaps_05.jpg
    // Bottom left blocks
    m_locations.push_back(new FootprintLocation(neg2M, -m, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(neg2M, neg2M, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(-m, neg2M, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Bottom right blocks
    m_locations.push_back(new FootprintLocation(1, neg2M, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(m, neg2M, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(m, -m, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Top right blocks
    m_locations.push_back(new FootprintLocation(1, m, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(m, m, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(m, 1, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Top left blocks
    m_locations.push_back(new FootprintLocation(neg2M, 1, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(neg2M, m, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(-m, m, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Inner blocks (only used in finest level)
    m_locations.push_back(new FootprintLocation(0, 0, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(1 - m, 0, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(0, 1 - m, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(1 - m, 1 - m, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Fix-up footprints (l, r, t, b)
    m_locations.push_back(new FootprintLocation(neg2M, -1, m_footprints[static_cast<int>(FootprintType::FixupHorizontal)]));
    m_locations.push_back(new FootprintLocation(m, -1, m_footprints[static_cast<int>(FootprintType::FixupHorizontal)]));
    m_locations.push_back(new FootprintLocation(-1, m, m_footprints[static_cast<int>(FootprintType::FixupVertical)]));
    m_locations.push_back(new FootprintLocation(-1, neg2M, m_footprints[static_cast<int>(FootprintType::FixupVertical)]));

    // Interior trims (l, r, t, b)
    m_locations.push_back(new FootprintLocation(-m, -m, m_footprints[static_cast<int>(FootprintType::InteriorTrimVertical)]));
    m_locations.push_back(new FootprintLocation(m - 1, -m, m_footprints[static_cast<int>(FootprintType::InteriorTrimVertical)]));
    m_locations.push_back(new FootprintLocation(-m, m - 1, m_footprints[static_cast<int>(FootprintType::InteriorTrimHorizontal)]));
    m_locations.push_back(new FootprintLocation(-m, -m, m_footprints[static_cast<int>(FootprintType::InteriorTrimHorizontal)]));

    // Outer degenerated ring
    m_locations.push_back(new FootprintLocation(neg2M, neg2M, m_footprints[static_cast<int>(FootprintType::OuterDegenerateRing)]));
  }

  void Terrain::generateClipmaps()
  {
    Clipmap *parent = nullptr;
    for (int l = CLIPMAP_L - 1; l >= 0; l--)
    {
      m_clipmaps[l] = new Clipmap(l, m_widthX, m_widthZ, m_yValues, parent);
      parent = m_clipmaps[l];
    }
  }

  Terrain::Terrain(size_t _widthX, size_t _widthZ) : m_widthX{_widthX},
                                                     m_widthZ{_widthZ},
                                                     m_yValues(m_widthX * m_widthZ, DEFAULT_HEIGHT),
                                                     m_colours(m_widthX * m_widthZ, ngl::Vec3(DEFAULT_COLOUR)),
                                                     m_footprints(6),
                                                     m_clipmaps(CLIPMAP_L)
  {
  }

  void Terrain::initialize()
  {
    generateFootprints();
    generateLocations();
    generateClipmaps();
  }

  Terrain::Terrain(size_t _hmWidth, size_t _hmHeight, std::vector<ngl::Vec3> _heightMap) : Terrain(_hmWidth, _hmHeight)
  {
    loadHeightMap(_hmWidth, _hmHeight, _heightMap);
  }

  bool Terrain::loadHeightMap(size_t _hmWidth, size_t _hmHeight, std::vector<ngl::Vec3> _heightMap)
  {
    if (_hmWidth > m_widthX || _hmHeight > m_widthZ)
    {
      return false;
    }

    for (int z = 0; z < _hmHeight; z++)
    {
      for (int x = 0; x < _hmWidth; x++)
      {
        auto pixel = _heightMap[z * _hmWidth + x];
        m_yValues[z * _hmWidth + x] = pixel.m_r;
        m_colours[z * _hmWidth + x] = pixel;
      }
    }

    return true;
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

  // std::vector<float> *Terrain::yValues()
  // {
  //   return &m_yValues;
  // }

  // std::vector<ngl::Vec3> *Terrain::colours()
  // {
  //   return &m_colours;
  // }

  // const std::vector<ngl::Vec2> &Terrain::fpVertices(FootprintType _footprint) const
  // {
  //   return m_footprints[static_cast<int>(_footprint)]->vertices();
  // }

  // const std::vector<GLuint> &Terrain::fpIndices(FootprintType _footprint) const
  // {
  //   return m_footprints[static_cast<int>(_footprint)]->indices();
  // }

  std::vector<Clipmap *> &Terrain::clipmaps()
  {
    return m_clipmaps;
  }

  std::vector<Footprint *> &Terrain::footprints()
  {
    return m_footprints;
  }

  std::vector<FootprintLocation *> Terrain::selectLocations(int _selection)
  {
    // TODO: this should return the required footprints based on the selection made
    std::vector<FootprintLocation *> selectedLocations;
    for (auto location : m_locations)
    {
      if (true)
      {
        selectedLocations.push_back(location);
      }
    }
    return selectedLocations;
  }

} // end namespace terraindeformer