#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "matrix_multiply_origin.h"
#include "dclock.h"
using namespace std;


#define A( i, j ) a[ (i)*lda + (j) ]
#define B( i, j ) b[ (i)*ldb + (j) ]
#define abs( x ) ( (x) < 0.0 ? -(x) : (x) )

void random_matrix( int m, int n, float *a, int lda )
{
  double drand48();

  for(int i=0; i<m; i++){
    for(int j=0; j<n; j++){
        A(i, j) = (float)drand48();
    }
  }
}

void copy_matrix(int m, int n, float *a, int lda, float *b, int ldb)
{
  int i, j;

  for(int i=0; i<m; i++){
    for(int j=0; j<n; j++){
        B(i, j) = A(i, j);
    }
  }
}

float compare_matrices( int m, int n, float *a, int lda, float *b, int ldb )
{

  float max_diff = 0.0, diff;
  for (int i=0; i<m; i++ ){
    for (int j=0; j<n; j++ ){
        diff = abs(A(i, j) - B(i, j));
        
        max_diff = max(diff, max_diff);

        if(max_diff > 0.5f || max_diff < -0.5f) {
            printf("\n error: i %d  j %d diff %f", i, j, max_diff);
        }
    }
  }
  return max_diff;
}

int m, n, k, lda, ldb, ldc;

double time_tmp, time_best, gflops, diff;

float *a, *b, *c, *prec, *nowc;    

int main(){

    for(int i = 40; i <= 800; i += 40){
        m = i;
        n = i;
        k = i;
        gflops = 2.0 * m * n * k * 1.0e-09;
        lda = m;
        ldb = n;
        ldc = k;
        a = (float *)malloc(lda * k * sizeof(float));
        b = (float *)malloc(ldb * n * sizeof(float));
        c = (float *)malloc(ldc * n * sizeof(float));
        prec = (float *)malloc(ldc * n * sizeof(float));
        nowc = (float *)malloc(ldc * n * sizeof(float));
        // 随机填充矩阵
        random_matrix(m, k, a, lda);
        random_matrix(k, n, b, ldb);
        random_matrix(m, n, prec, ldc);

        memset(prec, 0, ldc * n * sizeof(float));

        copy_matrix(m, n, prec, ldc, nowc, ldc);

        // 以nowc为基准，判断矩阵运行算结果是否正确
        MatrixMultiply(m, n, k, a, lda, b, ldb, nowc, ldc);

        // 循环20次，以最快的运行时间为结果
        for(int j=0; j < 20; j++){
            
            copy_matrix(m, n, prec, ldc, c, ldc);

            time_tmp = dclock();

            MatrixMultiply(m, n, k, a, lda, b, ldb, c, ldc);

            time_tmp = dclock() - time_tmp;

            if(j == 0)
                time_best = time_tmp;
            else
                time_best = min(time_best, time_tmp);
        }

        diff = compare_matrices(m, n, c, ldc, nowc, ldc);

        if(diff > 0.5f || diff < -0.5f){
            exit(0);
        }

        printf("%d %le %le \n", i, gflops / time_best, diff);
        fflush(stdout);

        free(a);
        free(b);
        free(c);
        free(prec);
        free(nowc);
    }
    return 0;
}