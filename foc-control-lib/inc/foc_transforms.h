#ifndef FOC_TRANSFORMS_H
#define FOC_TRANSFORMS_H

typedef struct {
    float a;
    float b;
    float c;
} abc_t;

typedef struct {
    float alpha;
    float beta;
} alpha_beta_t;

typedef struct {
    float d;
    float q;
} direct_quad_t;

/**
 * @brief Transform the given abc values to alpha-beta frame values
 *
 * @param i_a The a phase current
 * @param i_b The b phase current
 *
 * @return The transformed alpha-beta values
 * @note This uses the balanced system clark transform, where c = -(a + b)
 */
alpha_beta_t clarke_transform(float i_a, float i_b);

/**
 * @brief Transform the given alpha-beta values to dq frame values
 *
 * @param alpha_beta The alpha-beta values to transform
 * @param theta_el_rad The electrical angle of the rotor in radians (assumed to be in the range [0, 2*pi]).
 * @note The electrical angle is defined as the angle between the d-axis and the alpha-axis
 *
 * @return The transformed dq values
 */
direct_quad_t park_transform(alpha_beta_t alpha_beta, float theta_el_rad);

/**
 * @brief Transform the given alpha-beta values to abc frame values
 *
 * @param alpha_beta The alpha-beta values to transform
 *
 * @return The transformed abc values
 */
abc_t inverse_clarke_transform(alpha_beta_t alpha_beta);

/**
 * @brief Transform the given dq values to alpha-beta frame values
 *
 * @param dq The dq values to transform
 * @param theta_el_rad The electrical angle of the rotor in radians (assumed to be in the range [0, 2*pi]).
 * @note The electrical angle is defined as the angle between the d-axis and the alpha-axis
 *
 * @return The transformed alpha-beta values
 */
alpha_beta_t inverse_park_transform(direct_quad_t dq, float theta_el_rad);

#endif  // FOC_TRANSFORMS_H