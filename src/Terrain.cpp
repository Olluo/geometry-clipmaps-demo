/**
 * @file Terrain.cpp
 * @author Ollie Nicholls
 * @brief Based on GPUGems2 by NVIDIA https://developer.nvidia.com/gpugems/gpugems2/part-i-geometric-complexity/chapter-2-terrain-rendering-using-gpu-based-geometry
 * and IIS.SLSharp Geoclipmap https://github.com/IgniteInteractiveStudio/SLSharp
 * 
 */
#include <iostream>

#include "Constants.h"
#include "Terrain.h"

namespace terraindeformer
{
  Terrain::Terrain(Heightmap *_heightmap) : m_heightmap{_heightmap},
                                            m_footprints(6),
                                            m_clipmaps(CLIPMAP_L)
  {
    generateFootprints();
    generateLocations();
    generateClipmaps();
    updatePosition();
  }

  std::vector<ClipmapLevel *> &Terrain::clipmaps()
  {
    return m_clipmaps;
  }

  std::vector<Footprint *> &Terrain::footprints()
  {
    return m_footprints;
  }

  std::vector<FootprintLocation *> Terrain::selectLocations(FootprintSelection _selection)
  {
    std::vector<int> selectionIndices;
    switch (_selection)
    {
    case FootprintSelection::All:
      selectionIndices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
      break;
    case FootprintSelection::TopLeft:
      selectionIndices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19, 20, 22, 24};
      break;
    case FootprintSelection::TopRight:
      selectionIndices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19, 21, 22, 24};
      break;
    case FootprintSelection::BottomLeft:
      selectionIndices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19, 20, 23, 24};
      break;
    case FootprintSelection::BottomRight:
      selectionIndices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19, 21, 23, 24};
      break;
    default:
      break;
    }

    std::vector<FootprintLocation *> selectedLocations;
    for (auto i : selectionIndices)
    {
      selectedLocations.push_back(m_locations[i]);
    }

    return selectedLocations;
  }

  void Terrain::updatePosition()
  {
    auto position = m_position;
    std::vector<ngl::Vec2> positions;
    for (int i = 0; i < CLIPMAP_L; i++)
    {
      positions.push_back(position);
      position /= 2;
    }

    for (int i = CLIPMAP_L - 1; i >= 0; i--)
    {
      m_clipmaps[i]->setPosition(positions[i]);
    }
  }

  void Terrain::move(float _x, float _y)
  {
    m_position.m_x += _x;
    m_position.m_y += _y;
    updatePosition();
  }

  // ======================================= Private methods =======================================

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
    // This is used for the bottom left corner displacement of each footprint
    // 0, 0 is the local coordinate centre of the clipmap level
    // The m_position of each clipmap level is their world coordinate
    int h = static_cast<int>(CLIPMAP_H);
    int m = static_cast<int>(CLIPMAP_M);

    // See https://developer.nvidia.com/sites/all/modules/custom/gpugems/books/GPUGems2/elementLinks/02_clipmaps_05.jpg
    // Bottom left blocks
    m_locations.push_back(new FootprintLocation(h, -m, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(h, h, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(-m, h, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Bottom right blocks
    m_locations.push_back(new FootprintLocation(1, h, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(m, h, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(m, -m, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Top right blocks
    m_locations.push_back(new FootprintLocation(1, m, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(m, m, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(m, 1, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Top left blocks
    m_locations.push_back(new FootprintLocation(h, 1, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(h, m, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(-m, m, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Inner blocks (only used in finest level)
    m_locations.push_back(new FootprintLocation(0, 0, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(1 - m, 0, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(0, 1 - m, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(1 - m, 1 - m, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Fix-up footprints (l, r, t, b)
    m_locations.push_back(new FootprintLocation(h, -1, m_footprints[static_cast<int>(FootprintType::FixupHorizontal)]));
    m_locations.push_back(new FootprintLocation(m, -1, m_footprints[static_cast<int>(FootprintType::FixupHorizontal)]));
    m_locations.push_back(new FootprintLocation(-1, m, m_footprints[static_cast<int>(FootprintType::FixupVertical)]));
    m_locations.push_back(new FootprintLocation(-1, h, m_footprints[static_cast<int>(FootprintType::FixupVertical)]));

    // Interior trims (l, r, t, b)
    m_locations.push_back(new FootprintLocation(-m, -m, m_footprints[static_cast<int>(FootprintType::InteriorTrimVertical)]));
    m_locations.push_back(new FootprintLocation(m - 1, -m, m_footprints[static_cast<int>(FootprintType::InteriorTrimVertical)]));
    m_locations.push_back(new FootprintLocation(-m, m - 1, m_footprints[static_cast<int>(FootprintType::InteriorTrimHorizontal)]));
    m_locations.push_back(new FootprintLocation(-m, -m, m_footprints[static_cast<int>(FootprintType::InteriorTrimHorizontal)]));

    // Outer degenerated ring
    m_locations.push_back(new FootprintLocation(h, h, m_footprints[static_cast<int>(FootprintType::OuterDegenerateRing)]));
  }

  void Terrain::generateClipmaps()
  {
    ClipmapLevel *parent = nullptr;
    for (int l = CLIPMAP_L - 1; l >= 0; l--)
    {
      m_clipmaps[l] = new ClipmapLevel(l, m_heightmap, parent);
      parent = m_clipmaps[l];
    }
  }

} // end namespace terraindeformer