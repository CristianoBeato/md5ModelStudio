
#ifndef __GLTF_MATERIAL_HPP__
#define __GLTF_MATERIAL_HPP__

class gltfMaterial
{
public:
    gltfMaterial( void );
    ~gltfMaterial( void );

    bool    Load( gltf::Material &material );    
private:
};
#endif //!__GLTF_MATERIAL_HPP__