#include "Footprint.h"
#include "Constants.h"

#include <ngl/VAOFactory.h>

#include <iostream>

namespace terraindeformer
{
  Footprint::Footprint(size_t _width, size_t _depth) noexcept : m_width{_width},
                                                                m_depth{_depth}
  {
    calculate2DVertices();
    m_vertexCount = m_vertices.size();
    calculateIndices();
    m_indexCount = m_indices.size();
  }

  Footprint::Footprint(size_t _width) noexcept : m_width{_width},
                                                 m_depth{_width}
  {
    calculate2DVerticesDegenerate();
    m_vertexCount = m_vertices.size();
    calculateIndicesDegenerate();
    m_indexCount = m_indices.size();
  }

  Footprint::~Footprint() noexcept
  {
    if (!m_vaoBound)
    {
      m_vao->removeVAO();
    }
  }

  void Footprint::draw() noexcept
  {
    if (!m_vaoBound)
    {
      ngl::VAOFactory::registerVAOCreator("footprintVAO", FootprintVAO::create);
      m_vao = ngl::vaoFactoryCast<FootprintVAO>(ngl::VAOFactory::createVAO("footprintVAO", GL_TRIANGLE_STRIP));
      m_vaoBound = true;
    }

    m_vao->bind();

    if (!m_dataBound)
    {
      m_vao->setData(FootprintVAO::VertexData(m_vertices.size() * sizeof(ngl::Vec3), m_vertices[0].m_x));
      m_vao->setVertexAttributePointer(0, 2, GL_FLOAT, 0, 0);
      m_vao->setIndexData(FootprintVAO::IndexData(m_indices.size() * sizeof(GLuint), m_indices[0]), m_indices.size());
      m_dataBound = true;
    }

    m_vao->draw();
    m_vao->unbind();
  }

  // ======================================= Private methods =======================================

  void Footprint::calculate2DVertices() noexcept
  {
    for (size_t y = 0; y < m_depth; y++)
    {
      for (size_t x = 0; x < m_width; x++)
      {
        m_vertices.push_back(ngl::Vec2(static_cast<ngl::Real>(x), static_cast<ngl::Real>(y)));
      }
    }
  }

  void Footprint::calculate2DVerticesDegenerate() noexcept
  {
    // Bottom
    for (int x = 0; x < m_width; x++)
    {
      m_vertices.push_back(ngl::Vec2(static_cast<ngl::Real>(x), 0));
    }

    // Right
    for (int y = 1; y < m_width; y++)
    {
      m_vertices.push_back(ngl::Vec2(static_cast<ngl::Real>(m_width - 1), static_cast<ngl::Real>(y)));
    }

    // Top
    for (int x = static_cast<int>(m_width - 2); x >= 0; x--)
    {
      m_vertices.push_back(ngl::Vec2(static_cast<ngl::Real>(x), static_cast<ngl::Real>(m_width - 1)));
    }

    // Left
    for (int y = static_cast<int>(m_width - 2); y > 0; y--)
    {
      m_vertices.push_back(ngl::Vec2(0, static_cast<ngl::Real>(y)));
    }
  }

  void Footprint::calculateIndices() noexcept
  {
    for (int y = 0; y < m_depth - 1; y++)
    {
      // Happens on all but first row
      if (y > 0)
      {
        // Degenerate triangle begin: repeat first vertex
        m_indices.push_back(static_cast<GLuint>(y * m_width));
      }

      // Happens on all rows
      for (int x = 0; x < m_width; x++)
      {
        // One part of the triangle strip
        m_indices.push_back(static_cast<GLuint>((y * m_width) + x));
        m_indices.push_back(static_cast<GLuint>(((y + 1) * m_width) + x));
      }

      // Happens on all but last 2 rows
      if (y < m_depth - 2)
      {
        // Degenerate triangle end: repeat last vertex
        m_indices.push_back(static_cast<GLuint>(((y + 1) * m_width) + (m_width - 1)));
      }
    }
  }

  void Footprint::calculateIndicesDegenerate() noexcept
  {
    // Indices
    size_t nVertices = m_vertices.size();
    for (int i = 0; i < nVertices; i++)
    {
      m_indices.push_back(i);
      // If i is a corner add it again
      if (i != 0 && i % (m_width - 1) == 0)
      {
        m_indices.push_back(i);
      }
    }
    // Add the first vertex again so it creates a full loop
    m_indices.push_back(0);
  }

} // end namespace terraindeformer