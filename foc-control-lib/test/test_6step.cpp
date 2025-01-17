#include "CppUTest/TestHarness.h"

extern "C" {
#include "6step.h"
#include "util.h"
}

TEST_GROUP(SixStep){void setup() override{}

                    void teardown() override{}};

void check_phase_command_correct(six_step_phase_control_action_E control_action, six_step_phase_command_t phase_command,
                                 float speed_cmd) {
    const float phase_duty_cycle = speed_cmd / 2.0F * 0.5F;  // -1 -> 0, 1 -> 1,
    switch (control_action) {
        case SIX_STEP_CONTROL_ACTION_HIGH:
            CHECK_EQUAL(true, phase_command.phase_enabled);
            CHECK_EQUAL(phase_duty_cycle, phase_command.duty_cycle_multiplier);
            break;
        case SIX_STEP_CONTROL_ACTION_LOW:
            CHECK_EQUAL(true, phase_command.phase_enabled);
            CHECK_EQUAL(0.0f, phase_command.duty_cycle_multiplier);
            break;
        case SIX_STEP_CONTROL_ACTION_FLOAT:
            CHECK_EQUAL(false, phase_command.phase_enabled);
            CHECK_EQUAL(0.0F, phase_command.duty_cycle_multiplier);
            break;
        default:
            // wtf?
            CHECK(false);
            break;
    }
}

TEST(SixStep, SectorLookup) {
    // Test the sector lookup table
    CHECK_EQUAL(0, six_step_get_sector(DEG_TO_RAD(0.0f)));
    CHECK_EQUAL(0, six_step_get_sector(DEG_TO_RAD(29.9f)));
    CHECK_EQUAL(1, six_step_get_sector(DEG_TO_RAD(30.0f)));
    CHECK_EQUAL(1, six_step_get_sector(DEG_TO_RAD(89.9f)));
    CHECK_EQUAL(2, six_step_get_sector(DEG_TO_RAD(90.0f)));
    CHECK_EQUAL(2, six_step_get_sector(DEG_TO_RAD(149.9f)));
    CHECK_EQUAL(3, six_step_get_sector(DEG_TO_RAD(150.0f)));
    CHECK_EQUAL(3, six_step_get_sector(DEG_TO_RAD(209.9f)));
    CHECK_EQUAL(4, six_step_get_sector(DEG_TO_RAD(210.0f)));
    CHECK_EQUAL(4, six_step_get_sector(DEG_TO_RAD(269.9f)));
    CHECK_EQUAL(5, six_step_get_sector(DEG_TO_RAD(270.0f)));
    CHECK_EQUAL(5, six_step_get_sector(DEG_TO_RAD(329.9f)));
    CHECK_EQUAL(0, six_step_get_sector(DEG_TO_RAD(330.0f)));
    CHECK_EQUAL(0, six_step_get_sector(DEG_TO_RAD(359.0f)));
}

TEST(SixStep, SixStepSectorDutyCycleSanity) {
    for (uint8_t i = 0; i < SIX_STEP_NUM_SECTORS; i++) {
        const float test_angle = DEG_TO_RAD(60.0f * i);

        six_step_duty_cycles_t six_step_data = six_step_get_duty_cycle(test_angle, 0.5f);
        six_step_sector_control_t six_step_positive_control = six_step_sector_positive_control_table[i];

        check_phase_command_correct(six_step_positive_control.phase_a_action, six_step_data.phase_a, 0.5f);
        check_phase_command_correct(six_step_positive_control.phase_b_action, six_step_data.phase_b, 0.5f);
        check_phase_command_correct(six_step_positive_control.phase_c_action, six_step_data.phase_c, 0.5f);

        six_step_data = six_step_get_duty_cycle(test_angle, 1.2f);
        check_phase_command_correct(six_step_positive_control.phase_a_action, six_step_data.phase_a, 1.0f);
        check_phase_command_correct(six_step_positive_control.phase_b_action, six_step_data.phase_b, 1.0f);
        check_phase_command_correct(six_step_positive_control.phase_c_action, six_step_data.phase_c, 1.0f);

        six_step_data = six_step_get_duty_cycle(test_angle, -0.5f);
        six_step_sector_control_t six_step_negative_control = six_step_sector_negative_control_table[i];

        check_phase_command_correct(six_step_negative_control.phase_a_action, six_step_data.phase_a, 0.5f);
        check_phase_command_correct(six_step_negative_control.phase_b_action, six_step_data.phase_b, 0.5f);
        check_phase_command_correct(six_step_negative_control.phase_c_action, six_step_data.phase_c, 0.5f);

        six_step_data = six_step_get_duty_cycle(test_angle, -1.2f);
        check_phase_command_correct(six_step_negative_control.phase_a_action, six_step_data.phase_a, 1.0f);
        check_phase_command_correct(six_step_negative_control.phase_b_action, six_step_data.phase_b, 1.0f);
        check_phase_command_correct(six_step_negative_control.phase_c_action, six_step_data.phase_c, 1.0f);
    }
}
