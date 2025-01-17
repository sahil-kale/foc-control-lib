#include "CppUTest/TestHarness.h"

extern "C" {
#include "foc.h"
}

TEST_GROUP(Foc){void setup(){}

                void teardown(){}};

TEST(Foc, FocInit) {
    foc_data_t foc_data;
    foc_config_t foc_config = {
        .v_bus = 24.0F,
        .id_pid_config =
            {
                .kp = 1.0F,
                .ki = 0.0F,
                .kd = 0.0F,
                .dt = 1.0F / 24.0F,
            },
        .iq_pid_config =
            {
                .kp = 1.0F,
                .ki = 0.0F,
                .kd = 0.0F,
                .dt = 1.0F / 24.0F,
            },
        .id_pid_limits = NULL,
        .iq_pid_limits = NULL,
    };

    bool ret = foc_init(&foc_data, &foc_config);

    CHECK_TRUE(ret);
    CHECK_TRUE(foc_data.initialized);
    POINTERS_EQUAL(&foc_config, foc_data.config);
    DOUBLES_EQUAL(1.0F / 24.0F, foc_data.one_over_v_bus, 1e-3);

    CHECK_FALSE(foc_init(NULL, &foc_config));
    CHECK_FALSE(foc_init(&foc_data, NULL));
}

TEST(Foc, FocRunNullPointer) {
    foc_input_t foc_input = {
        .i_a = 1.0F,
        .i_b = 1.0F,
        .i_ref = {.d = 1.0F, .q = 1.0F},
        .rotor_theta_rad = 0.0F,
    };

    foc_output_t foc_output = foc_run(NULL, foc_input);

    CHECK_FALSE(foc_output.valid);

    // try with uninitialized foc_data
    foc_data_t foc_data;
    foc_data.initialized = false;

    foc_output = foc_run(&foc_data, foc_input);
    CHECK_FALSE(foc_output.valid);
}

TEST(Foc, ComputationalOutputs) {
    foc_input_t foc_input = {
        .i_a = 1.0F,
        .i_b = 1.0F,
        .i_ref = {.d = 1.0F, .q = 1.0F},
        .rotor_theta_rad = 3.14F / 3.0F,
    };

    foc_config_t foc_config = {
        .v_bus = 24.0F,
        .id_pid_config =
            {
                .kp = 1.0F,
                .ki = 0.0F,
                .kd = 0.0F,
                .dt = 1.0F / 20000.0F,
            },
        .iq_pid_config =
            {
                .kp = 1.0F,
                .ki = 0.0F,
                .kd = 0.0F,
                .dt = 1.0F / 20000.0F,
            },
        .id_pid_limits = NULL,
        .iq_pid_limits = NULL,
    };

    foc_data_t foc_data;

    foc_init(&foc_data, &foc_config);

    foc_output_t foc_output = foc_run(&foc_data, foc_input);
    CHECK_EQUAL(foc_output.valid, true);

    alpha_beta_t i_ab = clarke_transform(foc_input.i_a, foc_input.i_b);
    direct_quad_t i_dq = park_transform(i_ab, foc_input.rotor_theta_rad);

    // check the computational outputs
    DOUBLES_EQUAL(i_dq.d, foc_output.i_dq.d, 1e-3);
    DOUBLES_EQUAL(i_dq.q, foc_output.i_dq.q, 1e-3);

    // Blindly trust that Vdq is correct

    alpha_beta_t v_ab = inverse_park_transform(foc_output.v_dq, foc_input.rotor_theta_rad);
    DOUBLES_EQUAL(foc_output.v_ab.alpha, v_ab.alpha, 1e-3);
    DOUBLES_EQUAL(foc_output.v_ab.beta, v_ab.beta, 1e-3);
}