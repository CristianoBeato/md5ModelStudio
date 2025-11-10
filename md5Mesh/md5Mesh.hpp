/*
===============================================================
===============================================================
*/

#ifndef __MD5MESH_HPP__
#define __MD5MESH_HPP__

// Triangle surface
struct md5Triangle
{
  int32_t v1 = 0;
  int32_t v2 = 0;
  int32_t v3 = 0;
};

class md5Vertex
{
public:
    md5Vertex( void );
    md5Vertex( const int32_t in_start, const int32_t in_count, const glm::vec2 in_st );
    ~md5Vertex( void );

    int32_t WeightStart( void ) const { return m_start; }
    int32_t WeightCount( void ) const { return m_count; }
    glm::vec2   TextCoord( void  ) const { return m_st; }

private:
    int32_t         m_start;    // start weight
    int32_t         m_count;    // weight count
    glm::vec2       m_st;       // texture coodinate
};

// Weight
class md5Weight
{
public:
    md5Weight( void );
    md5Weight( const int32_t in_joint, const float in_bias, const glm::vec3 in_pos );
    ~md5Weight( void );

    int32_t     Joint( void ) const { return m_joint; }
    float       Bias( void ) const { return m_bias; }
    glm::vec3   Position( void ) const { return m_pos; }

private:
  int32_t       m_joint;
  float         m_bias;
  glm::vec3     m_pos;
};

class md5Mesh
{
public:
    md5Mesh( void );
    ~md5Mesh( void );

    void    SetShader( const std::string &in_shader );

    void                        ResizeVetices( const uint32_t in_count ) { m_vertices.resize( in_count ); }
    void                        ResizeWeights( const uint32_t in_count ) { m_weights.resize( in_count ); }
    void                        ResizeTriangles( const uint32_t in_count ) { m_triangles.resize( in_count ); }

    uint32_t                    NumVertices( void ) const { return m_vertices.size(); };
    uint32_t                    NumWeights( void ) const { return m_weights.size(); }
    uint32_t                    NumTriangles( void ) const { return m_triangles.size(); }

    std::vector<md5Vertex>      &GetVertices( void ) { return m_vertices; }
    std::vector<md5Weight>      &GetWeights( void ) { return m_weights; }
    std::vector<md5Triangle>    &GetTriangles( void ) { return m_triangles; }

    std::string                 Shader( void ) const { return m_shader; }

private:
    std::string                 m_shader;
    std::vector<md5Vertex>      m_vertices;
    std::vector<md5Weight>      m_weights;
    std::vector<md5Triangle>    m_triangles;
};

#endif //!__MD5MESH_HPP__