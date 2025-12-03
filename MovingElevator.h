#pragma once
#include "ElevatorState.h"
class MovingElevator : public ElevatorState
{
public:
	MovingElevator(int start, int target, int speed);
	void handle() override;

private:
	int startFlat;
	int targetFlat;
	int speed;
};

