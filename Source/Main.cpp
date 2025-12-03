#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <queue>

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
#define ELEVATOR_SPEED 0.0005f


unsigned int indexTexture;
unsigned int suTexture, prTexture, firstTexture, secondTexture, thirdTexture, fourthTexture, fifthTexture, sixthTexture;
unsigned int openTexture, closeTexture, stopTexture, ventTexture;
unsigned int glisaTexture;

int elevator_current_flat = 2;   //flats - 0 1 2 3 4 5 6 7
int person_current_flat = 1;

bool elevatorMoving = false;
int elevatorTargetFlat = -1;

float uX = 0.0f;
float personuY = -3 * FLAT_HEIGHT + PERSON_HEIGHT;
float uY = -FLAT_HEIGHT;
float elevatorX = RIGHT_VERTICAL_LINE_X;

bool person_in_elevator = false;
enum DoorState {
    DOORS_CLOSED,
    DOORS_OPENING,
    DOORS_OPEN,
    DOORS_CLOSING
};

DoorState doorState = DOORS_CLOSED;

float doorsuY = 0.0f;        
float DOOR_SPEED = 0.0007f;     
float doorOpenStart = 0.0f;    
float doorOpenDuration = 5.0f; 
bool doorExtendedOnce = false;

std::queue<int> elevatorFlats = {};

struct Button {
    float x, y;         
    float w = 0.2f;     
    float h = 0.2f;     
    int id;            
};

std::vector<Button> buttons;

void addButton(float x, float y, int id) {
    Button b;
    b.x = x;
    b.y = y;
    b.id = id;
    buttons.push_back(b);
};

float getFlatY(int flat) {
    return -1.0f + (flat+1) * FLAT_HEIGHT + (flat-1) * LINE_THICKNESS;
}

float compute_person_left_x_boundary() {
    if (doorState == DOORS_OPEN) {
        return LEFT_VERTICAL_LINE_X + PERSON_WIDTH / 2 - 0.05f;
    }
    if (!person_in_elevator) {
        return LEFT_VERTICAL_LINE_X + PERSON_WIDTH / 2 - 0.05f;
    }
    else {
        return RIGHT_VERTICAL_LINE_X + PERSON_WIDTH / 2 - 0.05f;
    }
}

float compute_person_right_x_boundary() {
    if (doorState == DOORS_OPEN) {
        return 1.0f - PERSON_WIDTH / 2 + 0.05f;
    }
    if (person_in_elevator) {
        return 1.0f - PERSON_WIDTH / 2 + 0.05f;
    }
    else {
        return RIGHT_VERTICAL_LINE_X - PERSON_WIDTH / 2 + 0.05f;
    }
}


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
    glUniform1f(glGetUniformLocation(elevatorShader, "uY"), uY);
    glUniform1f(glGetUniformLocation(elevatorShader, "doorsuY"), doorsuY);
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
    glUniform1f(glGetUniformLocation(glisaShader, "uY"), personuY);
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
void move_doors() {

    if (doorState == DOORS_OPENING) {
        doorsuY += DOOR_SPEED;
        if (doorsuY >= FLAT_HEIGHT) {
            doorsuY = FLAT_HEIGHT;
            doorState = DOORS_OPEN;
            doorOpenStart = glfwGetTime();
            doorExtendedOnce = false;
        }
    }

    else if (doorState == DOORS_OPEN) {
        if (glfwGetTime() - doorOpenStart >= doorOpenDuration) {
            doorState = DOORS_CLOSING;
        }
    }

    else if (doorState == DOORS_CLOSING) {
        doorsuY -= DOOR_SPEED;
        if (doorsuY <= 0.0f) {
            doorsuY = 0.0f;
            doorState = DOORS_CLOSED;
            elevatorMoving = false;
        }
    }
}


