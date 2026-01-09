
#ifndef __GLTF_SKIN_HPP__
#define __GLTF_SKIN_HPP__

class gltfJoint
{
public:
    gltfJoint( void );
    ~gltfJoint( void );

    bool    Load( const uint32_t in_index, const gltf::Node &in_node );
    void    Parent( gltfJoint* in_parent );
private:
    glm::vec3               m_transform;
    glm::vec3               m_scale;
    glm::quat               m_rotation;
    gltfJoint*              m_parent;
    std::string             m_name;
    std::vector<uint32_t>   m_children;

};

class gltfSkin
{
public:
    gltfSkin( void );
    ~gltfSkin( void );

    bool    Load( const gltf::Asset &in_asset, const gltf::Skin &in_skin );

private:
    std::vector<gltfJoint> m_joints;
};

#endif //!__GLTF_SKIN_HPP__