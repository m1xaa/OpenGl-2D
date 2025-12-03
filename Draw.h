#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void drawElevator(unsigned int elevatorShader, unsigned int elevatorVAO, float uY, float doorsuY);
void drawRect(unsigned int rectShader, unsigned int VAOrect);
void drawPerson(unsigned int glisaShader, unsigned int VAOglisa, float uX, float personuY);
void drawButton(unsigned int texture, float x, float y, unsigned int panelShader, unsigned int buttonVAO);
void drawLine(float x, float y, float w, float h, unsigned int shader, unsigned int lineVAO);
void drawHorizontalLines(float leftX, float rightX, unsigned int shader, unsigned int& lineVAO, int NUM_FLATS, float FLAT_HEIGHT, float LINE_THICKNESS);
void drawButtonFrame(float x, float y, unsigned int shader, unsigned int& buttonFrameVAO);