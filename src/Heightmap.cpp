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

  ngl::Real Heightmap::value(ngl::Real _x, ngl::Real _y) noexcept
  {
    // Just return red value for now
    return colour(_x, _y).lengthSquared();
  }

  ngl::Vec3 Heightmap::colour(ngl::Real _x, ngl::Real _y) noexcept
  {
    int x = static_cast<int>(_x);
    int y = static_cast<int>(_y);

    if (x < 0 || x > m_width - 1)
    {
      return ngl::Vec3();
    }

    if (y < 0 || y > m_depth - 1)
    {
      return ngl::Vec3();
    }

    return m_data[y * static_cast<int>(m_width) + x];
  }

} // end namespace terraindeformer