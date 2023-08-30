#include "multibuffervao.h"
#include <iostream>

MultiBufferVAO::MultiBufferVAO()
{
    glGenVertexArrays(1, &m_handle);
    bind();
}

MultiBufferVAO::~MultiBufferVAO(){
    glDeleteVertexArrays(1, &m_handle);
}

void MultiBufferVAO::bind(){
    glBindVertexArray(m_handle);
    for (GLuint attrib: m_attributes) {
        glEnableVertexAttribArray(attrib);
    }
}

void MultiBufferVAO::unbind(){
    for (GLuint attrib: m_attributes) {
        glDisableVertexAttribArray(attrib);
    }
    glBindVertexArray(0);
}

void MultiBufferVAO::addIndexBuffer(std::vector<int> indices) {
    m_ibo = std::make_shared<IBO>(indices);
    m_indexCount = indices.size();
}

void MultiBufferVAO::addAttribute(GLuint attrib_index, std::vector<float> data, GLint attrib_size){
    std::shared_ptr<VBO> newVBO = std::make_shared<VBO>(data);
    newVBO->bind();
    glVertexAttribPointer(attrib_index, attrib_size, GL_FLOAT, GL_FALSE, attrib_size*sizeof(GLfloat), 0);
    m_vbos.push_back(newVBO);
    m_attributes.push_back(attrib_index);
    newVBO->unbind();
}

void MultiBufferVAO::addIntAttribute(GLuint attrib_index, std::vector<int> data, GLint attrib_size) {
    std::shared_ptr<VBO> newVBO = std::make_shared<VBO>(data);
    newVBO->bind();
    glVertexAttribIPointer(attrib_index, attrib_size, GL_INT, attrib_size*sizeof(GLint), 0);
    m_vbos.push_back(newVBO);
    m_attributes.push_back(attrib_index);
    newVBO->unbind();
}

void MultiBufferVAO::draw(){
    bind();
    m_ibo->bind();
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    m_ibo->unbind();
    unbind();
}

