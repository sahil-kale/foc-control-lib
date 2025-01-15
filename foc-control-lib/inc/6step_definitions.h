#ifndef SIX_STEP_DEFINITIONS_H
#define SIX_STEP_DEFINITIONS_H
#include <stdint.h>

/**
 * Rotor Convention (Defined as CCW from the y axis)
 * - 0 rad/degrees: Phase A
 * - 2pi/3 rad or 120 degrees: Phase B
 * - 4pi/3 rad or 240 degrees: Phase C
 *
 *
 * Rotor angle refers to the current angle in radians of the rotor.
 */

#define SIX_STEP_NUM_SECTORS 6U

typedef enum {
    SIX_STEP_CONTROL_ACTION_HIGH = 0,  /// Phase is 'HIGH' and connected to Vbus
    SIX_STEP_CONTROL_ACTION_LOW,       /// Phase is 'LOW' (0) and connected to GND
    SIX_STEP_CONTROL_ACTION_FLOAT,     /// Phase is 'FLOAT' (0) and not connected to anything
} six_step_phase_control_action_E;

typedef struct {
    six_step_phase_control_action_E phase_a_action;  /// Phase A control action
    six_step_phase_control_action_E phase_b_action;  /// Phase B control action
    six_step_phase_control_action_E phase_c_action;  /// Phase C control action
} six_step_sector_control_t;

// 2 separate tables for positive and negative speed. It's possible to only use one table, but this is more readable and efficient
// at the expense of a tiny bit of flash memory

extern const six_step_sector_control_t six_step_sector_positive_control_table[SIX_STEP_NUM_SECTORS];
extern const six_step_sector_control_t six_step_sector_negative_control_table[SIX_STEP_NUM_SECTORS];

#endif  // SIX_STEP_DEFINITIONS_H