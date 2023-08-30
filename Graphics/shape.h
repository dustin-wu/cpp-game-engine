#pragma once

#include "GLWrappers/vao.h"

class Shape{
public:
    Shape(std::shared_ptr<VAO> vao);
    ~Shape();

    // Draw function
    void draw();

private:
    std::shared_ptr<VAO> m_vao;
};
