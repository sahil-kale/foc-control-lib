#ifndef SIX_STEP_HPP
#define SIX_STEP_HPP
#include <stdbool.h>

#include "3phase_types.h"
#include "6step_definitions.h"

typedef struct {
    motor_phase_command_t phase_a;  /// Phase A command
    motor_phase_command_t phase_b;  /// Phase B command
    motor_phase_command_t phase_c;  /// Phase C command
} six_step_duty_cycles_t;

typedef struct {
    bool valid;
    float rotor_el_angle_rad;
} six_step_sensed_rotor_angle_from_hall_t;

/**
 * @brief Get the sector of the rotor based on the rotor angle
 *
 * @param rotor_el_angle_rad The rotor electrical angle in radians.
 *
 * @return uint8_t The sector of the rotor
 */
uint8_t six_step_get_sector(float rotor_el_angle_rad);

/**
 * @brief Get the duty cycles for the 3 phases based on the rotor angle and speed
 *
 * @param rotor_el_angle_rad The rotor electrical angle in radians
 *
 * @param speed The speed of the rotor in the range of -1.0 to 1.0
 */
six_step_duty_cycles_t six_step_get_duty_cycle(float rotor_el_angle_rad, float speed);

/**
 * @brief Get the sensed rotor angle based on the hall sensor inputs
 *
 * @param hall_a The state of hall sensor A (in line with phase A)
 * @param hall_b The state of hall sensor B (in line with phase B)
 * @param hall_c The state of hall sensor C (in line with phase C)
 *
 * @return six_step_sensed_rotor_angle_from_hall_t Results of the rotor angle sensing
 */
six_step_sensed_rotor_angle_from_hall_t six_step_get_rotor_angle_from_hall_sensor(bool hall_a, bool hall_b, bool hall_c);

#endif  // SIX_STEP_HPP