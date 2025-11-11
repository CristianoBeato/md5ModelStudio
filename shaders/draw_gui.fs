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
    fragment = texture( uTexture, fs_in.texCoord ) * fs_in.color;
}