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
  Terrain::Terrain(Heightmap *_heightmap) noexcept : m_heightmap{_heightmap},
                                                     m_footprints(6),
                                                     m_clipmaps(CLIPMAP_L),
                                                     m_position{},
                                                     m_activeMin{0},
                                                     m_activeMax{CLIPMAP_L - 1}
  {
    generateFootprints();
    generateLocations();
    generateClipmaps();
    updatePosition();
  }

  std::vector<ClipmapLevel *> &Terrain::clipmaps() noexcept
  {
    return m_clipmaps;
  }

  std::vector<Footprint *> &Terrain::footprints() noexcept
  {
    return m_footprints;
  }

  std::vector<FootprintLocation *> Terrain::selectLocations(TrimLocation _trimLocation) noexcept
  {
    std::vector<int> selectionIndices;
    // These hard-coded values return the correct footprints based on the trim location
    // TODO: see if better way to get these values
    switch (_trimLocation)
    {
    case TrimLocation::All:
      selectionIndices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
      break;
    case TrimLocation::TopLeft:
      selectionIndices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19, 20, 21, 24};
      break;
    case TrimLocation::TopRight:
      selectionIndices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19, 20, 22, 24};
      break;
    case TrimLocation::BottomLeft:
      selectionIndices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19, 21, 23, 24};
      break;
    case TrimLocation::BottomRight:
      selectionIndices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19, 22, 23, 24};
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

  void Terrain::move(float _x, float _y) noexcept
  {
    m_position.m_x += _x;
    m_position.m_y += _y;
    updatePosition();
  }

  // ======================================= Private methods =======================================

  void Terrain::generateFootprints() noexcept
  {
    // Generate all the footprint types each clipmap level will use/reuse
    // Only need one of each type as they are reused and this reduces the number of vertices bound to the VBO/VAO
    m_footprints[static_cast<int>(FootprintType::Block)] = new Footprint(CLIPMAP_M, CLIPMAP_M);
    m_footprints[static_cast<int>(FootprintType::FixupHorizontal)] = new Footprint(CLIPMAP_M, 3);
    m_footprints[static_cast<int>(FootprintType::FixupVertical)] = new Footprint(3, CLIPMAP_M);
    m_footprints[static_cast<int>(FootprintType::InteriorTrimHorizontal)] = new Footprint((2 * CLIPMAP_M) + 1, 2);
    m_footprints[static_cast<int>(FootprintType::InteriorTrimVertical)] = new Footprint(2, (2 * CLIPMAP_M) + 1);
    m_footprints[static_cast<int>(FootprintType::OuterDegenerateRing)] = new Footprint((4 * CLIPMAP_M) - 1);
  }

  void Terrain::generateLocations() noexcept
  {
    // Footprints are placed based on their offset inside the local coordinates of the clipmap level
    // 0,0 is the bottom left corner of the clipmap level and each footprint is offset from there
    // The m_position is the location of the clipmap level's bottom left corner in world space
    // The centre of the clipmap level can be found by adding half CLIPMAP_D

    // This is used for the bottom left corner displacement of each footprint
    // 0, 0 is the local coordinate centre of the clipmap level
    // The m_position of each clipmap level is their world coordinate
    int m = static_cast<int>(CLIPMAP_M) - 1;

    // See https://developer.nvidia.com/sites/all/modules/custom/gpugems/books/GPUGems2/elementLinks/02_clipmaps_05.jpg
    // B1 - B4
    m_locations.push_back(new FootprintLocation(0, 3 * m + 2, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(m, 3 * m + 2, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(2 * m + 2, 3 * m + 2, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(3 * m + 2, 3 * m + 2, m_footprints[static_cast<int>(FootprintType::Block)]));

    // B5 & B6
    m_locations.push_back(new FootprintLocation(0, 2 * m + 2, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(3 * m + 2, 2 * m + 2, m_footprints[static_cast<int>(FootprintType::Block)]));

    // B7 & B8
    m_locations.push_back(new FootprintLocation(0, m, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(3 * m + 2, m, m_footprints[static_cast<int>(FootprintType::Block)]));

    // B9 - B12
    m_locations.push_back(new FootprintLocation(0, 0, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(m, 0, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(2 * m + 2, 0, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(3 * m + 2, 0, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Inner blocks (only used in finest level)
    m_locations.push_back(new FootprintLocation(m + 1, 2 * m + 1, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(2 * m + 1, 2 * m + 1, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(m + 1, m + 1, m_footprints[static_cast<int>(FootprintType::Block)]));
    m_locations.push_back(new FootprintLocation(2 * m + 1, m + 1, m_footprints[static_cast<int>(FootprintType::Block)]));

    // Fix-up footprints (order: top, left, right, bottom)
    m_locations.push_back(new FootprintLocation(2 * m, 3 * m + 2, m_footprints[static_cast<int>(FootprintType::FixupVertical)]));
    m_locations.push_back(new FootprintLocation(0, 2 * m, m_footprints[static_cast<int>(FootprintType::FixupHorizontal)]));
    m_locations.push_back(new FootprintLocation(3 * m + 2, 2 * m, m_footprints[static_cast<int>(FootprintType::FixupHorizontal)]));
    m_locations.push_back(new FootprintLocation(2 * m, 0, m_footprints[static_cast<int>(FootprintType::FixupVertical)]));

    // Interior trims (order: top, left, right, bottom)
    m_locations.push_back(new FootprintLocation(m, 3 * m + 1, m_footprints[static_cast<int>(FootprintType::InteriorTrimHorizontal)]));
    m_locations.push_back(new FootprintLocation(m, m, m_footprints[static_cast<int>(FootprintType::InteriorTrimVertical)]));
    m_locations.push_back(new FootprintLocation(3 * m + 1, m, m_footprints[static_cast<int>(FootprintType::InteriorTrimVertical)]));
    m_locations.push_back(new FootprintLocation(m, m, m_footprints[static_cast<int>(FootprintType::InteriorTrimHorizontal)]));

    // Outer degenerated ring
    m_locations.push_back(new FootprintLocation(0, 0, m_footprints[static_cast<int>(FootprintType::OuterDegenerateRing)]));
  }

  void Terrain::generateClipmaps() noexcept
  {
    ClipmapLevel *parent = nullptr;
    // Generate clipmaps from coarsest to finest as finer clipmaps need a reference to the coarser one
    for (int l = 0; l < CLIPMAP_L; l++)
    {
      m_clipmaps[l] = new ClipmapLevel(l, m_heightmap, parent);
      parent = m_clipmaps[l];
    }
  }

  void Terrain::updatePosition() noexcept
  {
    // The terrain is always positioned at the camera X,Z coordinate
    // Each clipmap level is then at a position based on their scale and an offset
    auto position = m_position;

    // This is the offset of half of the clipmap width so that the finest level is positioned in the centre 
    // then all other clipmaps are offset from that
    ngl::Vec2 clipmapCentreOffset(static_cast<ngl::Real>(CLIPMAP_D2), static_cast<ngl::Real>(CLIPMAP_D2));
    // All the positions are based off the initial position which starts at the clipmap offset
    ngl::Vec2 previousWorldPosition = -clipmapCentreOffset;
    // This offset is used to offset subsequent levels as all levels are at least 1 block (mxm) offset from the
    // finer level within their centre
    ngl::Real trimOffset = static_cast<ngl::Real>(CLIPMAP_M - 1);

    for (int l = static_cast<int>(CLIPMAP_L - 1); l >= 0; l--)
    {
      auto currentLevel = m_clipmaps[l];

      // The position of this level in world space (not scaled as this is done in the shader)
      ngl::Vec2 newWorldPosition = previousWorldPosition;

      // Round the values to integers to get rid of the decimal part
      int xPos = static_cast<int>(position.m_x);
      int yPos = static_cast<int>(position.m_y);
      int scale = currentLevel->scale();

      TrimLocation trimLocation;

      // Always show all trims on finest level
      if (l == static_cast<int>(CLIPMAP_L - 1))
      {
        trimLocation = TrimLocation::All;
      }
      else
      {
        // These if statements work by comparing the result of the position logical and the scale to determine
        // the position of the trim and the offset of this clipmap level. As the scales are all powers of 2
        // the bit of the scale can be used with the position to determine where this level should be.
        // e.g. scale = 4 == 0100, xPos = 5 == 0101, xPos & scale = 0100 > 0 therefore clipmap on the left.
        if ((xPos & scale) > 0)
        {
          if ((yPos & scale) > 0)
          {
            trimLocation = TrimLocation::BottomLeft;
            newWorldPosition -= ngl::Vec2(trimOffset + 1, trimOffset + 1);
          }
          else
          {
            trimLocation = TrimLocation::TopLeft;
            newWorldPosition -= ngl::Vec2(trimOffset + 1, trimOffset);
          }
        }
        else
        {
          if ((yPos & scale) > 0)
          {
            trimLocation = TrimLocation::BottomRight;
            newWorldPosition -= ngl::Vec2(trimOffset, trimOffset + 1);
          }
          else
          {
            trimLocation = TrimLocation::TopRight;
            newWorldPosition -= ngl::Vec2(trimOffset, trimOffset);
          }
        }
      }

      // The position of this level in heightmap space
      ngl::Vec2 newHeightmapPosition = newWorldPosition + (position / static_cast<ngl::Real>(scale));

      currentLevel->setPosition(newWorldPosition, newHeightmapPosition, trimLocation);
      // Divide the position by 2 as each subsequent level is scaled with powers of 2
      previousWorldPosition = newWorldPosition / 2.0f;
    }
  }

} // end namespace terraindeformer