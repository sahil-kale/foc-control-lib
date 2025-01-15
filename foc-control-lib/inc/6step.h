#ifndef SIX_STEP_HPP
#define SIX_STEP_HPP
#include <stdbool.h>

#include "6step_definitions.h"

typedef struct {
    bool phase_enabled;           /// True if the phase is enabled, false otherwise (High-Z)
    float duty_cycle_multiplier;  /// 0.0 to 1.0, where 0.5 corrosponds to 0V; 0.0 is -BusV/2, 1.0 is +BusV/2
} six_step_phase_command_t;

typedef struct {
    six_step_phase_command_t phase_a;  /// Phase A command
    six_step_phase_command_t phase_b;  /// Phase B command
    six_step_phase_command_t phase_c;  /// Phase C command
} six_step_duty_cycles_t;

/**
 * @brief Get the sector of the rotor based on the rotor angle
 *
 * @param rotor_angle_rad The rotor angle in radians.
 *
 * @return uint8_t The sector of the rotor
 */
uint8_t six_step_get_sector(float rotor_angle_rad);

/**
 * @brief Get the duty cycles for the 3 phases based on the rotor angle and speed
 *
 * @param rotor_angle_rad The rotor angle in radians
 *
 * @param speed The speed of the rotor in the range of -1.0 to 1.0
 */
six_step_duty_cycles_t six_step_get_duty_cycle(float rotor_angle_rad, float speed);

#endif  // SIX_STEP_HPP