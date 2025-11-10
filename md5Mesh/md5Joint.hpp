/*
===============================================================
===============================================================
*/

#ifndef __MD5_JOINT_HPP__
#define __MD5_JOINT_HPP__

struct vertexWeight_t
{
	int         joint;          // joint index
	glm::vec3   offset;         // joint based offset
	float       jointWeight;    
};

class md5Joint
{
public:
    md5Joint( void );
    ~md5Joint( void );

    void    Configure( const int32_t in_index, const std::string in_name, const md5Joint* in_parent );

    void    Transfom( glm::vec3 in_pos, glm::quat ori, const bool calcW = false );

    /// @brief Bone name string  
    const std::string   Name( void ) const { return m_name; }

    /// @brief get this joint idex
    int32_t             GetIndex( void ) const { return m_index; }

    /// @brief get parent joint index
    int32_t             GetParentIndex( void ) const;

    glm::vec3           GetPosition( void ) const { return m_position; }

    glm::quat           GetOrientation( void ) const { return m_orient; }

private:
    int32_t         m_index;
    glm::vec3       m_position; 
    glm::quat       m_orient;
    std::string     m_name;
    md5Joint*       m_parent;
};

#endif //__MD5_JOINT_HPP__