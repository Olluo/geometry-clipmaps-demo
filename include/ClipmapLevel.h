/**
 * @file ClipmapLevel.h
 * @author s5222743
 * @brief This class implements a level of the geoclipmap
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CLIPMAP_LEVEL_H_
#define CLIPMAP_LEVEL_H_

#include <ngl/Vec2.h>

#include "Heightmap.h"

namespace terraindeformer
{
  class ClipmapLevel
  {
  private:
    // The level of the clipmap
    int m_level;
    // The scale of the clipmap
    int m_scale;
    // The heightmap
    Heightmap* m_heightmap;
    // The texture for the ClipmapLevel - used for height data
    std::vector<ngl::Real> m_texture;
    // The texture buffer
    GLuint m_textureName;
    // The parent ClipmapLevel (coarser detail) used to get pixel from texture (caching)
    ClipmapLevel *m_parent;
    // The position of this ClipmapLevel
    ngl::Vec2 m_position;

    /**
     * @brief Generate a pixel at location based on parent texture and heightmap
     * 
     * @param _x The x location of the pixel
     * @param _y The y location of the pixel
     * @return float The height at pixel (_x, _y)
     */
    ngl::Real generatePixelAt(ngl::Real _x, ngl::Real _y) noexcept;

  public:
    /**
     * @brief Construct a new ClipmapLevel object
     * 
     * @param _level The level of detail of this clipmap
     * @param _heightmap The heightmap
     * @param _parent The parent ClipmapLevel (coarser detail) used to get pixel from texture (caching)
     */
    ClipmapLevel(int _level, Heightmap *_heightmap, ClipmapLevel *_parent) noexcept;
    /**
     * @brief Set the position of the clipmap. This will recalculate the texture.
     * 
     * @param _position 
     */
    void setPosition(ngl::Vec2 _position) noexcept;
    /**
     * @brief Return const reference to the texture for this clipmap
     * 
     * @return const std::vector<ngl::Real>& the texture of this clipmap
     */
    const std::vector<ngl::Real> &texture() const noexcept;
    /**
     * @brief return pointer to the textureName, to be bound to a specific texture buffer
     * 
     * @return GLuint& the textureName buffer
     */
    GLuint &textureName() noexcept;
    /**
     * @brief Get the scale of this clipmap
     * 
     * @return int 
     */
    int scale() const noexcept;
    /**
     * @brief Get the position of this clipmap
     * 
     * @return const ngl::Vec2& 
     */
    const ngl::Vec2 &position() const noexcept;
    bool left() const noexcept;
    bool bottom() const noexcept;
  };

} // end namespace terraindeformer
#endif // !CLIPMAP_LEVEL_H_