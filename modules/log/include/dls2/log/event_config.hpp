#ifndef EVENT_CONFIG_HPP
#define EVENT_CONFIG_HPP

#include <stdint.h>

enum class EventID : uint8_t {

    // Health anomalies (backend)
    WRONG_PROCESS_STATE = 0,
    WRONG_PROCESS_FREQUENCY,
    MISSING_INPUT,
    CPU_USAGE_TOO_HIGH,
    MEM_USAGE_TOO_HIGH,
    INPUTS_NOT_SYNCHRONIZED,
    WRONG_INPUT_FREQUENCY,
    MISSING_SEQUENCE_ID,

    // Safety anomalies (user)
    JOINT_POS_OUT_OF_BOUND,
    JOINT_VEL_OUT_OF_BOUND,
    JOINT_ACC_OUT_OF_BOUND,
    JOINT_TORQUE_OUT_OF_BOUND,
    REACHED_KINEMATIC_SINGULARITY,
    DETECTED_AUTOCOLLISION,
    NAN_VALUE,
    INF_VALUE,
    MOTOR_OVERHEAT
};

enum class EventSeverity : uint8_t {
    INFO = 0,       // Informational message
    WARNING,        // Warning message
    ERROR,          // Error message
    FATAL           // Fatal error message
};

#endif /* end of include guard: EVENT_IDS_HPP */