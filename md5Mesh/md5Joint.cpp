/*
===============================================================
===============================================================
*/

#include "precompiled.hpp"
#include "md5Joint.hpp"

#include <glm/ext/quaternion_common.hpp>

inline static void computeQuaternionW( glm::quat& q ) 
{
    q.w = std::sqrt( fmax( 0.0f, 1.0f - q.x*q.x - q.y*q.y - q.z*q.z) );
}

md5Joint::md5Joint( void ) : m_index( 0 ), m_parent( nullptr )
{
}

md5Joint::~md5Joint( void )
{
}

void md5Joint::Configure(const int32_t in_index, const std::string in_name, const md5Joint* in_parent )
{
    m_index = in_index;
    m_name = in_name;
    m_parent = const_cast<md5Joint*>( in_parent );
}

void md5Joint::Transfom(glm::vec3 in_pos, glm::quat in_ori, const bool calcW )
{
    m_position = in_pos;
    m_orient = in_ori;

    if ( calcW )
        computeQuaternionW( m_orient );
}

int md5Joint::GetParentIndex(void) const
{
    // no parent, return as origin
    if ( m_parent == nullptr )
        return -1;

    // get the index of the parent
    return m_parent->GetIndex();
}
