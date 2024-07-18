#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>

class Cube {
private:
    unsigned int id;
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
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f)
    );



    // Move constructor
    Cube(Cube&& other) noexcept;


    ~Cube();

    unsigned int getVAO() const { return VAO; }
    unsigned int getVBO() const { return VBO; }
    unsigned int getEBO() const { return EBO; }
    unsigned int getId() const { return id; }

    void draw();
    glm::mat4 getModelMatrix() const;
};

#endif // CUBE_H
