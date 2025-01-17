#ifndef FOC_H
#define FOC_H

#include "3phase_types.h"
#include "foc_transforms.h"
#include "pid.h"

typedef struct {
    float v_bus;
    control_utils_pid_config_t id_pid_config;
    control_utils_pid_config_t iq_pid_config;

    control_utils_pid_limits_t const *id_pid_limits;
    control_utils_pid_limits_t const *iq_pid_limits;

} foc_config_t;

typedef struct {
    foc_config_t const *config;

    control_utils_pid_data_t id_pid;
    control_utils_pid_data_t iq_pid;

    bool initialized;
    float one_over_v_bus;
} foc_data_t;

typedef struct {
    float i_a;  /// Phase A current (note: i_c = - i_a - i_b)
    float i_b;  /// Phase B current (note: i_c = - i_a - i_b)

    direct_quad_t i_ref;  /// Reference current in direct-quadrature frame

    float rotor_theta_rad;  /// Rotor angle in radians, measured from the alpha axis to the d-axis
} foc_input_t;

typedef struct {
    bool valid;

    // Motor Outputs
    motor_phase_command_t phase_a;
    motor_phase_command_t phase_b;
    motor_phase_command_t phase_c;

    // Computational Outputs
    direct_quad_t i_dq;
    direct_quad_t v_dq;
    alpha_beta_t v_ab;
} foc_output_t;

/**
 * @brief Initialize the FOC data structure
 *
 * @param foc_data Pointer to the FOC data structure
 * @param foc_config Pointer to the FOC configuration structure
 *
 * @return true if the initialization was successful
 */
bool foc_init(foc_data_t *const foc_data, foc_config_t const *const foc_config);

/**
 * @brief Run the FOC algorithm
 *
 * @param foc_data Pointer to the FOC data structure
 * @param foc_input FOC input structures
 *
 * @note Call this function at the control loop rate - this function is considered to be in the "hot" path
 * @return foc_output_t The FOC output structure
 */
foc_output_t foc_run(foc_data_t *foc_data, foc_input_t foc_input);

#endif  // FOC_H