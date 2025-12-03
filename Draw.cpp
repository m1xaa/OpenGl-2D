#include "Draw.h"

void drawElevator(unsigned int elevatorShader, unsigned int elevatorVAO, float uY, float doorsuY) {
    glUseProgram(elevatorShader);
    glBindVertexArray(elevatorVAO);
    glUniform1f(glGetUniformLocation(elevatorShader, "uY"), uY);
    glUniform1f(glGetUniformLocation(elevatorShader, "doorsuY"), doorsuY);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
}

void drawRect(unsigned int rectShader, unsigned int VAOrect) {
    glUseProgram(rectShader);
    glBindVertexArray(VAOrect);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawPerson(unsigned int glisaShader, unsigned int VAOglisa, float uX, float personuY) {
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

void drawLine(float x, float y, float w, float h, unsigned int shader, unsigned int lineVAO)
{
    glUseProgram(shader);

    glUniform2f(glGetUniformLocation(shader, "uOffset"), x, y);
    glUniform2f(glGetUniformLocation(shader, "uScale"), w, h);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 1.0f, 1.0f, 1.0f);

    glBindVertexArray(lineVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawHorizontalLines(float leftX, float rightX, unsigned int shader, unsigned int& lineVAO, int NUM_FLATS, float FLAT_HEIGHT, float LINE_THICKNESS)
{
    float centerX = (leftX + rightX) / 2.0f;

    float w = (rightX - leftX);

    for (int i = 0; i < NUM_FLATS; i++)
    {
        float y = -1.0f + i * FLAT_HEIGHT;
        drawLine(centerX, y, w, LINE_THICKNESS, shader, lineVAO);
    }
}

void drawButtonFrame(float x, float y, unsigned int shader, unsigned int& buttonFrameVAO)
{
    glUseProgram(shader);

    glUniform2f(glGetUniformLocation(shader, "uOffset"), x, y);
    glUniform2f(glGetUniformLocation(shader, "uScale"), 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 1.0f, 1.0f, 1.0f);

    glBindVertexArray(buttonFrameVAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}