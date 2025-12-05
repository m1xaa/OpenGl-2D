#include "Helper.h"

void addButton(float x, float y, int id, std::vector<Button>& buttons) {
    Button b;
    b.x = x;
    b.y = y;
    b.id = id;
    buttons.push_back(b);
};

float getFlatY(int flat, float FLAT_HEIGHT) {
    return -1.0f + (flat + 1) * FLAT_HEIGHT;
}

float compute_person_left_x_boundary(
    DoorState doorState,
    float LEFT_VERTICAL_LINE_X,
    float PERSON_WIDTH,
    bool person_in_elevator,
    float RIGHT_VERTICAL_LINE_X,
    bool sameFlat
) {
    float correction = PERSON_WIDTH / 2 - 0.05f;
    if (person_in_elevator && doorState != DOORS_OPEN) {
        return RIGHT_VERTICAL_LINE_X + correction;
    }

    return LEFT_VERTICAL_LINE_X + correction;

}

float compute_person_right_x_boundary(
    DoorState doorState,
    float PERSON_WIDTH,
    bool person_in_elevator,
    float RIGHT_VERTICAL_LINE_X,
    bool sameFlat
) {
    float correction = PERSON_WIDTH / 2 - 0.05f;
    if (!sameFlat) {
        return RIGHT_VERTICAL_LINE_X - correction;
    }
    
    if (person_in_elevator || doorState == DOORS_OPEN) {
        return 1.0f - correction;
    }

    return RIGHT_VERTICAL_LINE_X - correction;
}

void limitFPS(double& lastTimeForRefresh)
{
    double now = glfwGetTime();
    double targetFrameTime = 1.0 / FPS;
    double remaining = (lastTimeForRefresh + targetFrameTime) - now;

    if (remaining > 0.0)
    {
        std::this_thread::sleep_for(std::chrono::duration<double>(remaining));
    }
    else
    {
        glfwPollEvents();
    }

    lastTimeForRefresh = glfwGetTime();
}