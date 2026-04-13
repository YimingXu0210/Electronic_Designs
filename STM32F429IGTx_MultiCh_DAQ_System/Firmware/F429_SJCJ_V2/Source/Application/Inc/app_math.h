#ifndef __APP_MATH_H
#define __APP_MATH_H

#include "app_main.h"

#define DATA_SIZE 50
#define KERNEL_SIZE 11  // 必须是奇数
#define SIGMA 2.0       // 高斯标准差
#define MAX_DEGREE 	10  // 最大多项式的阶数

void generate_gaussian_kernel(double *kernel, int kernel_size, double sigma);
void gaussian_filter(const double *data, double *output, int data_size, const double *kernel, int kernel_size);
void gauss(void);
double App_Math_Poly_Eval(double coefficients[MAX_DEGREE + 1], int degree, double x);

#endif /* __APP_MATH_H */
