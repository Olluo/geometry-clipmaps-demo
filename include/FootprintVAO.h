/**
 * @file FootprintVAO.h
 * @author Ollie Nicholls
 * @brief An implementation of the AbstractVAO. 
 * Only made this as couldn't seem to get SimpleIndexVAO working even though this is pretty much the same implementation.
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef FOOTPRINT_VAO_H_
#define FOOTPRINT_VAO_H_

#include <ngl/AbstractVAO.h>

namespace geoclipmap
{
  class FootprintVAO : public ngl::AbstractVAO
  {
  public:    
    struct IndexData
    {
      IndexData(size_t _size, const GLuint &_data, GLenum _mode = GL_STATIC_DRAW) : m_size(_size),
                                                                                    m_data(_data),
                                                                                    m_mode(_mode)
      {
      }

      size_t m_size;
      const GLuint &m_data;
      GLenum m_mode = GL_STATIC_DRAW;
    };

    /**
     * @brief Get the factory to create the VAO
     * 
     * @param _mode 
     * @return std::unique_ptr<ngl::AbstractVAO> 
     */
    static std::unique_ptr<ngl::AbstractVAO> create(GLenum /*_mode*/)
    {
      return std::unique_ptr<AbstractVAO>(new FootprintVAO());
    }
    /**
     * @brief Draw the data buffered to the VAO
     * 
     */
    virtual void draw() const;
    /**
     * @brief Destroy the Footprint VAO object
     * 
     */
    virtual ~FootprintVAO() = default;
    /**
     * @brief Remove all the bound data and the VAO
     * 
     */
    virtual void removeVAO();
    /**
     * @brief Set the vertex data of the VAO
     * 
     * @param _data The vertex data to be buffered to the VAO
     */
    virtual void setData(const VertexData &_data);
    /**
     * @brief Set the index data of the VAO
     * 
     * @param _data The index data to be buffered to the VAO
     * @param _indexCount The number of indices to be buffered to the VAO
     */
    void setIndexData(const IndexData &_data, size_t _indexCount);
    /**
     * @brief Get the buffer ID
     * 
     * @return GLuint The buffer ID
     */
    GLuint getBufferID(unsigned int /*_index*/) { return m_buffer; }
    /**
     * @brief Just return nullptr as this method is not needed
     */
    ngl::Real *mapBuffer(unsigned int /*_index*/, GLenum /*_accessMode*/) { return nullptr; }

  protected:
    /**
     * @brief Construct a new Footprint VAO object using the parent ctor
     * to bind the VAO
     */
    FootprintVAO() : ngl::AbstractVAO(GL_TRIANGLE_STRIP)
    {
    }

  private:
    // The buffer ID for the VAO
    GLuint m_buffer = 0;
  };

} // end namespace geoclipmap

#endif // !FOOTPRINT_VAO_H_