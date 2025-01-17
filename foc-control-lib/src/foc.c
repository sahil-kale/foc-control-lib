#include "foc.h"

#include <stddef.h>

#include "foc_transforms.h"
#include "pid.h"

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
    foc_output_t ret;
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

        ret.v_ab = inverse_park_transform(ret.v_dq, foc_input.rotor_theta_rad);

        // TODO: SVM!
    }

    return ret;
}