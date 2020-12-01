#include "Clipmap.h"
#include "Constants.h"

namespace terraindeformer
{

  float Clipmap::generatePixelAt(int _x, int _z)
  {
    // for now just return the pixel in the heightmap
    // in future we will want to get an adjusted height that is between the coarse
    // and fine level to avoid holes in the terrain
    return m_heightMap[_z * m_hmWidthX + _x];
  }

  Clipmap::Clipmap(int _level,
                   size_t _hmWidthX,
                   size_t _hmWidthZ,
                   std::vector<float> &_heightMap,
                   Clipmap *_parent) : m_level{_level},
                                       m_hmWidthX{_hmWidthX},
                                       m_hmWidthZ{_hmWidthZ},
                                       m_heightMap{_heightMap},
                                       m_parent{_parent},
                                       m_texture(CLIPMAP_D * CLIPMAP_D),
                                       m_scale{1 << m_level}
  {
  }

  void Clipmap::setPosition(ngl::Vec2 _position)
  {
    m_position = _position;

    // TODO: need to get correct heights from correct position on heightmap instead of just getting the same for every clipmap
    for (int z = 0; z < m_hmWidthZ; z++)
    {
      for (int x = 0; x < m_hmWidthX; x++)
      {
        int xPos = x;
        int zPos = z;
        m_texture[z * CLIPMAP_N + x] = generatePixelAt(xPos, zPos);
      }
    }
    // // pixel.x
    // int xp = 15;
    // // pixel.y
    // int yp = 0;

    // // midpoint of clipmap
    // auto d2 = (CLIPMAP_SIZE + 1) / 2;
    // // clipmap position.x
    // int x = 15;
    // // clipmap position.y
    // int y = 0;
    // // (2x + xp) AND NOT CLIPMAP_SIZE
    // // (2x + xp) AND NOT 1111
    // // (2x + xp) AND 0000
    // int cx = (x * 2 + xp) & ~CLIPMAP_SIZE;
    // int cy = (y * 2 + yp) & ~CLIPMAP_SIZE;
    // int xr = xp - cx - d2;
    // int yr = yp - cy - d2;
    // int xw = xr << m_level;
    // int yw = yr << m_level;

    // xw %= 64;
    // yw %= 64;
    // if (x < 0)
    //   xw += m_hmWidthX;
    // if (y < 0)
    //   yw += m_hmWidthZ;

    // auto pixel = generatePixelAt(x, y);
    // auto blah = 1;
  }

  const std::vector<float> &Clipmap::texture() const
  {
    return m_texture;
  }

  GLuint &Clipmap::textureName()
  {
    return m_textureName;
  }

  int Clipmap::scale() const
  {
    return m_scale;
  }

  const ngl::Vec2 &Clipmap::position() const
  {
    return m_position;
  }

} // end namespace terraindeformer