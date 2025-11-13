#version 460 core

layout( location = 0 ) in vec2 inPos;    // vertex position 
layout( location = 1 ) in vec2 inTco;    // texture coordinate

layout( std140, binding = 0 ) uniform uTransform 
{
    vec4 uColor;   
    mat4 uProj;
} uniforms;

layout( location = 0 ) out VertexData
{
   vec2 texCoord;
   vec4 color;
} vs_out;

void main()
{
    gl_Position = uniforms.uProj * vec4( inPos, 0.0f, 1.0f );
    vs_out.color = uniforms.uColor;
    vs_out.texCoord = inTco;
}