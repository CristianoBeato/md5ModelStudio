/*
===============================================================
===============================================================
*/

#ifndef __MD5MODEL_HPP__
#define __MD5MODEL_HPP__

// playn text model version 
inline constexpr int        VERSION = 10;

// binary model version
inline constexpr uint8_t    MD5B_VERSION = 106;
inline constexpr uint32_t   MD5B_MAGIC = ( '5' << 24 ) | ( 'D' << 16 ) | ( 'M' << 8 ) | MD5B_VERSION;

#include "md5Joint.hpp"
#include "md5Mesh.hpp"
#include "md5Anim.hpp"

class md5Model
{
public:
    md5Model( void );
    ~md5Model( void );

    bool    Load( const std::string in_path );
    bool    Save( const std::string in_path );

private:
    std::string             m_comandLine;
    std::vector<md5Joint>   m_joints;
    std::vector<md5Mesh>    m_meshes;

    bool    ParseJoints( const std::ifstream& m_stream );
};

#endif //!__MD5MODEL_HPP__