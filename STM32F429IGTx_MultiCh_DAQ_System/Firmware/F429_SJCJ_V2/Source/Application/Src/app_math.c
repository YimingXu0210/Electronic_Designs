#include "app_math.h"
#include "app_uart.h"

// 生成高斯核
void generate_gaussian_kernel(double *kernel, int kernel_size, double sigma) {
    int center = kernel_size / 2;
    double sum = 0.0;
    
    for (int i = 0; i < kernel_size; i++) {
        kernel[i] = exp(-0.5 * pow((i - center) / sigma, 2));
        sum += kernel[i];
    }

    // 归一化，使核的和为1
    for (int i = 0; i < kernel_size; i++) {
        kernel[i] /= sum;
    }
}

// 对电流数据进行高斯滤波
void gaussian_filter(const double *data, double *output, int data_size, const double *kernel, int kernel_size) {
    int half_kernel = kernel_size / 2;
    
    for (int i = 0; i < data_size; i++) {
        output[i] = 0.0;
        
        for (int j = 0; j < kernel_size; j++) {
            int data_index = i + j - half_kernel;
            
            // 边界处理：超出边界的部分使用最接近的边界值
            if (data_index < 0) data_index = 0;
            else if (data_index >= data_size) data_index = data_size - 1;
            
            output[i] += data[data_index] * kernel[j];
        }
    }
}

void gauss(void) {
    // 模拟采集到的电流数据
	ps("1");
    double data[DATA_SIZE];
    double filtered_data[DATA_SIZE];
    
    // 生成模拟数据：正弦信号 + 噪声

    for (int i = 0; i < DATA_SIZE; i++) {
        data[i] = sin(2 * M_PI * 5 * i / DATA_SIZE) + 0.5 * ((double)rand() / RAND_MAX - 0.5);
    }

    // 生成高斯核
    double kernel[KERNEL_SIZE];
    generate_gaussian_kernel(kernel, KERNEL_SIZE, SIGMA);

    // 使用高斯滤波
    gaussian_filter(data, filtered_data, DATA_SIZE, kernel, KERNEL_SIZE);

    // 输出结果（可以根据需要进行进一步处理或可视化）
    for (int i = 0; i < DATA_SIZE; i++) {
        ps("{gauss}%f\r\n", filtered_data[i]);
    }

}

double App_Math_Poly_Eval(double coefficients[MAX_DEGREE + 1], int degree, double x) {
	double result = 0;
	for (int i = degree; i >= 0; i--) {
		result = result * x + coefficients[i];
	}
	return result;
}
