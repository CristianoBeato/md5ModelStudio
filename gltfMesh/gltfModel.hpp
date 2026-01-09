
#ifndef __GLTF_MODEL_HPP__
#define __GLTF_MODEL_HPP__

class gltfModel
{
public:
    gltfModel( void );
    ~gltfModel( void );

    bool    Load( const std::string &in_path );
    bool    Save( const std::string &in_path );

private:
    gltf::Parser                            m_parser;
    std::vector<gltfSkin>                   m_skins;
    std::vector<gltfMaterial>               m_materialList;
    std::vector<gltfMesh>                   m_meshList;
    std::vector<gltfAnimation>              m_animationList;
};

#endif //!__GLTF_MODEL_HPP__