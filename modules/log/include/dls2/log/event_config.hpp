#ifndef EVENT_CONFIG_HPP
#define EVENT_CONFIG_HPP

#include <stdint.h>

enum class EventID : uint8_t {
    // Health anomalies
    WRONG_PROCESS_STATE = 0,
    WRONG_PROCESS_FREQUENCY,
    CPU_THROTTLING,
    MISSING_INPUT,
    LOW_FREQUENCY_INPUT,
    MISSING_SEQUENCE_ID,
    MOTOR_OVERHEAT,

    // Safety anomalies
    JOINT_POS_OUT_OF_BOUND,
    JOINT_VEL_OUT_OF_BOUND,
    JOINT_ACC_OUT_OF_BOUND,
    JOINT_TORQUE_OUT_OF_BOUND,
    REACHED_KINEMATIC_SINGULARITY,
    DETECTED_AUTOCOLLISION,
    NAN_VALUE,
    TOO_HIGH_VALUE,
    INPUTS_NOT_SYNCHRONIZED,
};

enum class EventSeverity : uint8_t {
    INFO = 0,       // Informational message
    WARNING,        // Warning message
    ERROR,          // Error message
    FATAL           // Fatal error message
};

#endif /* end of include guard: EVENT_IDS_HPP */