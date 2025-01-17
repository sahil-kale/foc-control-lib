#include "foc_transforms.h"

#include "cordic_trig_generated.h"
#include "util.h"

alpha_beta_t clarke_transform(float i_a, float i_b) {
    alpha_beta_t alpha_beta;
    alpha_beta.alpha = i_a;
    alpha_beta.beta = (ONE_OVER_SQRT_3 * i_a) + (i_b * 2.0F * ONE_OVER_SQRT_3);

    return alpha_beta;
}

direct_quad_t park_transform(alpha_beta_t alpha_beta, float theta_el_rad) {
    direct_quad_t ret;
    float sin_val = 0.0F;
    float cos_val = 0.0F;
    cordic_trig_get_sin_cos(theta_el_rad, &sin_val, &cos_val);

    ret.d = (alpha_beta.alpha * cos_val) + (alpha_beta.beta * sin_val);
    ret.q = -(alpha_beta.alpha * sin_val) + (alpha_beta.beta * cos_val);

    return ret;
}

abc_t inverse_clarke_transform(alpha_beta_t alpha_beta) {
    abc_t abc;
    abc.a = alpha_beta.alpha;
    abc.b = -0.5F * alpha_beta.alpha + 0.5F * SQRT_3 * alpha_beta.beta;
    abc.c = -0.5F * alpha_beta.alpha - 0.5F * SQRT_3 * alpha_beta.beta;

    return abc;
}

alpha_beta_t inverse_park_transform(direct_quad_t dq, float theta_el_rad) {
    alpha_beta_t ret;
    float sin_val = 0.0F;
    float cos_val = 0.0F;
    cordic_trig_get_sin_cos(theta_el_rad, &sin_val, &cos_val);

    ret.alpha = dq.d * cos_val - dq.q * sin_val;
    ret.beta = dq.d * sin_val + dq.q * cos_val;

    return ret;
}