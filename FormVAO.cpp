#include "FormVAO.h"

void createButtonFrameVAO(unsigned int& VAO)
{
    float vertices[] = {
        -0.1f,  0.1f,
        -0.1f, -0.1f,
         0.1f, -0.1f,
         0.1f,  0.1f
    };

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void createRectVAO(float* verticesRect, size_t rectSize, unsigned int& VAOrect) {

    unsigned int VBOrect;
    glGenVertexArrays(1, &VAOrect);
    glGenBuffers(1, &VBOrect);

    glBindVertexArray(VAOrect);
    glBindBuffer(GL_ARRAY_BUFFER, VBOrect);
    glBufferData(GL_ARRAY_BUFFER, rectSize, verticesRect, GL_STATIC_DRAW);

    // Atribut 0 (pozicija):
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atribut 1 (boja):
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void createElevatorVAO(float* vertices, size_t size, unsigned int& elevatorVAO) {
    unsigned int elevatorVBO;
    glGenVertexArrays(1, &elevatorVAO);
    glGenBuffers(1, &elevatorVBO);

    glBindVertexArray(elevatorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, elevatorVBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void createButtonVAO(unsigned int& VAO)
{
    float vertices[] = {
        -0.1f,  0.1f,  0.0f, 1.0f,
        -0.1f, -0.1f,  0.0f, 0.0f,
         0.1f, -0.1f,  1.0f, 0.0f,
         0.1f,  0.1f,  1.0f, 1.0f
    };

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void createLineVAO(unsigned int& lineVAO)
{
    float vertices[] = {
        -0.5f,  1.0f,
        -0.5f, -1.0f,
         0.5f, -1.0f,
         0.5f,  1.0f
    };

    unsigned int VBO;
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
