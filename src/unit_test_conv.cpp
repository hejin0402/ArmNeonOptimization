#include <iostream>
#include <stdio.h>
// #define CATCH_CONFIG_MAIN
// #include <catch.h>
#include <convolution_3x3s1.h>
using namespace std;

bool cmp(float a, float b){
    if(fabs(a - b) < 0.0001){
        return true;
    }
    else{
        return false;
    }
}

float a[100]={-1.1268, -1.0129, -1.6802,  0.4670,  0.6153, 0.3901, -1.0640, -0.2936, -1.2315,  0.5493,
            -0.0420, -0.2721,  0.1954, -0.2216,  0.6879, -0.2050, -1.0803, -0.3176,  0.5296,  0.3935,
            2.0697,  0.5919,  0.6199, -0.0346, -0.7999, 0.2293, -1.5282,  1.0790, -0.2825,  1.0728,
            -0.4607, -0.7011, -0.2361,  0.2906,  0.1105, -0.9928, -0.5154,  0.7045, -0.9297,  0.7138,
            -0.3283,  0.2736, -0.8493, -0.7276,  2.4235, -0.6976,  1.2200, -0.4467,  1.7764, -0.0902,
            -0.5115,  0.7125,  2.6900,  1.6307, -1.0520, 0.8062, -1.5037, -0.2208, -0.3704, -0.0657,
            -0.7742, -0.5371,  0.1098, -1.2455, -1.6273, -0.7954, -0.0381, -0.9875,  0.6384,  0.0350,
            0.5414,  0.6222, -0.6658,  0.3772, -0.8883};

float b[100]={0.1554, -0.4298, -0.5700, -0.0541, -0.0924, -0.0473, 0.4419,  0.1781, -0.1656,
            0.5869,  0.1084, -0.4887, 0.2125,  0.2375, -0.1172, 0.3615,  0.4178, -0.2846,
            0.3886,  0.2065,  0.4286, 0.2377,  0.4176,  0.6548, -0.1712,  0.2952,  0.4079,
            1.4470,  1.5160,  0.2381, 0.4402,  0.4872,  0.4865, -0.1318, -0.4056, -0.5152};

float c[200]={-0.0073,  0.1526, -0.0270, -0.0072,  0.0659,  0.1265, 0.1657, -0.0500,  0.0438,
            -0.0798,  0.0382, -0.0959, -0.0398, -0.0483, -0.1100, 0.0023,  0.0489,  0.0612,
            -0.1398, -0.1093,  0.1475, 0.1383,  0.0858, -0.0167, -0.0942, -0.0131, -0.1723,
            0.0643,  0.1751, -0.0905, -0.1898, -0.0066,  0.0137, -0.0252,  0.0558,  0.1431};

int main(){
    const int inw = 5;
    const int inh = 5;
    const int inch = 3;
    const int kw = 3;
    const int kh = 3;
    const int outw = inw - kw + 1;
    const int outh = inh - kh + 1;
    const int outch = 4;

    //5x5x3
    float *src = new float[inw * inh * inch];
    //3x3x4
    float *kernel = new float[kw * kh * outch];
    //3x3x4
    float *dest = new float[outw * outh * outch];

    //赋值
    for(int i = 0; i < inw * inh * inch; i++){
        src[i] = a[i];
    }

    for(int i = 0; i < kw * kh * outch; i++){
        kernel[i] = b[i];
    }

    conv3x3s1_neon(src, inw, inh, inch, kernel, kw, kh, dest, outw, outh, outch);

    for(int i = 0; i < outw * outh * outch ; i++){
        bool flag = cmp(dest[i], c[i]);
        if(flag == false){
            printf("WA: %d\n", i);
            printf("Expected: %.5f, ConvOutput: %.5f\n", c[i], dest[i]);
        }
    }

    for(int i = 0; i < outw * outh * outch; i++){
        printf("%.5f ", dest[i]);
    }

    printf("\n");

    return 0;
}