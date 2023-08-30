#pragma once


#include "Graphics/GLWrappers/ibo.h"
#include "Graphics/GLWrappers/vao.h"

class MultiBufferVAO
{
public:
    MultiBufferVAO();
    ~MultiBufferVAO();

    void bind();
    void unbind();

    void draw();

    void addIndexBuffer(std::vector<int> indices);
    void addAttribute(GLuint attrib_index, std::vector<float> data, GLint attrib_size);
    void addIntAttribute(GLuint attrib_index, std::vector<int> data, GLint attrib_size);


private:
    std::shared_ptr<IBO> m_ibo;
    std::vector<std::shared_ptr<VBO>> m_vbos;
    std::vector<GLuint> m_attributes;
    GLuint m_handle;
    int m_indexCount = 0;

};
