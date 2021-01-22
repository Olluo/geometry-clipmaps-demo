/**
 * @file Terrain.h
 * @author Ollie Nicholls
 * @brief This class is resposible for managing the Geoclipmap
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <ngl/Vec2.h>
#include <ngl/Vec3.h>

#include "ClipmapLevel.h"
#include "Footprint.h"
#include "Heightmap.h"

namespace geoclipmap
{
  class Terrain
  {
  public:
    /**
     * 
     * @brief Construct a new Terrain object with a height map
     * 
     * @param _heightmap The height map to initialise the Terrain object with
     */
    Terrain(Heightmap *_heightmap) noexcept;
    /**
     * @brief Return a vector of all the clipmaps that have been generated
     * 
     * @return std::vector<Clipmap *>& 
     */
    std::vector<ClipmapLevel *> &clipmaps() noexcept;
    /**
     * @brief Return a vector of all the footprints
     * 
     * @return std::vector<Footprint *>& 
     */
    std::vector<Footprint *> &footprints() noexcept;
    /**
     * @brief Return a selection of footprint locations based on the selection 
     * parameter
     * 
     * @param _selection The footprints required
     * @return std::vector<FootprintLocation *> 
     */
    std::vector<FootprintLocation *> selectLocations(TrimLocation _trimLocation) noexcept;
    /**
     * @brief Initialise the terrain object and generate all sub-parts
     * 
     */
    void initialize() noexcept;
    /**
     * @brief Move the terrain in X and Y and then recompute the terrain at that
     * position
     * 
     * @param _x The amount to move in X
     * @param _y The amount to move in Y
     */
    void move(float _x, float _y) noexcept;
    /**
     * @brief Set the number of active LoD levels using the height of the camera
     * 
     * @param _camHeight The height of the camera
     */
    void setActiveLevels(ngl::Real _camHeight);
    /**
     * @brief Get the active coarsest LoD level
     * 
     * @return unsigned char the active coarsest LoD level
     */
    unsigned char activeCoarsest()
    {
      return m_activeCoarsest;
    }
    /**
     * @brief Get the active finest LoD level
     * 
     * @return unsigned char the active finest LoD level
     */
    unsigned char activeFinest()
    {
      return m_activeFinest;
    }

  private:
    // The heightmap to get height data from 
    Heightmap *m_heightmap;
    // The list of all clipmap levels
    std::vector<ClipmapLevel *> m_clipmaps;
    // The list of all footprints
    std::vector<Footprint *> m_footprints;
    // The list of all the locations
    std::vector<FootprintLocation *> m_locations;
    // The position of the terrain
    ngl::Vec2 m_position;
    // The previous position of the terrain
    ngl::Vec2 m_prevPosition;
    // The active coarsest LoD level
    unsigned char m_activeCoarsest;
    // The active finest LoD level
    unsigned char m_activeFinest;
    // The previous active coarsest LoD level
    unsigned char m_prevActiveCoarsest;
    // The previous active finest LoD level
    unsigned char m_prevActiveFinest;

    /**
     * @brief Generate the set of footprints
     * 
     */
    void generateFootprints() noexcept;
    /**
     * @brief Generate all the possible footprint locations
     * 
     */
    void generateLocations() noexcept;
    /**
     * @brief Generate a clipmap for each level of detail
     * 
     */
    void generateClipmaps() noexcept;
    /**
     * @brief Called when the terrain has moved and used to update all the 
     * clipmap levels
     * 
     */
    void updatePosition() noexcept;

#ifdef TERRAIN_TESTING
#include <gtest/gtest.h>
    FRIEND_TEST(TerrainTest, ctor);
#endif
  };

} // end namespace geoclipmap
#endif // !TERRAIN_H_