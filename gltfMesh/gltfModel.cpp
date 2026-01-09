
#include "precompiled.hpp"
#include "gltf.hpp"
#include "gltfModel.hpp"

#include <filesystem>

constexpr auto supportedExtensions =    gltf::Extensions::KHR_mesh_quantization | 
                                        gltf::Extensions::KHR_materials_variants;

                    
constexpr auto gltfOptions =    gltf::Options::DontRequireValidAssetMember | 
                                gltf::Options::AllowDouble | 
                                gltf::Options::LoadExternalBuffers |
                                gltf::Options::DecomposeNodeMatrices | 
                                gltf::Options::GenerateMeshIndices;

gltfModel::gltfModel( void )
{
    m_parser = gltf::Parser( supportedExtensions );
}

gltfModel::~gltfModel( void )
{
}

bool gltfModel::Load( const std::string &in_path )
{
    uint32_t i = 0;
    std::filesystem::path path = in_path;
    
    gltf::Expected<gltf::MappedGltfFile> file = gltf::MappedGltfFile::FromPath( in_path );
	if (!bool(file)) 
    {
		std::cerr << "Failed to open glTF file: " << gltf::getErrorMessage( file.error() ) << '\n';
		return false;
	}

    gltf::Expected<gltf::Asset> assets = m_parser.loadGltf( file.get(), path.parent_path(), gltfOptions);
    if ( assets.error() != gltf::Error::None) 
    {
        std::cerr << "Failed to load glTF: " << gltf::getErrorMessage( assets.error()) << '\n';
        return false;
    }

    //
    //
    // Load skins amarture 
    auto &skins = assets.get().skins;
    m_skins.resize( skins.size() );
    for ( i = 0; i < skins.size(); i++ )
    {
        m_skins[i].Load( assets.get(), skins[i] );
    }

    //
    //
    // parse materials 
    auto &materials = assets.get().materials;
    m_meshList.resize( materials.size() );
    for ( i = 0; i < materials.size(); i++ )
    {
        m_materialList[i].Load( materials[i] );
    }
    
    //
    //
    // Parse model meshes 
    auto &meshes = assets.get().meshes;
    m_meshList.resize( meshes.size() );
    for ( i = 0; i < meshes.size(); i++)
    {
        m_meshList[i].Load( assets.get(), meshes[i] );
    }
        
    //
    //
    // Parse animations
    auto &animation = assets.get().animations;
    m_animationList.resize( animation.size() );
    for ( i = 0; i < animation.size(); i++)
    {
        m_animationList[i].Load( animation[i] );
    }
    
    return false;
}

bool gltfModel::Save( const std::string &in_path )
{
    return false;
}
