
#ifndef __VIEW_STRUCTURES_HPP__
#define __VIEW_STRUCTURES_HPP__

#include <glm/glm.hpp>

struct viewVert_t
{
    glm::vec3   position;
    glm::vec3   normal;
    glm::vec2   tcoord;
    glm::u8vec4 color;   
};

struct viewUniforms_t
{
    glm::mat4       model;          // model matrix
    glm::mat4       view;           // view matrix
    glm::mat4       projection;     // projection matrix
};

struct drawElements_t
{
    GLuint  count;
    GLuint  instanceCount;
    GLuint  firstIndex;
    GLint   baseVertex;
    GLuint  baseInstance;
};

#endif //!__VIEW_STRUCTURES_HPP__