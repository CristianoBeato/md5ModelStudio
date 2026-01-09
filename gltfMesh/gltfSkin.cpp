#include "precompiled.hpp"
#include "gltf.hpp"
#include "gltfSkin.hpp"

gltfJoint::gltfJoint( void ) : m_parent( nullptr )
{
}

gltfJoint::~gltfJoint( void )
{
}

bool gltfJoint::Load(const uint32_t in_index, const gltf::Node &in_node)
{
    // get joint name
    m_name = in_node.name.empty() ? "joint_" + std::to_string(in_index) : in_node.name.c_str();

    auto transform = std::get<gltf::TRS>( in_node.transform );
    m_transform = glm::vec3( transform.translation.x(), transform.translation.y(), transform.translation.z() ); 
    m_rotation = glm::qua( transform.rotation.x(), transform.rotation.y(), transform.rotation.z(), transform.rotation.w() );
    m_scale = glm::vec3( transform.scale.x(), transform.scale.y(), transform.scale.z() );

    return true;
}

void gltfJoint::Parent( gltfJoint* in_parent )
{
    m_parent = in_parent;
}

gltfSkin::gltfSkin( void )
{
}

gltfSkin::~gltfSkin( void )
{
}

bool gltfSkin::Load( const gltf::Asset &in_asset, const gltf::Skin &in_skin )
{
    m_joints.resize( in_skin.joints.size() );
    for ( uint32_t i = 0; i < in_skin.joints.size(); i++)
    {
        auto& node = in_asset.nodes[in_skin.joints[i]];
        m_joints[i].Load( i, node );
        
        // Set childrens
        for ( uint32_t j = 0; i < node.children.size(); i++)
        {
            auto children = node.children[j];
            m_joints[children].Parent( &m_joints[i] );
        }
    }
    
    return true;
}
