#pragma once

#include <GL/glew.h>
#include <vector>

class IBO
{
public:
    IBO(std::vector<int> indices);
    ~IBO();

    void bind();
    void unbind();
    GLsizei getLength();

private:
    GLuint m_handle;
    GLsizei m_length;
};
