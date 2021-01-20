/**
 * @file FootprintVAO.cpp
 * @author Ollie Nicholls
 * @brief An implementation of the AbstractVAO. 
 * Only made this as couldn't seem to get SimpleIndexVAO working even though 
 * this is pretty much the same implementation.
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <iostream>

#include "FootprintVAO.h"

namespace geoclipmap
{

  void FootprintVAO::draw() const
  {
    if (m_allocated == false)
    {
      std::cerr << "Warning trying to draw unallocated Footprint VAO\n";
    }

    if (m_bound == false)
    {
      std::cerr << "Warning trying to draw unbound Footprint VAO\n";
    }

    glDrawElements(m_mode, static_cast<GLsizei>(m_indicesCount), GL_UNSIGNED_INT, static_cast<ngl::Real *>(nullptr));
  }

  void FootprintVAO::removeVAO()
  {
    if (m_bound == true)
    {
      unbind();
    }

    if (m_allocated == true)
    {
      glDeleteBuffers(1, &m_buffer);
    }

    glDeleteVertexArrays(1, &m_id);
    m_allocated = false;
  }

  void FootprintVAO::setData(const VertexData &_data)
  {
    if (m_bound == false)
    {
      std::cerr << "Warning trying to set vertex data on unbound Footprint VAO\n";
    }

    GLuint vboID;
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_data.m_size), &_data.m_data, _data.m_mode);

    m_allocated = true;
  }

  void FootprintVAO::setIndexData(const IndexData &_data, size_t _indexCount)
  {
    if (m_bound == false)
    {
      std::cerr << "Warning trying to set vertex data on unbound Footprint VAO\n";
    }

    m_indicesCount = _indexCount;

    GLuint iboID;
    glGenBuffers(1, &iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(_data.m_size), &_data.m_data, _data.m_mode);
  }
} // end namespace geoclipmap