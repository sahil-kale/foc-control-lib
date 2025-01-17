#include "CppUTest/TestHarness.h"
#include "math.h"
#include "util.h"

extern "C" {
#include "foc_transforms.h"
}

TEST_GROUP(FocTransforms){void setup(){}

                          void teardown(){}};

TEST(FocTransforms, ClarkeTransform) {
    abc_t abc = {1.0F, -0.5F, -0.5F};
    alpha_beta_t expected;
    expected.alpha = cosf(DEG_TO_RAD(0.0F));
    expected.beta = sinf(DEG_TO_RAD(0.0F));

    alpha_beta_t calculated = clarke_transform(abc.a, abc.b);

    DOUBLES_EQUAL(expected.alpha, calculated.alpha, 1e-3);
    DOUBLES_EQUAL(expected.beta, calculated.beta, 1e-3);

    abc = {-0.5F, 1.0F, -0.5F};  // 120 degrees
    expected.alpha = cosf(DEG_TO_RAD(120.0F));
    expected.beta = sinf(DEG_TO_RAD(120.0F));

    calculated = clarke_transform(abc.a, abc.b);
    DOUBLES_EQUAL(expected.alpha, calculated.alpha, 1e-3);
    DOUBLES_EQUAL(expected.beta, calculated.beta, 1e-3);

    abc = {-0.5F, -0.5F, 1.0F};  // 240 degrees
    expected.alpha = cosf(DEG_TO_RAD(240.0F));
    expected.beta = sinf(DEG_TO_RAD(240.0F));

    calculated = clarke_transform(abc.a, abc.b);
    DOUBLES_EQUAL(expected.alpha, calculated.alpha, 1e-3);
    DOUBLES_EQUAL(expected.beta, calculated.beta, 1e-3);
}

TEST(FocTransforms, ParkTransform) {
    alpha_beta_t alpha_beta = {1.0F, 0.0F};
    direct_quad_t expected = {1.0F, 0.0F};

    direct_quad_t calculated = park_transform(alpha_beta, DEG_TO_RAD(0.0F));
    DOUBLES_EQUAL(expected.d, calculated.d, 1e-3);
    DOUBLES_EQUAL(expected.q, calculated.q, 1e-3);

    alpha_beta = {0.0F, 1.0F};
    expected = {0.0F, 1.0F};

    calculated = park_transform(alpha_beta, DEG_TO_RAD(0.0F));
    DOUBLES_EQUAL(expected.d, calculated.d, 1e-3);
    DOUBLES_EQUAL(expected.q, calculated.q, 1e-3);

    alpha_beta = {0.5F, 0.5F};
    expected = {0.5F, 0.5F};

    calculated = park_transform(alpha_beta, DEG_TO_RAD(0.0F));
    DOUBLES_EQUAL(expected.d, calculated.d, 1e-3);
    DOUBLES_EQUAL(expected.q, calculated.q, 1e-3);

    calculated = park_transform(alpha_beta, DEG_TO_RAD(90.0F));
    expected = {0.5F, -0.5F};
    DOUBLES_EQUAL(expected.d, calculated.d, 1e-3);
    DOUBLES_EQUAL(expected.q, calculated.q, 1e-3);

    calculated = park_transform(alpha_beta, DEG_TO_RAD(180.0F));
    expected = {-0.5F, -0.5F};
    DOUBLES_EQUAL(expected.d, calculated.d, 1e-3);
    DOUBLES_EQUAL(expected.q, calculated.q, 1e-3);

    calculated = park_transform(alpha_beta, DEG_TO_RAD(270.0F));
    expected = {-0.5F, 0.5F};
    DOUBLES_EQUAL(expected.d, calculated.d, 1e-3);
    DOUBLES_EQUAL(expected.q, calculated.q, 1e-3);
}

TEST(FocTransforms, InverseClarkeTransform) {
    alpha_beta_t alpha_beta = {1.0F, 0.0F};
    abc_t expected = {1.0F, -0.5F, -0.5F};

    abc_t calculated = inverse_clarke_transform(alpha_beta);
    DOUBLES_EQUAL(expected.a, calculated.a, 1e-3);
    DOUBLES_EQUAL(expected.b, calculated.b, 1e-3);
    DOUBLES_EQUAL(expected.c, calculated.c, 1e-3);

    alpha_beta = {0.0F, 1.0F};
    expected = {0.0F, 1.0F, -1.0F};
}

TEST(FocTransforms, InverseParkTransform) {
    direct_quad_t dq = {1.0F, 0.0F};
    alpha_beta_t expected = {1.0F, 0.0F};

    alpha_beta_t calculated = inverse_park_transform(dq, DEG_TO_RAD(0.0F));
    DOUBLES_EQUAL(expected.alpha, calculated.alpha, 1e-3);
    DOUBLES_EQUAL(expected.beta, calculated.beta, 1e-3);

    dq = {0.0F, 1.0F};
    expected = {-1.0F, 0.0F};

    calculated = inverse_park_transform(dq, DEG_TO_RAD(90.0F));
    DOUBLES_EQUAL(expected.alpha, calculated.alpha, 1e-3);
    DOUBLES_EQUAL(expected.beta, calculated.beta, 1e-3);
}