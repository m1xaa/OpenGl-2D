#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Header/Util.h"


unsigned int indexTexture;
unsigned int suTexture, prTexture, firstTexture, secondTexture, thirdTexture, fourthTexture, fifthTexture, sixthTexture;
unsigned int openTexture, closeTexture, stopTexture, ventTexture;


void preprocessTexture(unsigned& texture, const char* filepath) {
    texture = loadImageToTexture(filepath); 
    glBindTexture(GL_TEXTURE_2D, texture);


    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void formVAOs(float* verticesRect, size_t rectSize, unsigned int& VAOrect) {

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


void drawRect(unsigned int rectShader, unsigned int VAOrect) {
    glUseProgram(rectShader); 
    glBindVertexArray(VAOrect);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
}

void drawButton(unsigned int texture, float x, float y, unsigned int panelShader, unsigned int buttonVAO)
{
    glUseProgram(panelShader);

    glUniform2f(glGetUniformLocation(panelShader, "uPos"), x, y);
    glUniform2f(glGetUniformLocation(panelShader, "uScale"), 0.8f, 0.8f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(buttonVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(
        mode->width,
        mode->height,
        "Kostur",
        monitor,   
        NULL
    );

    if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) return endProgram("GLEW nije uspeo da se inicijalizuje.");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned int indexShader = createShader("index.vert", "index.frag");
    unsigned int panelShader = createShader("panel.vert", "panel.frag");

    unsigned int buttonVAO;
    createButtonVAO(buttonVAO);

    preprocessTexture(suTexture, "Resources/-1.png");
    preprocessTexture(prTexture, "Resources/0.png");
    preprocessTexture(firstTexture, "Resources/1.png");
    preprocessTexture(secondTexture, "Resources/2.png");
    preprocessTexture(thirdTexture, "Resources/3.png");
    preprocessTexture(fourthTexture, "Resources/4.png");
    preprocessTexture(fifthTexture, "Resources/5.png");
    preprocessTexture(sixthTexture, "Resources/6.png");

    preprocessTexture(openTexture, "Resources/open.png");
    preprocessTexture(closeTexture, "Resources/close.png");
    preprocessTexture(stopTexture, "Resources/stop.jpg");
    preprocessTexture(ventTexture, "Resources/vent.png");

    glUseProgram(panelShader);
    glUniform1i(glGetUniformLocation(panelShader, "tex"), 0);



    float verticesRect[] = {
    -1.0f,  1.0f, 0.0f, 1.0f,  
    -1.0f,  0.6f, 0.0f, 0.0f,   
    -0.6f,  0.6f, 1.0f, 0.0f,   
    -0.6f,  1.0f, 1.0f, 1.0f
    };


    unsigned int VAOrect;
    formVAOs(verticesRect, sizeof(verticesRect), VAOrect);

    glClearColor(0.2f, 0.8f, 0.6f, 1.0f);

    preprocessTexture(indexTexture, "Resources/name.png");

    glUseProgram(indexShader);
    glUniform1i(glGetUniformLocation(indexShader, "tex"), 0);


    glfwSetKeyCallback(window, key_callback);
    

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT); 

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, indexTexture);

        drawRect(indexShader, VAOrect);

        float col1 = -0.85f;
        float col2 = -0.65f;

        float yTop = 0.60f;
        float dy = -0.25f;

        drawButton(suTexture, col1, yTop, panelShader, buttonVAO);
        drawButton(prTexture, col2, yTop, panelShader, buttonVAO);

        drawButton(firstTexture, col1, yTop + dy, panelShader, buttonVAO);
        drawButton(secondTexture, col2, yTop + dy, panelShader, buttonVAO);

        drawButton(thirdTexture, col1, yTop + 2 * dy, panelShader, buttonVAO);
        drawButton(fourthTexture, col2, yTop + 2 * dy, panelShader, buttonVAO);

        drawButton(fifthTexture, col1, yTop + 3 * dy, panelShader, buttonVAO);
        drawButton(sixthTexture, col2, yTop + 3 * dy, panelShader, buttonVAO);

 
        drawButton(openTexture, col1, yTop + 4 * dy, panelShader, buttonVAO);
        drawButton(closeTexture, col2, yTop + 4 * dy, panelShader, buttonVAO);

        drawButton(stopTexture, col1, yTop + 5 * dy, panelShader, buttonVAO);
        drawButton(ventTexture, col2, yTop + 5 * dy, panelShader, buttonVAO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}