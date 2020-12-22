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

#include "ClipmapLevel.h"
#include "Footprint.h"
#include "Heightmap.h"

#include <ngl/Vec2.h>
#include <ngl/Vec3.h>

namespace terraindeformer
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
     * @brief Return a selection of footprint locations based on the selection parameter
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
    void move(float _x, float _y) noexcept;
    void setActiveMin(size_t _activeMin) {m_activeMin = _activeMin;}
    void setActiveMax(size_t _activeMax) {m_activeMax = _activeMax;}
    size_t activeMin() {return m_activeMin;}
    size_t activeMax() {return m_activeMax;}

  private:
    Heightmap *m_heightmap;
    // The list of all clipmap levels
    std::vector<ClipmapLevel *> m_clipmaps;
    // The list of all footprints
    std::vector<Footprint *> m_footprints;
    // The list of all the locations
    std::vector<FootprintLocation *> m_locations;
    // The position of the terrain
    ngl::Vec2 m_position;
    size_t m_activeMin;
    size_t m_activeMax;

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
    void updatePosition() noexcept;
  };

} // end namespace terraindeformer
#endif // !TERRAIN_H_