void move_elevator() {

    if (elevatorFlats.empty() || doorState != DOORS_CLOSED) {
        elevatorMoving = false;
        return;
    }

    if (!elevatorMoving) {
        elevatorTargetFlat = elevatorFlats.front();
        elevatorMoving = true;
    }

    float targetY = getFlatY(elevatorTargetFlat);

    if (uY < targetY) {
        if (person_in_elevator) {
            personuY += ELEVATOR_SPEED;
        }
        uY += ELEVATOR_SPEED;
        if (uY > targetY) uY = targetY;
    }
    else if (uY > targetY) {
        if (person_in_elevator) {
            personuY -= ELEVATOR_SPEED;
        }
        uY -= ELEVATOR_SPEED;
        if (uY < targetY) uY = targetY;
    }

    if (fabs(uY - targetY) < 0.0001f) {
        uY = targetY;
        elevator_current_flat = elevatorTargetFlat;
        elevatorFlats.pop();
        elevatorMoving = false;

        doorState = DOORS_OPENING;
    }

}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        if (!person_in_elevator && doorState == DOORS_CLOSED && uX == compute_person_right_x_boundary()) {
            if (elevator_current_flat != person_current_flat) {
                elevatorFlats.push(person_current_flat);
            }
            else {
                doorState = DOORS_OPENING;
            }
        }
    }
}

