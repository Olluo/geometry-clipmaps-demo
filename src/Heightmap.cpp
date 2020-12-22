#include "Heightmap.h"

namespace terraindeformer
{

  Heightmap::Heightmap(ngl::Real _width, ngl::Real _height, std::vector<ngl::Vec3> _data) noexcept : m_width{_width},
                                                                                                     m_depth{_height},
                                                                                                     m_data{_data}
  {
  }

  ngl::Real Heightmap::width() noexcept
  {
    return m_width;
  }

  ngl::Real Heightmap::depth() noexcept
  {
    return m_depth;
  }

  ngl::Real Heightmap::value(int _x, int _y) noexcept
  {
    // Just return red value for now
    return colour(_x, _y).m_r;
  }

  ngl::Vec3 Heightmap::colour(int _x, int _y) noexcept
  {
    if (_x < 0 || _x > m_width - 1)
    {
      return ngl::Vec3();
    }

    if (_y < 0 || _y > m_depth - 1)
    {
      return ngl::Vec3();
    }

    return m_data[_y * static_cast<int>(m_width) + _x];
  }

} // end namespace terraindeformer