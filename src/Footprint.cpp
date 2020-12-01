#include <iostream>

#include "Constants.h"
#include "Footprint.h"

namespace terraindeformer
{
  void Footprint::calculate2DVertices()
  {
    for (size_t z = 0; z < m_widthZ; z++)
    {
      for (size_t x = 0; x < m_widthX; x++)
      {
        m_vertices.push_back(ngl::Vec2(static_cast<ngl::Real>(x), static_cast<ngl::Real>(z)));
      }
    }
  }

  void Footprint::calculate2DVerticesDegenerate()
  {
    // Bottom
    for (int x = 0; x < m_widthX; x++)
    {
      m_vertices.push_back(ngl::Vec2(static_cast<ngl::Real>(x), 0));
    }

    // Right
    for (int z = 1; z < m_widthX; z++)
    {
      m_vertices.push_back(ngl::Vec2(static_cast<ngl::Real>(m_widthX - 1), static_cast<ngl::Real>(z)));
    }

    // Top
    for (int x = static_cast<int>(m_widthX - 2); x >= 0; x--)
    {
      m_vertices.push_back(ngl::Vec2(static_cast<ngl::Real>(x), static_cast<ngl::Real>(m_widthX - 1)));
    }

    // Left
    for (int z = static_cast<int>(m_widthX - 2); z > 0; z--)
    {
      m_vertices.push_back(ngl::Vec2(0, static_cast<ngl::Real>(z)));
    }
  }

  void Footprint::calculateIndices()
  {
    for (int z = 0; z < m_widthZ - 1; z++)
    {
      // Happens on all but first row
      if (z > 0)
      {
        // Degenerate triangle begin: repeat first vertex
        m_indices.push_back(static_cast<GLuint>(z * m_widthX));
      }

      // Happens on all rows
      for (int x = 0; x < m_widthX; x++)
      {
        // One part of the triangle strip
        m_indices.push_back(static_cast<GLuint>((z * m_widthX) + x));
        m_indices.push_back(static_cast<GLuint>(((z + 1) * m_widthX) + x));
      }

      // Happens on all but last 2 rows
      if (z < m_widthZ - 2)
      {
        // Degenerate triangle end: repeat last vertex
        m_indices.push_back(static_cast<GLuint>(((z + 1) * m_widthX) + (m_widthX - 1)));
      }
    }
  }

  void Footprint::calculateIndicesDegenerate()
  {
    // Indices
    size_t nVertices = m_vertices.size();
    for (int i = 0; i < nVertices; i++)
    {
      m_indices.push_back(i);
      // If i is a corner add it again
      if (i != 0 && i % (m_widthX - 1) == 0)
      {
        m_indices.push_back(i);
      }
    }
    // Add the first vertex again so it creates a full loop
    m_indices.push_back(0);
  }

  Footprint::Footprint(size_t _widthX, size_t _widthZ) : m_widthX{_widthX},
                                                         m_widthZ{_widthZ}
  {
    calculate2DVertices();
    m_vertexCount = m_vertices.size();
    calculateIndices();
    m_indexCount = m_indices.size();
  }

  Footprint::Footprint(size_t _width) : m_widthX{_width},
                                        m_widthZ{_width}
  {
    calculate2DVerticesDegenerate();
    m_vertexCount = m_vertices.size();
    calculateIndicesDegenerate();
    m_indexCount = m_indices.size();
  }

  size_t Footprint::widthX() const
  {
    return m_widthX;
  }

  size_t Footprint::widthZ() const
  {
    return m_widthZ;
  }

  const std::vector<ngl::Vec2> &Footprint::vertices() const
  {
    return m_vertices;
  }

  size_t Footprint::vertexCount() const
  {
    return m_vertexCount;
  }

  const std::vector<GLuint> &Footprint::indices() const
  {
    return m_indices;
  }

  size_t Footprint::indexCount() const
  {
    return m_indexCount;
  }

  GLuint &Footprint::vao()
  {
    return m_vao;
  }

  GLuint &Footprint::vbo()
  {
    return m_vbo;
  }

  GLuint &Footprint::ibo()
  {
    return m_ibo;
  }

} // end namespace terraindeformer