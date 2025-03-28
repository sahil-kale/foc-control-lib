#include "foc.h"

#include <stddef.h>

#include "foc_transforms.h"
#include "pid.h"

static const foc_output_t foc_default_output = {
    .valid = false,
    .phase_a = {.phase_enabled = false, .duty_cycle_multiplier = 0.0F},
    .phase_b = {.phase_enabled = false, .duty_cycle_multiplier = 0.0F},
    .phase_c = {.phase_enabled = false, .duty_cycle_multiplier = 0.0F},
    .i_dq = {.d = 0.0F, .q = 0.0F},
    .v_dq = {.d = 0.0F, .q = 0.0F},
    .v_ab = {.alpha = 0.0F, .beta = 0.0F},
};

bool foc_init(foc_data_t *foc_data, foc_config_t const *const foc_config) {
    bool ret = (foc_data != NULL) && (foc_config != NULL);

    if (ret) {
        foc_data->config = foc_config;
        foc_data->one_over_v_bus = 1.0F / foc_config->v_bus;

        ret &= control_utils_pid_init(&foc_data->id_pid, &foc_config->id_pid_config, foc_config->id_pid_limits);
        ret &= control_utils_pid_init(&foc_data->iq_pid, &foc_config->iq_pid_config, foc_config->iq_pid_limits);

        foc_data->initialized = true;
    }

    return ret;
}

foc_output_t foc_run(foc_data_t *const foc_data, foc_input_t foc_input) {
    foc_output_t ret = foc_default_output;
    ret.valid = (foc_data != NULL) && (foc_data->initialized);

    if (ret.valid) {
        alpha_beta_t i_ab = clarke_transform(foc_input.i_a, foc_input.i_b);
        ret.i_dq = park_transform(i_ab, foc_input.rotor_theta_rad);

        const float i_q_error = foc_input.i_ref.q - ret.i_dq.q;
        const float i_d_error = foc_input.i_ref.d - ret.i_dq.d;

        ret.valid &= control_utils_pid_run(i_q_error, &foc_data->iq_pid);
        ret.valid &= control_utils_pid_run(i_d_error, &foc_data->id_pid);

        ret.v_dq.q = foc_data->iq_pid.output;
        ret.v_dq.d = foc_data->id_pid.output;

        // TODO: Implement saturator here

        ret.v_ab = inverse_park_transform(ret.v_dq, foc_input.rotor_theta_rad);

        // TODO: Use SVM to generate the phase commands, rather than sine
        abc_t V_abc = inverse_clarke_transform(ret.v_ab);

        ret.phase_a.duty_cycle_multiplier = foc_convert_inverse_sine_voltages_to_duty_cycle(V_abc.a, foc_data->one_over_v_bus);
        ret.phase_b.duty_cycle_multiplier = foc_convert_inverse_sine_voltages_to_duty_cycle(V_abc.b, foc_data->one_over_v_bus);
        ret.phase_c.duty_cycle_multiplier = foc_convert_inverse_sine_voltages_to_duty_cycle(V_abc.c, foc_data->one_over_v_bus);

        ret.phase_a.phase_enabled = ret.valid;
        ret.phase_b.phase_enabled = ret.valid;
        ret.phase_c.phase_enabled = ret.valid;
    }

    return ret;
}

float foc_convert_inverse_sine_voltages_to_duty_cycle(float phase_voltage, float one_over_v_bus) {
    // Max phase voltage from inverse clarke (sine pwm) is +- 0.5Vbus
    // A voltage of 0*Vbus is 0.5 duty cycle (corrosponding to '0V') phase voltage
    // A voltage of 0.5*Vbus is 1.0 duty cycle (corrosponding to 'Vbus') phase voltage
    // A voltage of -0.5*Vbus is 0.0 duty cycle (corrosponding to '-Vbus') phase voltage
    return (phase_voltage * one_over_v_bus + 0.5F);
}
