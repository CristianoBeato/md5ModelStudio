// render mesh vertex shader 
#version 450

// vertex attribs input 
layout( location = 0 ) in vec2 inSt;    // texture coordinate 
layout( location = 1 ) in uint inFirst; // first weight
layout( location = 2 ) in uint inCount; // num weights

struct joint
{
    vec4 pos;
    vec4 orient;
};

struct weight
{
    uint joint;   
};

// binding = 0: joints
layout(std430, binding = 0) buffer Joints 
{
    joint joints[];
};

// binding = 1: weights pos
layout(std430, binding = 1) buffer WeightsPos 
{
    vec4 positions[];
};

// binding = 2: weights bias 
layout( std430, binding = 2 ) buffer WeightsBiass
{
    float bias[];
};

// binding = 3: weights joit 
layout( std430, binding = 3 ) buffer WeightsJoint
{
    uint jointID[];
};

vec3 quat_rotate( vec4 q, vec3 v ) 
{
    vec3 qv = q.xyz;
    vec3 t = 2.0 * cross(qv, v);
    return v + q.w * t + cross( qv, t );
}

layout( std140 ) uniform uTransform 
{
    mat4 uModel;
    mat4 uViewProj;
} Transform;

void main()
{
    uint start = inFirst;
    uint count = inCount;

    vec3 finalPos = vec3(0.0);
    vec3 finalNormal = vec3(0.0);

    for ( uint i = 0; i < count; ++i)
    {
        uint    wIndex = start + i;
        uint    wJoint = jointID[wIndex];
        float   wBias = bias[wIndex];
        vec3    wPos = positions[wIndex].xyz;

        vec4 jPos = joints[wJoint].pos;
        vec4 jOri = joints[wJoint].orient;

        // rotacionar o weight pos pelo quaternion do joint
        vec3 rotated = quat_rotate(jOri, wPos);

        vec3 transformed = jPos.xyz + rotated;

        finalPos += transformed * wBias;
    }

    // Se você armazenou normal no vertex, faça o mesmo processo para normal (sem soma de jointPos).
    vec4 worldPos = Transform.uModel * vec4(finalPos, 1.0);
    gl_Position = Transform.uViewProj * worldPos;

    v_uv = a_uv;
}
