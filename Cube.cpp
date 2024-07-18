#include "Cube.h"
#include <iostream>

Cube::Cube(unsigned int id, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
    std::mt19937& gen, std::uniform_real_distribution<>& dis)
    : id(id), position(position), rotation(rotation), scale(scale),
    VAO(0), VBO(0), EBO(0) {
    // Generate a random color
    color = glm::vec3(dis(gen), dis(gen), dis(gen));

    std::cout << "Cube " << id << " constructed at " << this
        << " with color: " << color.r << ", " << color.g << ", " << color.b << std::endl;
    this->setupMesh();
}

Cube::~Cube() {
    std::cout << "Cube " << id << " destroyed at " << this << std::endl;
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
    }
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
    }
}

void Cube::setupMesh() {
    struct Vertex {
        glm::vec3 position_cords;
        glm::vec2 texture_cords;
    };
    Vertex vertices[] = {
        {{-0.5f, -0.5f, -0.5f},{0.0f,0.0f}},
        {{0.5f, -0.5f, -0.5f},{1.0f,0.0f}},
        {{0.5f, 0.5f, -0.5f},{1.0f,1.0f}},
        {{-0.5f, 0.5f, -0.5f},{0.0f,1.0f}},
        {{-0.5f, -0.5f, 0.5f},{0.0f,0.0f}},
        {{0.5f, -0.5f, 0.5f},{1.0f,0.0f}},
        {{0.5f, 0.5f, 0.5f},{1.0f,1.0f}},
        {{-0.5f, 0.5f, 0.5f},{0.0f,1.0f}}
    };

    unsigned int indices[] = {
        0, 1, 2,  2, 3, 0,
        4, 5, 6,  6, 7, 4,
        0, 4, 7,  7, 3, 0,
        1, 5, 6,  6, 2, 1,
        3, 2, 6,  6, 7, 3,
        0, 1, 5,  5, 4, 0
    };

    glGenVertexArrays(1, &VAO);
    std::cout << "Generated VAO: " << VAO << std::endl;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, position_cords));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, texture_cords));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Move constructor
Cube::Cube(Cube&& other) noexcept
    : id(other.id),
      position(std::move(other.position)),
      rotation(std::move(other.rotation)),
      scale(std::move(other.scale)),
      color(std::move(other.color)),  // Add this line
      VAO(other.VAO),
      VBO(other.VBO),
      EBO(other.EBO) {
    other.VAO = other.VBO = other.EBO = 0;
    std::cout << "Cube " << id << " move constructed at " << this
              << " with VAO: " << VAO
              << " and color: " << color.r << ", " << color.g << ", " << color.b << std::endl;
}


void Cube::draw() {
    glBindVertexArray(VAO);
    // Check if the VBO and EBO are still bound to the VAO
    GLint vbo_bound, ebo_bound;
    glGetVertexArrayiv(VAO, GL_VERTEX_ARRAY_BUFFER_BINDING, &vbo_bound);
    glGetVertexArrayiv(VAO, GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo_bound);

    // Check vertex attribute state
    GLint enabled;
    glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    glGetVertexAttribiv(1, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

}

glm::mat4 Cube::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    return model;
}
