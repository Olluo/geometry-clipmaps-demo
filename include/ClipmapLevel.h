/**
 * @file ClipmapLevel.h
 * @author Ollie Nicholls
 * @brief This class implements a level of the geoclipmap
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CLIPMAP_LEVEL_H_
#define CLIPMAP_LEVEL_H_

#include <ngl/Vec2.h>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>

#include "Heightmap.h"

namespace geoclipmap
{

  enum class TrimLocation
  {
    All,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
  };
  class ClipmapLevel
  {

  public:
    /**
     * @brief Construct a new ClipmapLevel object
     * 
     * @param _level The level of detail of this clipmap
     * @param _heightmap The heightmap
     * @param _parent The parent ClipmapLevel (coarser detail) used to get pixel from texture (caching)
     */
    ClipmapLevel(int _level, Heightmap *_heightmap, ClipmapLevel *_parent, TrimLocation _trimLocation = TrimLocation::TopRight) noexcept;
    /**
     * @brief Set the position of the clipmap. This will recalculate the texture. TODO
     * 
     * @param _position 
     */
    void setPosition(ngl::Vec2 _worldPosition, ngl::Vec2 _heightmapPosition, TrimLocation _trimLocation) noexcept;
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
    TrimLocation trimLocation() const noexcept;
    void bindTextures() noexcept;

  private:
    // The level of the clipmap
    int m_level;
    // The scale of the clipmap
    int m_scale;
    // The heightmap
    Heightmap *m_heightmap;
    // The texture for the ClipmapLevel - used for height data
    std::vector<ngl::Vec4> m_texture;
    // The texture buffer
    GLuint m_textureName;
    // The parent ClipmapLevel (coarser detail) used to get pixel from texture (caching)
    ClipmapLevel *m_parent;
    // The position of this ClipmapLevel
    ngl::Vec2 m_worldPosition;
    ngl::Vec2 m_heightmapPosition;
    TrimLocation m_trimLocation;
    bool m_allocated = false;

    /**
     * @brief Generate a pixel at location based on parent texture and heightmap
     * 
     * @param _x The x location of the pixel
     * @param _y The y location of the pixel
     * @return ngl::Vec2 The height at pixel (_x, _y) where m_x is the fine pixel, m_y is the coarse pixel
     */
    ngl::Vec4 generatePixelAt(int _x, int _y) noexcept;

#ifdef TERRAIN_TESTING
#include <gtest/gtest.h>
    FRIEND_TEST(ClipmapTest, ctor);
    FRIEND_TEST(ClipmapTest, setPosition);
#endif
  };

} // end namespace geoclipmap
#endif // !CLIPMAP_LEVEL_H_