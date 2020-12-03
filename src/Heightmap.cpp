#include "Heightmap.h"

namespace terraindeformer
{

  Heightmap::Heightmap(ngl::Real _width, ngl::Real _height, std::vector<ngl::Vec3> _data) : m_width{_width},
                                                                                            m_depth{_height},
                                                                                            m_data{_data}
  {
  }

  ngl::Real Heightmap::width()
  {
    return m_width;
  }

  ngl::Real Heightmap::depth()
  {
    return m_depth;
  }

  ngl::Real Heightmap::value(ngl::Real _x, ngl::Real _y)
  {
    // Make the values within the range of the heightmap
    int x = static_cast<int>(_x) % static_cast<int>(m_width);
    int y = static_cast<int>(_y) % static_cast<int>(m_depth);

    // If the values are negative now, wrap back to positive
    if (x < 0)
    {
      x *= -1;
    }
    if (y < 0)
    {
      y *= -1;
    }

    // Just return red value for now
    return m_data[y * m_width + x].m_r;
  }

  ngl::Vec3 Heightmap::colour(ngl::Real _x, ngl::Real _y)
  {
    // Make the values within the range of the heightmap
    int x = static_cast<int>(_x) % static_cast<int>(m_width);
    int y = static_cast<int>(_y) % static_cast<int>(m_depth);

    // If the values are negative now, wrap back to positive
    if (x < 0)
    {
      x *= -1;
    }
    if (y < 0)
    {
      y *= -1;
    }
    
    return m_data[_y * m_width + _x];
  }

} // end namespace terraindeformer