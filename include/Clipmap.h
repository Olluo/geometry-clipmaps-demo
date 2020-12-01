#ifndef CLIPMAP_H_
#define CLIPMAP_H_

#include <ngl/Vec2.h>

namespace terraindeformer
{
  class Clipmap
  {
  private:
    // The level of detail of the Clipmap
    int m_level;
    // The scale of the clipmap
    int m_scale;
    // The X width of the height map
    size_t m_hmWidthX;
    // The Z width of the height map
    size_t m_hmWidthZ;
    // The heightmap
    std::vector<float> m_heightMap;
    // The texture for the Clipmap - used for height data
    std::vector<float> m_texture;
    // The texture buffer
    GLuint m_textureName;
    // The parent Clipmap (coarser detail) used to get pixel from texture (caching)
    Clipmap *m_parent;
    // The position of this Clipmap
    ngl::Vec2 m_position;
    /**
     * @brief Generate a pixel at location based on parent texture and heightmap
     * 
     * @param _x The x location of the pixel
     * @param _z The z location of the pixel
     * @return float The height at pixel (_x, _z)
     */
    float generatePixelAt(int _x, int _z);

  public:
    /**
     * @brief Construct a new Clipmap object
     * 
     * @param _level The level of detail of this clipmap
     * @param _hmWidthX The width in X of the height map
     * @param _hmWidthZ The width in Z of the height map
     * @param _heightMap The heightmap
     * @param _parent The parent Clipmap (coarser detail) used to get pixel from texture (caching)
     */
    Clipmap(int _level, size_t _hmWidthX, size_t _hmWidthZ, std::vector<float> &_heightMap, Clipmap *_parent);
    /**
     * @brief Set the position of the clipmap. This will recalculate the texture.
     * 
     * @param _position 
     */
    void setPosition(ngl::Vec2 _position);
    /**
     * @brief Return const reference to the texture for this clipmap
     * 
     * @return const std::vector<float>& the texture of this clipmap
     */
    const std::vector<float> &texture() const;
    /**
     * @brief return pointer to the textureName, to be bound to a specific texture buffer
     * 
     * @return GLuint& the textureName buffer
     */
    GLuint &textureName();
    /**
     * @brief Get the scale of this clipmap
     * 
     * @return int 
     */
    int scale() const;
    /**
     * @brief Get the position of this clipmap
     * 
     * @return const ngl::Vec2& 
     */
    const ngl::Vec2 &position() const;
  };

} // end namespace terraindeformer
#endif // !CLIPMAP_H_