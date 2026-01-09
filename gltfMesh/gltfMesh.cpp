
#include "precompiled.hpp"
#include "gltf.hpp"
#include "gltfMesh.hpp"

gltfPrimitive::gltfPrimitive( void ) : 
    m_materialIndex( 0 )
{
}

gltfPrimitive::~gltfPrimitive( void )
{
}

bool gltfPrimitive::Load( const gltf::Asset &in_asset, const gltf::Primitive &in_primitive )
{
    uint32_t baseColorTexcoordIndex = 0;
    const gltf::Attribute* position = in_primitive.findAttribute( "POSITION" );
        
    assert( position != in_primitive.attributes.end()); // A mesh primitive is required to hold the POSITION attribute.
	assert( in_primitive.indicesAccessor.has_value()); // We specify GenerateMeshIndices, so we should always have indices

    // TODO: remember md5Mesh ( ver 1.0 ) only suport triangles
    switch ( in_primitive.type )
    {
    case gltf::PrimitiveType::Points:
        assert( !"MD5MESH DON'T SUPORT TRIANGLE FAN SURFACE" );
        break;
    case gltf::PrimitiveType::Lines:
        assert( !"MD5MESH DON'T SUPORT TRIANGLE FAN SURFACE" );
        break;
    case gltf::PrimitiveType::LineLoop:
        assert( !"MD5MESH DON'T SUPORT TRIANGLE FAN SURFACE" );
        break;
    case gltf::PrimitiveType::LineStrip:
        assert( !"MD5MESH DON'T SUPORT TRIANGLE FAN SURFACE" );
        break;
    case gltf::PrimitiveType::Triangles:
        break;
    case gltf::PrimitiveType::TriangleStrip:
        assert( !"MD5MESH DON'T SUPORT TRIANGLE FAN SURFACE" );
        break;
    case gltf::PrimitiveType::TriangleFan:
        assert( !"MD5MESH DON'T SUPORT TRIANGLE FAN SURFACE" );
        break;
    default:
        break;
    }

    if ( in_primitive.materialIndex.has_value() )
    {
        m_materialIndex = in_primitive.materialIndex.value() + 1; // Adjust for default material

        auto& material = in_asset.materials[in_primitive.materialIndex.value()];
        auto& baseColorTexture = material.pbrData.baseColorTexture;

        if ( baseColorTexture.has_value() ) 
        {			
            if (baseColorTexture->transform && baseColorTexture->transform->texCoordIndex.has_value()) 
				baseColorTexcoordIndex = baseColorTexture->transform->texCoordIndex.value(); 
            else 
				baseColorTexcoordIndex = material.pbrData.baseColorTexture->texCoordIndex;
        }
    }
    else 
        m_materialIndex = 0;

    // Position
    const gltf::Accessor& positionAccessor = in_asset.accessors[position->accessorIndex];
    if (!positionAccessor.bufferViewIndex.has_value())
        return false;

    m_positions.resize( positionAccessor.count );
    gltf::iterateAccessorWithIndex<gltf::math::fvec3>(in_asset, positionAccessor, [&](gltf::math::fvec3 pos, std::size_t idx) 
    {
		m_positions[idx] = glm::vec3( pos.x(), pos.y(), pos.z() );
	});
    
    std::string texcoordAttribute = std::string("TEXCOORD_") + std::to_string(baseColorTexcoordIndex);
    if ( const gltf::Attribute* texcoord = in_primitive.findAttribute(texcoordAttribute); texcoord != in_primitive.attributes.end() ) 
    {
        // Tex coord
		auto& texCoordAccessor = in_asset.accessors[texcoord->accessorIndex];
        if (!texCoordAccessor.bufferViewIndex.has_value())
            return false; // todo: if we not found vetex 

        m_textureCoords.resize( texCoordAccessor.count );
		gltf::iterateAccessorWithIndex<fastgltf::math::fvec2>( in_asset, texCoordAccessor, [&](gltf::math::fvec2 uv, std::size_t idx) 
        {
			m_textureCoords[idx] = glm::vec2( uv.x(), uv.y() );
		});
    }

    auto& indexAccessor = in_asset.accessors[in_primitive.indicesAccessor.value()];
    if (!indexAccessor.bufferViewIndex.has_value())
            return false;

    m_indices.resize( indexAccessor.count );
    if ( indexAccessor.componentType == gltf::ComponentType::UnsignedByte || indexAccessor.componentType == gltf::ComponentType::UnsignedShort )
    {
        gltf::copyFromAccessor<std::uint16_t>( in_asset, indexAccessor, m_indices.data());
	}
// current no suport for 32bit indexes 
//  else 
//  {
//    	gltf::copyFromAccessor<std::uint32_t>( in_asset, indexAccessor, indices);
//	}

    if ( const gltf::Attribute* joints = in_primitive.findAttribute("JOINTS_0"); joints != in_primitive.attributes.end() ) 
    {
        // joints index
        auto& jointsAccessor = in_asset.accessors[joints->accessorIndex];
        if (!jointsAccessor.bufferViewIndex.has_value())
            return false; // todo: if we not found joints 

        // copy joints
        m_joints.resize( jointsAccessor.count );
		gltf::iterateAccessorWithIndex<gltf::math::u16vec4>( in_asset, jointsAccessor, [&](gltf::math::u16vec4 joints, std::size_t idx) 
        {
			m_joints[idx] = glm::u16vec4( joints.x(), joints.y(), joints.z(), joints.w() );
		});
    }
    
    if ( const gltf::Attribute* weights = in_primitive.findAttribute("WEIGHTS_0"); weights != in_primitive.attributes.end() ) 
    {
        // joints weights
        auto& weightsAccessor = in_asset.accessors[weights->accessorIndex];
        if (!weightsAccessor.bufferViewIndex.has_value())
            return false; // todo: if we not found joints  weights

        gltf::iterateAccessorWithIndex<gltf::math::fvec4>( in_asset, weightsAccessor, [&](gltf::math::fvec4 weight, std::size_t idx )
        {
			m_weights[idx] = glm::vec4( weight.x(), weight.y(), weight.z(), weight.w() );
		});
    }

    return true;
}

gltfMesh::gltfMesh(void)
{
}

gltfMesh::~gltfMesh(void)
{
}

bool gltfMesh::Load( const gltf::Asset &in_asset, const gltf::Mesh &in_mesh)
{
    uint32_t i = 0;

    auto &primitives = in_mesh.primitives;
    m_primitiveList.resize( primitives.size() );
    for (  i = 0; i < primitives.size(); i++)
    {
        m_primitiveList[i].Load( in_asset, primitives[i] );
    }

    //in_mesh.weights.size()

    return true;
}
