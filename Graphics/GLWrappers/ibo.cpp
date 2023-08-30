#include "ibo.h"
#include <iostream>

IBO::IBO(std::vector<int> indices):
    m_length(indices.size())
{
    glGenBuffers(1, &m_handle);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_length*sizeof(int), indices.data(), GL_STATIC_DRAW);
    unbind();
}

IBO::~IBO(){
    glDeleteBuffers(1, &m_handle);
}

void IBO::bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
}

void IBO::unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLsizei IBO::getLength(){
    return m_length;
}
