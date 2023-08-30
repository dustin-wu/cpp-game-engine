#include "graphics.h"
#include "glm/gtx/norm.hpp"
#include "shapedata.h"
#include "Engine/windowconfig.h"
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Graphics::Graphics()
{
    m_textRenderer = std::make_shared<TextRenderer>();
    m_guiRenderer = std::make_shared<GuiRenderer>();
    m_particleRenderer = std::make_shared<ParticleRenderer>();
}

Graphics::~Graphics(){

}

void Graphics::initializeGLEW(){
    glewExperimental = GL_TRUE;
    std::cout<<"GLEWInit status: "<<glewInit()<<std::endl;
}

void Graphics::initialize(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout<<"initialize text renderer"<<std::endl;
    m_textRenderer->initialize();
    m_guiRenderer->initialize();
    m_particleRenderer->initialize();

    std::cout<<"add shapes"<<std::endl;
    addShape("quad", quadVertexBufferData, VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV);
    addShape("cube", cubeVertexBufferData, VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV);
    addShape("sphere", sphereVertexBufferData, VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV);
    addShape("cylinder", cylinderVertexBufferData, VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV);
    addShape("cone", coneVertexBufferData, VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV);

    addShader("phong", {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"Resources/Shaders/phong.vert", "Resources/Shaders/phong.frag"});
    addShader("text", {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"Resources/Shaders/text.vert", "Resources/Shaders/text.frag"});
    addShader("gui", {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"Resources/Shaders/gui.vert", "Resources/Shaders/gui.frag"});
    addShader("beam", {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"Resources/Shaders/phong.vert", "Resources/Shaders/beam.frag"});
    addShader("particles", {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"Resources/Shaders/particles.vert", "Resources/Shaders/particles.frag"});
    addShader("animation", {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"Resources/Shaders/animation.vert", "Resources/Shaders/phong.frag"});

    bindShader("phong");

    addMaterial("default", glm::vec3(1));

    std::cout<<"add font"<<std::endl;
    addFont("opensans", "Resources/Fonts/OpenSans.ttf");
    getFont("opensans");
    addFont("joystix", "Resources/Fonts/joystix.otf");
    getFont("joystix");

    m_windowSize = glm::vec2(STARTING_WINDOW_WIDTH, STARTING_WINDOW_HEIGHT);
    std::cout<<"opened window"<<std::endl;

}

void Graphics::clearScreen(GLbitfield mask){
    glClear(mask);
}

void Graphics::setClearColor(glm::vec3 clearColor){
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
}

void Graphics::setCameraData(std::shared_ptr<Camera> camera){
    m_active_shader->setCamera(camera);
}

void Graphics::addShader(std::string shaderName, std::vector<GLenum> shaderTypes, std::vector<const char*> filepaths){
    m_shaders.insert({shaderName, std::make_shared<Shader>(shaderTypes, filepaths)});
}

void Graphics::removeShader(std::string shaderName){
    if(shaderName != "phong" && shaderName != "text"){
        m_shaders.erase(shaderName);
    }
}

void Graphics::bindShader(std::string shaderName){
    m_shaders.at(shaderName)->bind();
    m_active_shader = m_shaders.at(shaderName);
//    std::cout << "bound to shader: " << shaderName << std::endl;
}

