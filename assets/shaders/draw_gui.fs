#version 460 core

// fragment color output 
layout (location = 0) out vec4 fragment;

// binding sampler location
layout( binding = 0 ) uniform sampler2D uTexture;

// vertex input
layout( location = 0 ) in VertexData
{
   vec2 texCoord;
   vec4 color;
} fs_in;

//
void main()
{
    vec4 smpcolor = texture( uTexture, fs_in.texCoord ) * fs_in.color; 
    if( smpcolor.a < 0.1 )
        discard;
    
    fragment = smpcolor;
}