
#ifndef __GLTF_MESH_HPP__
#define __GLTF_MESH_HPP__

class gltfPrimitive
{
    
public:
    gltfPrimitive( void );
    ~gltfPrimitive( void );

    bool Load( const gltf::Asset &in_asset, const gltf::Primitive &in_primitiver );

private:
    uint32_t                    m_materialIndex;
    std::vector<uint16_t>       m_indices;
    std::vector<glm::vec3>      m_positions;
    std::vector<glm::vec2>      m_textureCoords;
    std::vector<glm::u16vec4>   m_joints;
    std::vector<glm::vec4>      m_weights;
};

class gltfMesh
{
public:
    gltfMesh( void );
    ~gltfMesh( void );

    bool Load( const gltf::Asset &in_asset, const gltf::Mesh &in_mesh );

private:
    std::vector<gltfPrimitive>  m_primitiveList;
};

#endif //__GLTF_MESH_HPP__