std::shared_ptr<Shape> Graphics::addShape(std::string shapeName, std::vector<float> data, VAOAttrib attribs){
    m_shapes.insert({shapeName, std::make_shared<Shape>(std::make_shared<VAO>(std::make_shared<VBO>(data), attribs))});
    return m_shapes.at(shapeName);
}
std::vector<glm::vec3> Graphics::addShape(std::string shapeName, std::string filepath, bool hasUV){
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    int numTriangles = 0;
    for(size_t s = 0; s < shapes.size(); s++){
        numTriangles += shapes[s].mesh.num_face_vertices.size();
    }

    std::vector<float> drawData;
    drawData.resize(numTriangles * 3 * 8);
    std::vector<glm::vec3> collisionData;
    collisionData.resize(numTriangles * 3);

    int i = 0;
    int j = 0;
    for(size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            unsigned int fv = shapes[s].mesh.num_face_vertices[f];

            for(size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                // Add position
                drawData[i] = attrib.vertices[3*idx.vertex_index];
                drawData[i + 1] = attrib.vertices[3*idx.vertex_index + 1];
                drawData[i + 2] = attrib.vertices[3*idx.vertex_index + 2];
                // Add normal
                drawData[i + 3] = attrib.normals[3*idx.normal_index];
                drawData[i + 4] = attrib.normals[3*idx.normal_index + 1];
                drawData[i + 5] = attrib.normals[3*idx.normal_index + 2];
                // Add uv
                drawData[i + 6] = 0;
                drawData[i + 7] = 0;
                if(hasUV && shapeName != "sprayMesh"){
                    drawData[i + 6] = attrib.texcoords[2*idx.texcoord_index];
                    drawData[i + 7] = attrib.texcoords[2*idx.texcoord_index + 1];
                }

                // Add collision position data
                collisionData[j] = glm::vec3(attrib.vertices[3*idx.vertex_index], attrib.vertices[3*idx.vertex_index + 1], attrib.vertices[3*idx.vertex_index + 2]);
                i += 8;
                j += 1;
            }

            index_offset += fv;
        }
    }

    m_shapes.insert({shapeName, std::make_shared<Shape>(std::make_shared<VAO>(std::make_shared<VBO>(drawData), VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV))});

    return collisionData;
}

std::pair<std::vector<glm::vec3>, std::vector<glm::ivec3>> Graphics::getNavmeshData(std::string filepath){
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    int numTriangles = 0;
    for(size_t s = 0; s < shapes.size(); s++){
        numTriangles += shapes[s].mesh.num_face_vertices.size();
    }

    std::cout << numTriangles << std::endl;

    std::vector<glm::vec3> positions;
    std::vector<glm::ivec3> faces;

    for(int i = 0; i<attrib.vertices.size(); i+=3){
        positions.push_back(glm::vec3(attrib.vertices[i], attrib.vertices[i+1], attrib.vertices[i+2]));
    }


    int i = 0;
    int j = 0;
    for(size_t s = 0; s < shapes.size(); s++) {
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            tinyobj::index_t idx1 = shapes[s].mesh.indices[3* f];
            tinyobj::index_t idx2 = shapes[s].mesh.indices[3* f + 1];
            tinyobj::index_t idx3 = shapes[s].mesh.indices[3* f + 2];
            faces.push_back(glm::ivec3(idx1.vertex_index, idx2.vertex_index, idx3.vertex_index));
        }
    }

    return std::make_pair(positions, faces);
}

void Graphics::removeShape(std::string shapeName){
    m_shapes.erase(shapeName);
}

std::shared_ptr<Shape> Graphics::getShape(std::string shapeName){
    return m_shapes.at(shapeName);
}

void Graphics::drawShape(std::shared_ptr<Shape> myShape, std::shared_ptr<ModelTransform> modelTransform, std::shared_ptr<Material> material){
    if(material == nullptr){
        m_active_shader->setMaterial(getMaterial("default"));
    }
    else{
        m_active_shader->setMaterial(material);
    }
    m_active_shader->setModelTransform(modelTransform);

    myShape->draw();
}

void Graphics::drawShape(std::shared_ptr<Shape> myShape, glm::mat4 modelMatrix, std::shared_ptr<Material> material){
    if(material == nullptr){
        m_active_shader->setMaterial(getMaterial("default"));
    }
    else{
        m_active_shader->setMaterial(material);
    }
    m_active_shader->setModelTransform(modelMatrix);

    myShape->draw();
}

void Graphics::drawAnimatedEntity(std::shared_ptr<AnimatedModel> entity, glm::mat4 modelMatrix) {
    m_active_shader->setMaterial(getMaterial(entity->getTexture()));
//    m_active_shader->setMaterial(getMaterial("default"));
    m_active_shader->setModelTransform(modelMatrix);
    std::vector<glm::mat4> jointTransforms = entity->getJointTransforms();

    m_active_shader->setMultipleTransforms("jointTransforms", jointTransforms);
    entity->getVAO()->draw();
}


