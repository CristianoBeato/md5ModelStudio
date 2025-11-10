/*
===============================================================
===============================================================
*/

#include "precompiled.hpp"
#include "md5Mesh.hpp"

md5Vertex::md5Vertex( void ) : m_start( 0 ), m_count( 0 ), m_st( 0.0f, 0.0f )
{
}

md5Vertex::md5Vertex( const int32_t in_start, const int32_t in_count, const glm::vec2 in_st ) :
    m_start( in_start ),
    m_count( in_count ),
    m_st( in_st )
{
}


md5Vertex::~md5Vertex( void )
{
    m_start = 0;
    m_count = 0;
    m_st.x = 0.0f;
    m_st.y = 0.0f;
}

md5Weight::md5Weight(void ) : m_joint( 0 ), m_bias( 0.0f ), m_pos( 0.0f )
{
}

md5Weight::md5Weight(const int32_t in_joint, const float in_bias, const glm::vec3 in_pos) : m_joint(in_joint),
                                                                                            m_bias(in_bias),
                                                                                            m_pos(in_pos)
{
}

md5Weight::~md5Weight(void)
{
}

md5Mesh::md5Mesh( void )
{
}

md5Mesh::~md5Mesh( void )
{
}

void md5Mesh::SetShader(const std::string &in_shader )
{
    m_shader = in_shader;
}
