#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void createButtonFrameVAO(unsigned int& VAO);
void createRectVAO(float* verticesRect, size_t rectSize, unsigned int& VAOrect);
void createElevatorVAO(float* vertices, size_t size, unsigned int& elevatorVAO);
void createButtonVAO(unsigned int& VAO);
void createLineVAO(unsigned int& lineVAO);