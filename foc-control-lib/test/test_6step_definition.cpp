#include "CppUTest/TestHarness.h"

extern "C" {
#include "6step_definitions.h"
}

TEST_GROUP(SixStepDefinitions){void setup() override{}

                               void teardown() override{}};

TEST(SixStepDefinitions, PhaseSanityCheckPositive) {
    six_step_sector_control_t prev_control = six_step_sector_positive_control_table[0];

    six_step_phase_control_action_E prev_control_array[3] = {prev_control.phase_a_action, prev_control.phase_b_action,
                                                             prev_control.phase_c_action};

    for (size_t i = 1; i < SIX_STEP_NUM_SECTORS; i++) {
        six_step_sector_control_t new_control = six_step_sector_positive_control_table[i];
        six_step_phase_control_action_E new_control_array[3] = {new_control.phase_a_action, new_control.phase_b_action,
                                                                new_control.phase_c_action};

        for (size_t j = 0; j < 3; j++) {
            // if the prev control action is HIGH, the new control action cannot be low
            // if the prev control action is LOW, the new control action cannot be high
            // if the prev control action is FLOAT, the new control action cannot be float

            if (prev_control_array[j] == SIX_STEP_CONTROL_ACTION_HIGH) {
                CHECK(new_control_array[j] != SIX_STEP_CONTROL_ACTION_LOW);
            } else if (prev_control_array[j] == SIX_STEP_CONTROL_ACTION_LOW) {
                CHECK(new_control_array[j] != SIX_STEP_CONTROL_ACTION_HIGH);
            } else if (prev_control_array[j] == SIX_STEP_CONTROL_ACTION_FLOAT) {
                CHECK(new_control_array[j] != SIX_STEP_CONTROL_ACTION_FLOAT);
            }

            prev_control_array[j] = new_control_array[j];
        }
    }
}

TEST(SixStepDefinitions, PhaseSanityCheckNegative) {
    six_step_sector_control_t prev_control = six_step_sector_negative_control_table[0];

    six_step_phase_control_action_E prev_control_array[3] = {prev_control.phase_a_action, prev_control.phase_b_action,
                                                             prev_control.phase_c_action};

    for (size_t i = 1; i < SIX_STEP_NUM_SECTORS; i++) {
        six_step_sector_control_t new_control = six_step_sector_negative_control_table[i];
        six_step_phase_control_action_E new_control_array[3] = {new_control.phase_a_action, new_control.phase_b_action,
                                                                new_control.phase_c_action};

        for (size_t j = 0; j < 3; j++) {
            // if the prev control action is HIGH, the new control action cannot be low
            // if the prev control action is LOW, the new control action cannot be high
            // if the prev control action is FLOAT, the new control action cannot be float

            if (prev_control_array[j] == SIX_STEP_CONTROL_ACTION_HIGH) {
                CHECK(new_control_array[j] != SIX_STEP_CONTROL_ACTION_LOW);
            } else if (prev_control_array[j] == SIX_STEP_CONTROL_ACTION_LOW) {
                CHECK(new_control_array[j] != SIX_STEP_CONTROL_ACTION_HIGH);
            } else if (prev_control_array[j] == SIX_STEP_CONTROL_ACTION_FLOAT) {
                CHECK(new_control_array[j] != SIX_STEP_CONTROL_ACTION_FLOAT);
            }

            prev_control_array[j] = new_control_array[j];
        }
    }
}
