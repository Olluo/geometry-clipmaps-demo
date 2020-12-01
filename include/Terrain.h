#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "Clipmap.h"
#include "Footprint.h"

#include <ngl/Vec2.h>
#include <ngl/Vec3.h>

namespace terraindeformer
{
  class Terrain
  {
  private:
    // The X width of the terrain
    size_t m_widthX;
    // The Z width of the terrain
    size_t m_widthZ;
    // Used for the height texture
    std::vector<float> m_yValues;
    // Used for colour lookup
    std::vector<ngl::Vec3> m_colours;
    // The list of all clipmap levels
    std::vector<Clipmap *> m_clipmaps;
    // The list of all footprints
    std::vector<Footprint *> m_footprints;
    // The list of all the locations
    std::vector<FootprintLocation *> m_locations;

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

  public:
    /**
     * 
     * @brief Construct a new Terrain object with a width X and width Z
     * 
     * @param _widthX The width in X of the Terrain object
     * @param _widthZ The width in Z of the Terrain object
     */
    Terrain(size_t _widthX, size_t _widthZ);
    /**
     * 
     * @brief Construct a new Terrain object with a height map
     * 
     * @param _hmWidth The width of the height map (also the width of the Terrain object)
     * @param _hmHeight The height of the height map (also the width of the Terrain object)
     * @param _heightMap The height map ot initialise the Terrain object with
     */
    Terrain(size_t _hmWidth, size_t _hmHeight, std::vector<ngl::Vec3> _heightMap);
    /**
     * @brief Load a height map into the terrain, size of heightmap must not be greater than size of terrain
     * 
     * @param _hmWidth The width of the height map
     * @param _hmHeight The height of the height map
     * @param _heightMap The height map ot initialise the Terrain object with
     * @return true The height map was loaded with no errors
     * @return false Error loading height map
     */
    bool Terrain::loadHeightMap(size_t _hmWidth, size_t _hmHeight, std::vector<ngl::Vec3> _heightMap);
    /**
     * @brief Returns the width in X of the terrain
     * 
     * @return size_t 
     */
    size_t widthX() const;
    /**
     * @brief Returns the width in Z of the terrain
     * 
     * @return size_t 
     */
    size_t widthZ() const;
    /**
     * @brief Returns the height of the terrain at the indexed point _x, _z
     * 
     * @param _x The x index of the point
     * @param _z The z index of the point
     * @return float 
     */
    float height(int _x, int _z) const;
    /**
     * @brief Returns the colour of the terrain at the indexed point _x, _z
     * 
     * @param _x The x index of the point
     * @param _z The z index of the point
     * @return ngl::Vec3 
     */
    ngl::Vec3 colour(int _x, int _z) const;

    /**
     * @brief Set the height of the terrain at the indexed point _x, _z
     * 
     * @param _x The x index of the point
     * @param _z The z index of the point
     * @param _height The new height of the terrain
     * @return true If the height was set successfully
     * @return false If the height wasn't set successfully
     */
    bool setHeight(int _x, int _z, float _height);
    /**
     * @brief Reset the height of the terrain at the indexed point _x, _z to 0.0
     * 
     * @param _x The x index of the point
     * @param _z The z index of the point
     * @return true If the height was set successfully
     * @return false If the height wasn't set successfully
     */
    bool resetHeight(int _x, int _z);
    /**
     * @brief Set the colour of the terrain at the indexed point _x, _z
     * 
     * @param _x The x index of the point
     * @param _z The z index of the point
     * @param _colour The new colour of the terrain
     * @return true If the colour was set successfully
     * @return false If the colour wasn't set successfully
     */
    bool setColour(int _x, int _z, ngl::Vec3 _colour);
    /**
     * @brief Reset the colour of the terrain at the indexed point _x, _z to 0.9
     * 
     * @param _x The x index of the point
     * @param _z The z index of the point
     * @return true If the colour was set successfully
     * @return false If the colour wasn't set successfully
     */
    bool resetColour(int _x, int _z);
    // std::vector<float> *yValues();
    // std::vector<ngl::Vec3> *colours();
    // const std::vector<ngl::Vec2> &fpVertices(FootprintType _footprint) const;
    // const std::vector<GLuint> &fpIndices(FootprintType _footprint) const;
    /**
     * @brief Return a vector of all the clipmaps that have been generated
     * 
     * @return std::vector<Clipmap *>& 
     */
    std::vector<Clipmap *> &clipmaps();
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
    std::vector<FootprintLocation *> selectLocations(int _selection);
    /**
     * @brief Initialise the terrain object and generate all sub-parts
     * 
     */
    void initialize();
  };

} // end namespace terraindeformer
#endif // !TERRAIN_H_