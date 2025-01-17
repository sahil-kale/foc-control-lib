#ifndef THREE_PHASE_TYPES_H
#define THREE_PHASE_TYPES_H

#include <stdbool.h>

typedef struct {
    bool phase_enabled;           /// True if the phase is enabled, false otherwise (High-Z)
    float duty_cycle_multiplier;  /// 0.0 to 1.0, where 0.5 corresponds to 0V; 0.0 is -BusV/2, 1.0 is +BusV/2
} motor_phase_command_t;

#endif  // THREE_PHASE_TYPES_H