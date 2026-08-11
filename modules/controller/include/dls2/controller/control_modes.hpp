#pragma once

#include <stdint.h>

enum class ControlMode : uint8_t {
        TORQUE_MODE = 0,
		IMPEDANCE_MODE,
		POSITION_MODE,
		VELOCITY_MODE
};
