#ifndef FOOTPRINT_H_
#define FOOTPRINT_H_

#include <ngl/Vec2.h>
#include <ngl/Vec3.h>

namespace terraindeformer
{
  enum class FootprintType
  {
    Block,
    FixupHorizontal,
    FixupVertical,
    InteriorTrimHorizontal,
    InteriorTrimVertical,
    OuterDegenerateRing
  };

  class Footprint
  {
  private:
    // The X width of the Footprint
    size_t m_widthX;
    // The Z width of the Footprint
    size_t m_widthZ;
    // The vertices representing the Footprint
    std::vector<ngl::Vec2> m_vertices;
    // The indices representing the Footprint
    std::vector<GLuint> m_indices;
    // The number of vertices
    size_t m_vertexCount;
    // The number of indices
    size_t m_indexCount;
    // The vertex array for the Footprint
    GLuint m_vao;
    // The vertex buffer for the Footprint
    GLuint m_vbo;
    // The index buffer for the Footprint
    GLuint m_ibo;

    /**
     * @brief Calculate the 2D vertices for the Footprint
     */
    void calculate2DVertices();
    /**
     * @brief Calculate the 2D vertices for a degenerate triangle ring Footprint
     */
    void calculate2DVerticesDegenerate();
    /**
     * @brief Calculate the indices for the Footprint.
     * 
     * See https://www.learnopengles.com/tag/triangle-strips/ which shows Java
     * for how to do this which I adapted to my problem. It also seems their 
     * code is technically wrong as yes it works for square sets but not
     * rectangular as they mixed up which width they used when trying to find 
     * items from the next row, so that is fixed here.
     */
    void calculateIndices();
    /**
     * @brief Calculate the indices for a degenerate triangle ring Footprint
     */
    void calculateIndicesDegenerate();

  public:
    /**
     * @brief Construct a new Footprint object
     * 
     * @param _widthX The width in X of the Footprint
     * @param _widthZ The width in Z of the Footprint
     */
    Footprint(size_t _widthX, size_t _widthZ);
    /**
     * @brief Construct a degenerate triangle ring Footprint
     * 
     * @param _width The width X/Z of the ring
     */
    Footprint(size_t _width);
    /**
     * @brief Get the width in X of the footprint
     * 
     * @return size_t The width in X
     */
    size_t widthX() const;
    /**
     * @brief Get the width in Z of the footprint
     * 
     * @return size_t The width in Z
     */
    size_t widthZ() const;
    /**
     * @brief Get the vertices that represent this footprint
     * 
     * @return const std::vector<ngl::Vec2>& Const reference to the vector of vertices
     */
    const std::vector<ngl::Vec2> &vertices() const;
    /**
     * @brief The number of vertices that represent this footprint
     * 
     * @return size_t 
     */
    size_t vertexCount() const;
    /**
     * @brief Get the indices that represent this footprint
     * 
     * @return const std::vector<GLuint>& Const reference to the vector of vertices
     */
    const std::vector<GLuint> &indices() const;
    /**
     * @brief The number of indices that represent this footprint
     * 
     * @return size_t 
     */
    size_t indexCount() const;
    /**
     * @brief Return pointer to the vao, to be bound to a specific vertex array buffer
     * 
     * @return GLuint& 
     */
    GLuint &vao();
    /**
     * @brief Return pointer to the vbo, to be bound to a specific vertex buffer
     * 
     * @return GLuint& 
     */
    GLuint &vbo();
    /**
     * @brief Return pointer to the ibo, to be bound to a specific index buffer
     * 
     * @return GLuint& 
     */
    GLuint &ibo();
  };

  struct FootprintLocation
  {
    // The position in x
    int x;
    // The position in z
    int z;
    // The footprint at (x, z)
    Footprint *footprint;
    /**
     * @brief Construct a new Footprint Location object that represents where the footprint could be
     * 
     * @param _x The position in x
     * @param _z The position in z
     * @param _footprint A reference to a footprint that will be at this location
     */
    FootprintLocation(int _x, int _z, Footprint *_footprint) : x{_x},
                                                               z{_z},
                                                               footprint{_footprint}
    {
    }
  };
} // end namespace terraindeformer
#endif // !FOOTPRINT_H_