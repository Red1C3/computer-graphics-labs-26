#include "model.h"

Model::Model(vector<vec3> positions, vector<vec3> colors) {
    verticesCount = positions.size();
    vector<vec3> vertices = positions;

    for (auto v : colors) {
        vertices.push_back(v);
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // A buffer is a reference in the VRAM
    GLuint VBO;

    // First we define a buffer
    glGenBuffers(1, &VBO);

    // Then we set it as the active buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Now we are ready to send data to VRAM
    // We send the size of the whole data
    // As well as the data itself
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), vertices.data(),
        GL_STATIC_DRAW);

    // We describe how the data is read from the vertex shader
    // Notice how the position is 3 floats so we set the stride (space between
    // each vertex) to 3*sizeof(float)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
        (void*)(((vertices.size() / 2)) * sizeof(vec3)));

    // Finally we need to set the location 0 as enabled
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Model::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);
}
