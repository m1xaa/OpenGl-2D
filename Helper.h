#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

enum DoorState {
    DOORS_CLOSED,
    DOORS_OPENING,
    DOORS_OPEN,
    DOORS_CLOSING
};

struct Button {
    float x, y;
    float w = 0.2f;
    float h = 0.2f;
    int id;
};
void addButton(float x, float y, int id, std::vector<Button>& buttons);
float getFlatY(int flat, float FLAT_HEIGHT);
float compute_person_left_x_boundary(
    DoorState doorState, 
    float LEFT_VERTICAL_LINE_X,
    float PERSON_WIDTH, 
    bool person_in_elevator, 
    float RIGHT_VERTICAL_LINE_X
);

float compute_person_right_x_boundary(
    DoorState doorState,
    float PERSON_WIDTH,
    bool person_in_elevator,
    float RIGHT_VERTICAL_LINE_X
);