#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>
#include <random>

class Cube {
private:
    unsigned int id;
    glm::vec3 color;
    unsigned int VAO, VBO, EBO;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    struct Vertex {
        glm::vec3 position_cords;
        glm::vec2 texture_cords;
    };
    void setupMesh();

public:
    Cube(
        unsigned int id,
        glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale,
        std::mt19937& gen,
        std::uniform_real_distribution<>& dis
    );



    // Move constructor
    Cube(Cube&& other) noexcept;


    ~Cube();

    void setColor(glm::vec3 newColor) { color = newColor; }
    glm::vec3 getColor() const { return color; }

    unsigned int getVAO() const { return VAO; }
    unsigned int getVBO() const { return VBO; }
    unsigned int getEBO() const { return EBO; }
    unsigned int getId() const { return id; }

    void draw();
    glm::mat4 getModelMatrix() const;
};

#endif // CUBE_H
