#include "6step.h"

#include <math.h>
#include <stdint.h>

#include "6step_definitions.h"
#include "util.h"

static void populate_phase_command(six_step_phase_command_t *phase_command, six_step_phase_control_action_E control_action,
                                   float speed_cmd) {
    const float phase_duty_cycle = speed_cmd / 2.0F * 0.5F;  // -1 -> 0, 1 -> 1,
    switch (control_action) {
        case SIX_STEP_CONTROL_ACTION_HIGH:
            phase_command->phase_enabled = true;
            phase_command->duty_cycle_multiplier = phase_duty_cycle;
            break;
        case SIX_STEP_CONTROL_ACTION_LOW:
            phase_command->phase_enabled = true;
            phase_command->duty_cycle_multiplier = 0.0F;
            break;
        case SIX_STEP_CONTROL_ACTION_FLOAT:
        default:
            phase_command->phase_enabled = false;
            phase_command->duty_cycle_multiplier = 0.0F;
            break;
    }
}

uint8_t six_step_get_sector(float rotor_angle_rad) {
    rotor_angle_rad = fmodf(rotor_angle_rad, 2.0F * M_PI_FLOAT);  // Todo: investigate loop time performance of fmodf

    uint8_t sector = 0;
    if ((rotor_angle_rad >= DEG_TO_RAD(330.0F)) || (rotor_angle_rad < DEG_TO_RAD(30.0F))) {
        sector = 0;
    } else if (rotor_angle_rad < DEG_TO_RAD(90.0F)) {
        sector = 1;
    } else if (rotor_angle_rad < DEG_TO_RAD(150.0F)) {
        sector = 2;
    } else if (rotor_angle_rad < DEG_TO_RAD(210.0F)) {
        sector = 3;
    } else if (rotor_angle_rad < DEG_TO_RAD(270.0F)) {
        sector = 4;
    } else {
        sector = 5;
    }

    return sector;
}

six_step_duty_cycles_t six_step_get_duty_cycle(float rotor_angle_rad, float speed) {
    const uint8_t sector = six_step_get_sector(rotor_angle_rad);
    speed = CLAMP(speed, -1.0F, 1.0F);
    const float abs_speed = ABS_MACRO(speed);

    const six_step_sector_control_t sector_control =
        speed > 0.0F ? six_step_sector_positive_control_table[sector] : six_step_sector_negative_control_table[sector];

    six_step_duty_cycles_t duty_cycles;
    populate_phase_command(&duty_cycles.phase_a, sector_control.phase_a_action, abs_speed);
    populate_phase_command(&duty_cycles.phase_b, sector_control.phase_b_action, abs_speed);
    populate_phase_command(&duty_cycles.phase_c, sector_control.phase_c_action, abs_speed);

    return duty_cycles;
}