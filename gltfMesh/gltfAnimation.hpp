#ifndef __GLTF_ANIMATION_HPP__
#define __GLTF_ANIMATION_HPP__

class gltfAnimation
{
public:
    gltfAnimation( void );
    ~gltfAnimation( void );

    bool    Load( const gltf::Animation &in_anim );

private:

};

#endif //__GLTF_ANIMATION_HPP__