void handle_button_click(int id)
{
    std::cout << "Kliknut button id: " << id << std::endl;

    if (!person_in_elevator) return;

    switch (id) {

    case 0: elevatorFlats.push(0); break; // SU
    case 1: elevatorFlats.push(1); break;  // PR
    case 2: elevatorFlats.push(2); break;  // 1
    case 3: elevatorFlats.push(3); break;  // 2
    case 4: elevatorFlats.push(4); break;  // 3
    case 5: elevatorFlats.push(5); break;  // 4
    case 6: elevatorFlats.push(6); break;  // 5
    case 7: elevatorFlats.push(7); break;  // 6

    case 8: // OPEN
        if (doorState == DOORS_CLOSED) doorState = DOORS_OPENING;
        else if (doorState == DOORS_OPEN && !doorExtendedOnce) {
            doorOpenStart = glfwGetTime();
            doorExtendedOnce = true;
        }
        break;

    case 9: // CLOSE
        if (doorState == DOORS_OPEN || doorState == DOORS_OPENING)
            doorState = DOORS_CLOSING;
        break;

    case 10: // STOP
        break;

    case 11: // VENT
        break;
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (!person_in_elevator) {
        return;
    }

    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
        return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float mx = (float)xpos / width * 2.0f - 1.0f;
    float my = -(float)ypos / height * 2.0f + 1.0f;

    for (auto& b : buttons)
    {
        bool inside =
            mx >= b.x - b.w / 2 && mx <= b.x + b.w / 2 &&
            my >= b.y - b.h / 2 && my <= b.y + b.h / 2;

        if (inside)
        {
            handle_button_click(b.id);
            break;
        }
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
    PERSON_START_X - PERSON_WIDTH / 2,  0, 0.0f, 1.0f,
    PERSON_START_X - PERSON_WIDTH / 2,  -PERSON_HEIGHT, 0.0f, 0.0f,
    PERSON_START_X + PERSON_WIDTH / 2,  -PERSON_HEIGHT, 1.0f, 0.0f,
    PERSON_START_X + PERSON_WIDTH / 2,  0, 1.0f, 1.0f
    };

    unsigned int VAOglisa;
    formVAOs(glisaRect, sizeof(glisaRect), VAOglisa);
    preprocessTexture(glisaTexture, "Resources/glisa.png");
    glUseProgram(glisaShader);
    glUniform1i(glGetUniformLocation(glisaShader, "tex"), 0);

    float elevatorRect[] = {
        elevatorX, 0.0f,                        1,0,0, 0,
        elevatorX, -LINE_THICKNESS,             1,0,0, 0,
        1.0f,      -LINE_THICKNESS,             1,0,0, 0,
        1.0f,       0.0f,                       1,0,0, 0,

        elevatorX, -LINE_THICKNESS,             0,0,0, 1,
        elevatorX, -FLAT_HEIGHT,                0,0,0, 1,
        1.0f,      -FLAT_HEIGHT,                0,0,0, 1,
        1.0f,      -LINE_THICKNESS,             0,0,0, 1,

        elevatorX, -FLAT_HEIGHT,                1,0,0, 0,
        elevatorX, -FLAT_HEIGHT - LINE_THICKNESS, 1,0,0, 0,
        1.0f,      -FLAT_HEIGHT - LINE_THICKNESS, 1,0,0, 0,
        1.0f,      -FLAT_HEIGHT,                1,0,0, 0,
    };



    unsigned int VAOelevator;
    formElevatorVAOs(elevatorRect, sizeof(elevatorRect), VAOelevator);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    

    while (!glfwWindowShouldClose(window))
    {
        move_elevator();
        move_doors();
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            uX -= 0.001f;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            uX += 0.001f;
        }

        float right_bounadry = compute_person_right_x_boundary();
        float left_boundary = compute_person_left_x_boundary();
        if (uX > right_bounadry) uX = right_bounadry;
        if (uX < left_boundary) uX = left_boundary;

        if (doorState == DOORS_OPEN) {


            if (uX > RIGHT_VERTICAL_LINE_X && !person_in_elevator) {
                person_in_elevator = true;
                person_current_flat = elevator_current_flat;
            }

            if (uX < RIGHT_VERTICAL_LINE_X && person_in_elevator) {
                person_in_elevator = false;
                person_current_flat = elevator_current_flat;
            }
        }



        glClear(GL_COLOR_BUFFER_BIT); 

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, indexTexture);

        drawRect(indexShader, VAOrect);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, glisaTexture);
        drawPerson(glisaShader, VAOglisa);

        drawElevator(elevatorShader, VAOelevator);

        buttons.clear();

        float col1 = -0.85f;
        float col2 = -0.65f;

        float yTop = 0.60f;
        float dy = -0.25f;

        int id = 0;

        addButton(col1, yTop, id++);  drawButton(suTexture, col1, yTop, panelShader, buttonVAO);
        addButton(col2, yTop, id++);  drawButton(prTexture, col2, yTop, panelShader, buttonVAO);

        addButton(col1, yTop + dy, id++);  drawButton(firstTexture, col1, yTop + dy, panelShader, buttonVAO);
        addButton(col2, yTop + dy, id++);  drawButton(secondTexture, col2, yTop + dy, panelShader, buttonVAO);

        addButton(col1, yTop + 2 * dy, id++); drawButton(thirdTexture, col1, yTop + 2 * dy, panelShader, buttonVAO);
        addButton(col2, yTop + 2 * dy, id++); drawButton(fourthTexture, col2, yTop + 2 * dy, panelShader, buttonVAO);

        addButton(col1, yTop + 3 * dy, id++); drawButton(fifthTexture, col1, yTop + 3 * dy, panelShader, buttonVAO);
        addButton(col2, yTop + 3 * dy, id++); drawButton(sixthTexture, col2, yTop + 3 * dy, panelShader, buttonVAO);

        addButton(col1, yTop + 4 * dy, id++); drawButton(openTexture, col1, yTop + 4 * dy, panelShader, buttonVAO);
        addButton(col2, yTop + 4 * dy, id++); drawButton(closeTexture, col2, yTop + 4 * dy, panelShader, buttonVAO);

        addButton(col1, yTop + 5 * dy, id++); drawButton(stopTexture, col1, yTop + 5 * dy, panelShader, buttonVAO);
        addButton(col2, yTop + 5 * dy, id++); drawButton(ventTexture, col2, yTop + 5 * dy, panelShader, buttonVAO);

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