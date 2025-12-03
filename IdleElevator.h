#pragma once
#include "ElevatorState.h"
class IdleElevator : public ElevatorState
{
public:
	void handle() override;
};

