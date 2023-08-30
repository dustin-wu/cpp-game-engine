#pragma once

#include "../shaderloader.h"
#include "../camera.h"
#include "../material.h"
#include "../modeltransform.h"
#include "../light.h"

class Shader{
public:
    Shader(std::vector<GLenum> shaderTypes, std::vector<const char*> filepaths);
    ~Shader();

    void bind();
    void unbind();
    GLuint getHandle();
    void setMaterial(std::shared_ptr<Material> material);
    void setCamera(std::shared_ptr<Camera> camera);
    void setModelTransform(std::shared_ptr<ModelTransform> modelTransform);
    void setModelTransform(glm::mat4 modelMatrix);
    void setMultipleTransforms(std::string uniformName, std::vector<glm::mat4> matrices);
    void setGlobalCoeffs(glm::vec3 coeffs);
    void setLights(std::vector<std::shared_ptr<Light>> lights);
    void clearLights();
    void setTextUniforms(float screenWidth, float screenHeight, glm::vec3 color);
    void setGuiUniforms(float screenWidth, float screenHeight);

private:
    GLuint m_handle;
};
