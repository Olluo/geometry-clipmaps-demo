#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <ngl/Vec2.h>
#include <ngl/Vec3.h>

namespace terraindeformer
{

  const float DEFAULT_HEIGHT = 0.0f;
  const float DEFAULT_COLOUR = 0.9f;
  const size_t TILE_SIZE = 16;

  class Terrain
  {
  private:
    /**
     * @brief Calculate the adjusted width which is the width when scaled to fit in TILE_SIZE tiles + 1
     * 
     * @param _width The width to scale
     * @return size_t 
     */
    size_t calculateAdjustedWidth(size_t _width) const;
    size_t m_widthX;
    size_t m_widthZ;
    std::vector<float> m_yValues;
    std::vector<ngl::Vec2> m_xzValues;
    std::vector<ngl::Vec3> m_colours;

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
     * @param _heightMap The height map ot initialise the Terrain object with
     * @param _hmWidth The width of the height map (also the width of the Terrain object)
     * @param _hmHeight The height of the height map (also the width of the Terrain object)
     */
    Terrain(size_t _hmWidth, size_t _hmHeight, std::vector<ngl::Vec3> _heightMap);

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
    void print();
  };

} // end namespace terraindeformer
#endif // !TERRAIN_H_