void Graphics::drawBeam(glm::vec3 startPos, glm::vec3 endPos, float lineWidth, std::string texture) {
    m_active_shader->setMaterial(getMaterial(texture));

    static GLuint vbo = 0;
    static GLuint vao = 0;
    if (vbo == 0) {
        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(2*sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    glm::vec3 direction = endPos - startPos;
    float length = glm::length(direction);
    direction /= length;
    glm::vec3 up(0, 1, 0);
    if (glm::dot(direction, up) > 0.99) {
        up = glm::vec3(1, 0, 0);
    }
    glm::vec3 right = glm::cross(direction, up);
    up = glm::cross(right, direction);

    glm::vec3 topLeft = startPos + up * lineWidth;
    glm::vec3 topRight = endPos + up * lineWidth;
    glm::vec3 bottomLeft = startPos - up * lineWidth;
    glm::vec3 bottomRight = endPos - up * lineWidth;

    glm::vec3 topLeft1 = startPos + right * lineWidth;
    glm::vec3 topRight1 = endPos + right * lineWidth;
    glm::vec3 bottomLeft1 = startPos - right * lineWidth;
    glm::vec3 bottomRight1 = endPos - right * lineWidth;

    // Define the vertices for the two quads
    GLfloat lineVertices[] = {
        // First quad
        topLeft.x, topLeft.y, topLeft.z, 0.1, 0.1,
        topRight.x, topRight.y, topRight.z, 1, 0.1,
        bottomLeft.x, bottomLeft.y, bottomLeft.z, 0.1, 1,
        bottomRight.x, bottomRight.y, bottomRight.z, 1, 1,
        // Second quad
        topLeft1.x, topLeft1.y, topLeft1.z, 0.1, 0.1,
        topRight1.x, topRight1.y, topRight1.z, 1, 0.1,
        bottomLeft1.x, bottomLeft1.y, bottomLeft1.z, 0.1, 1,
        bottomRight1.x, bottomRight1.y, bottomRight1.z, 1, 1
    };

    m_active_shader->setModelTransform(glm::mat4(1));

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
    glDisable(GL_CULL_FACE);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    glBindVertexArray(0);
    glEnable(GL_CULL_FACE);

    glDisableVertexAttribArray(0);
}

std::vector<float> Graphics::getObjData(std::string filepath){
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::vector<glm::vec3> faces;
    std::vector<glm::vec3> vertices;

    for(size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            unsigned int fv = shapes[s].mesh.num_face_vertices[f];

            glm::vec3 face;
            for(size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                face[v] = idx.vertex_index;

            }
            faces.push_back(face);

            index_offset += fv;
        }
    }
    for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
        vertices.emplace_back(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    }

    std::vector<float> data;
    data.resize(18 * faces.size());
    for(int i = 0; i<faces.size(); i++){
        glm::vec3 v0 = vertices[static_cast<int>(faces[i].x)];
        glm::vec3 v1 = vertices[static_cast<int>(faces[i].y)];
        glm::vec3 v2 = vertices[static_cast<int>(faces[i].z)];
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        data[18*i] = v0.x;
        data[18*i+1] = v0.y;
        data[18*i+2] = v0.z;
        data[18*i+3] = normal.x;
        data[18*i+4] = normal.y;
        data[18*i+5] = normal.z;
        data[18*i+6] = v1.x;
        data[18*i+7] = v1.y;
        data[18*i+8] = v1.z;
        data[18*i+9] = normal.x;
        data[18*i+10] = normal.y;
        data[18*i+11] = normal.z;
        data[18*i+12] = v2.x;
        data[18*i+13] = v2.y;
        data[18*i+14] = v2.z;
        data[18*i+15] = normal.x;
        data[18*i+16] = normal.y;
        data[18*i+17] = normal.z;
    }

    return data;
}

std::shared_ptr<Material> Graphics::addMaterial(std::string materialName, glm::vec3 color, float shininess){
    std::shared_ptr<Material> newMaterial = std::make_shared<Material>(color, shininess);
    m_materials.insert({materialName, newMaterial});
    return m_materials.at(materialName);
}

std::shared_ptr<Material> Graphics::addMaterial(std::string materialName, float shininess){
    std::shared_ptr<Material> newMaterial = std::make_shared<Material>(shininess);
    m_materials.insert({materialName, newMaterial});
    return m_materials.at(materialName);
}

std::shared_ptr<Material> Graphics::addMaterial(std::string materialName, std::string filePath, float shininess){
    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(filePath);
    std::shared_ptr<Material> newMaterial = std::make_shared<Material>(newTexture, shininess);
    m_materials.insert({materialName, newMaterial});
    return m_materials.at(materialName);
}

void Graphics::removeMaterial(std::string materialName){
    if(materialName != "default"){
        m_materials.erase(materialName);
    }
}

std::shared_ptr<Material> Graphics::getMaterial(std::string materialName){
    return m_materials.at(materialName);
}

std::shared_ptr<Font> Graphics::addFont(std::string fontName, std::string filepath){
    std::shared_ptr<Font> newFont = std::make_shared<Font>(filepath);
    m_fonts.insert({fontName, newFont});
    return m_fonts.at(fontName);
}

void Graphics::removeFont(std::string fontName){
    m_fonts.erase(fontName);
}

std::shared_ptr<Font> Graphics::getFont(std::string fontName){
    return m_fonts.at(fontName);
}

void Graphics::drawUIText(std::shared_ptr<Font> font, std::string text, glm::vec2 anchorPosition, AnchorPoint anchorPoint, float textBoxWidth, float fontSize, float lineSpacing, glm::vec3 textColor){
    m_active_shader->setTextUniforms(m_windowSize.x, m_windowSize.y, textColor);
    m_textRenderer->renderUIText(font, text, anchorPosition, anchorPoint, textBoxWidth, fontSize, lineSpacing, textColor);

}

void Graphics::drawGUI(std::shared_ptr<GuiElement> guiElement, GUIAnchorPoint anchor) {
    if (guiElement->hasTexture()) {
        m_active_shader->setGuiUniforms(m_windowSize.x, m_windowSize.y);
        m_active_shader->setMaterial(getMaterial(guiElement->getTexture()));
        m_guiRenderer->renderGUIElement(guiElement, anchor);
    }
    if (!guiElement->hasText()) return;

    bindShader("text");
    std::shared_ptr<TextInfo> text = guiElement->getText();
    m_active_shader->setTextUniforms(m_windowSize.x, m_windowSize.y, text->textColor);
    float xpos = ((guiElement->getPosition().x + text->textOffset.x) * 0.5f + 0.5f) * m_windowSize.x;
    float ypos = ((guiElement->getPosition().y + text->textOffset.y) * 0.5f + 0.5f) * m_windowSize.y;
    clearScreen(GL_DEPTH_BUFFER_BIT);

    m_textRenderer->renderUIText(getFont(text->font), text->text, glm::vec2(xpos , ypos), text->fontSize, text->textColor);
    bindShader("gui");
}

void Graphics::setGlobalData(glm::vec3 globalCoeffs){
    m_active_shader->setGlobalCoeffs(globalCoeffs);
}

void Graphics::setLights(std::vector<std::shared_ptr<Light>> lights){
    m_active_shader->setLights(lights);
}

void Graphics::clearLights(){
    m_active_shader->clearLights();
}

void Graphics::setWindowSize(glm::ivec2 windowSize){
    m_windowSize = windowSize;
}

glm::ivec2 Graphics::getWindowSize(){
    return m_windowSize;
}

void Graphics::setFramebufferSize(glm::ivec2 framebufferSize){
    m_framebufferSize = framebufferSize;
    glViewport(0, 0, m_framebufferSize.x, m_framebufferSize.y);
}

glm::ivec2 Graphics::getFramebufferSize(){
    return m_framebufferSize;
}

glm::mat4 Graphics::billboardModel(Particle particle, std::shared_ptr<Camera> camera) {
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, particle.getPosition());

    glm::mat3 viewTranspose = glm::transpose(camera->getView());
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
//            std::cout << viewTranspose[i][j] << std::endl;
            model[i][j] = viewTranspose[i][j];
        }
    }

    model = glm::rotate(model, glm::radians(particle.getRotation()), glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(particle.getScale()));
    return model;
}

void Graphics::renderParticles(std::unordered_map<std::string, std::vector<Particle>>& particles, std::shared_ptr<Camera> camera) {
    bindShader("particles");

    for (auto& [texture, particlesList]: particles) {
        m_active_shader->setMaterial(getMaterial(texture));
        std::sort(particlesList.begin(), particlesList.end(),
            [&](Particle& a, Particle& b) -> bool
            {
                return glm::distance2(a.getPosition(), camera->getPos()) > glm::distance2(b.getPosition(), camera->getPos());
            });

        for (const Particle &particle: particlesList) {
            glm::mat4 model = billboardModel(particle, camera);
            m_active_shader->setModelTransform(model);
            m_particleRenderer->renderParticle(particle);
        }
    }

}
