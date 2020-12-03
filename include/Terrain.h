#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "ClipmapLevel.h"
#include "Footprint.h"
#include "Heightmap.h"

#include <ngl/Vec2.h>
#include <ngl/Vec3.h>

namespace terraindeformer
{

  enum class FootprintSelection
  {
    All,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
  };

  class Terrain
  {
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

    /**
     * @brief Generate the set of footprints
     * 
     */
    void generateFootprints();
    /**
     * @brief Generate all the possible footprint locations
     * 
     */
    void generateLocations();
    /**
     * @brief Generate a clipmap for each level of detail
     * 
     */
    void generateClipmaps();
    void updatePosition();

  public:
    /**
     * 
     * @brief Construct a new Terrain object with a height map
     * 
     * @param _heightmap The height map to initialise the Terrain object with
     */
    Terrain(Heightmap *_heightmap);
    /**
     * @brief Return a vector of all the clipmaps that have been generated
     * 
     * @return std::vector<Clipmap *>& 
     */
    std::vector<ClipmapLevel *> &clipmaps();
    /**
     * @brief Return a vector of all the footprints
     * 
     * @return std::vector<Footprint *>& 
     */
    std::vector<Footprint *> &footprints();
    /**
     * @brief Return a selection of footprint locations based on the selection parameter
     * 
     * @param _selection The footprints required
     * @return std::vector<FootprintLocation *> 
     */
    std::vector<FootprintLocation *> selectLocations(FootprintSelection _selection);
    /**
     * @brief Initialise the terrain object and generate all sub-parts
     * 
     */
    void initialize();
    void move(float _x, float _y);
  };

} // end namespace terraindeformer
#endif // !TERRAIN_H_