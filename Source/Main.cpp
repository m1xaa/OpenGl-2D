#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Header/Util.h"

#define LEFT_VERTICAL_LINE_X -0.55F
#define RIGHT_VERTICAL_LINE_X 0.75f
#define LINE_THICKNESS 0.01f

#define NUM_FLATS 8
#define FLAT_HEIGHT 2.0f / NUM_FLATS 
#define PERSON_HEIGHT 0.8f * FLAT_HEIGHT
#define PERSON_WIDTH 0.2f
#define PERSON_START_X 0.0f
#define PERSON_START_Y -1.0f + 2*LINE_THICKNESS + FLAT_HEIGHT


unsigned int indexTexture;
unsigned int suTexture, prTexture, firstTexture, secondTexture, thirdTexture, fourthTexture, fifthTexture, sixthTexture;
unsigned int openTexture, closeTexture, stopTexture, ventTexture;
unsigned int glisaTexture;

float uX = 0.0f;
float elevatorY = -1 +  3 * FLAT_HEIGHT + 1 * LINE_THICKNESS;
float elevatorX = RIGHT_VERTICAL_LINE_X;


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

void formElevatorVAOs(float* vertices, size_t size, unsigned int& elevatorVAO) {
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

void drawElevator(unsigned int elevatorShader, unsigned int elevatorVAO) {
    glUseProgram(elevatorShader);
    glBindVertexArray(elevatorVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);   
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);  
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
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

void drawPerson(unsigned int glisaShader, unsigned int VAOglisa) {
    glUseProgram(glisaShader);
    glBindVertexArray(VAOglisa);
    glUniform1f(glGetUniformLocation(glisaShader, "uX"), uX);
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

void drawLine(float x, float y, float w, float h, unsigned int shader, unsigned int lineVAO)
{
    glUseProgram(shader);

    glUniform2f(glGetUniformLocation(shader, "uOffset"), x, y);
    glUniform2f(glGetUniformLocation(shader, "uScale"), w, h);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 1.0f, 1.0f, 1.0f);

    glBindVertexArray(lineVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawHorizontalLines(float leftX, float rightX, unsigned int shader, unsigned int& lineVAO) 
{
    float centerX = (leftX + rightX) / 2.0f;

    float w = (rightX - leftX);

    for (int i = 0; i < NUM_FLATS; i++)
    {
        float y = -1.0f + i * FLAT_HEIGHT;
        drawLine(centerX, y, w, LINE_THICKNESS, shader, lineVAO);
    }
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
    unsigned int lineShader = createShader("line.vert", "line.frag");
    unsigned int glisaShader = createShader("person.vert", "person.frag");
    unsigned int elevatorShader = createShader("elevator.vert", "elevator.frag");

    unsigned int lineVAO;
    createLineVAO(lineVAO);

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

    float glisaRect[] = {
    PERSON_START_X - PERSON_WIDTH / 2,  PERSON_START_Y + PERSON_HEIGHT, 0.0f, 1.0f,
    PERSON_START_X - PERSON_WIDTH / 2,  PERSON_START_Y, 0.0f, 0.0f,
    PERSON_START_X + PERSON_WIDTH / 2,  PERSON_START_Y, 1.0f, 0.0f,
    PERSON_START_X + PERSON_WIDTH / 2,  PERSON_START_Y + PERSON_HEIGHT, 1.0f, 1.0f
    };

    unsigned int VAOglisa;
    formVAOs(glisaRect, sizeof(glisaRect), VAOglisa);
    preprocessTexture(glisaTexture, "Resources/glisa.png");
    glUseProgram(glisaShader);
    glUniform1i(glGetUniformLocation(glisaShader, "tex"), 0);

    float elevatorRect[] = {
    elevatorX, elevatorY, 1,0,0, 0,
    elevatorX, elevatorY - LINE_THICKNESS, 1,0,0, 0,
    1, elevatorY - LINE_THICKNESS, 1,0,0, 0,
    1, elevatorY, 1,0,0, 0,

    elevatorX, elevatorY - LINE_THICKNESS, 0,0,0, 1,
    elevatorX, elevatorY - FLAT_HEIGHT - LINE_THICKNESS, 0,0,0, 1,
    1, elevatorY - FLAT_HEIGHT - LINE_THICKNESS, 0,0,0, 1,
    1, elevatorY - LINE_THICKNESS, 0,0,0, 1,

    elevatorX, elevatorY - FLAT_HEIGHT - LINE_THICKNESS, 1,0,0, 0,
    elevatorX, elevatorY - FLAT_HEIGHT - 2*LINE_THICKNESS, 1,0,0, 0,
    1, elevatorY - FLAT_HEIGHT - 2*LINE_THICKNESS, 1,0,0, 0,
    1, elevatorY - FLAT_HEIGHT - LINE_THICKNESS, 1,0,0, 0,

    };

    unsigned int VAOelevator;
    formElevatorVAOs(elevatorRect, sizeof(elevatorRect), VAOelevator);

    glfwSetKeyCallback(window, key_callback);
    

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            uX -= 0.001f;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            uX += 0.001f;
        }

        float margin = 0.05f;
        if (uX - PERSON_WIDTH / 2 + margin < LEFT_VERTICAL_LINE_X)
            uX = LEFT_VERTICAL_LINE_X + PERSON_WIDTH / 2 - margin;
        if (uX + PERSON_WIDTH / 2 - margin > RIGHT_VERTICAL_LINE_X)
            uX = RIGHT_VERTICAL_LINE_X - PERSON_WIDTH / 2 + margin;

        glClear(GL_COLOR_BUFFER_BIT); 

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, indexTexture);

        drawRect(indexShader, VAOrect);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, glisaTexture);
        drawPerson(glisaShader, VAOglisa);

        drawElevator(elevatorShader, VAOelevator);

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

        drawLine(LEFT_VERTICAL_LINE_X, 0.0f, LINE_THICKNESS, 1.0f, lineShader, lineVAO);

        drawLine(RIGHT_VERTICAL_LINE_X, 0.0f, LINE_THICKNESS, 1.0f, lineShader, lineVAO);

        drawHorizontalLines(LEFT_VERTICAL_LINE_X, RIGHT_VERTICAL_LINE_X,lineShader, lineVAO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}