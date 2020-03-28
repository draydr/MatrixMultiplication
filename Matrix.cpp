/*
* David Dray ~ COMP 233 ~ Spr 2020
* File:    Matrix.c
* Purpose: Multiply matrices together and test the speed
* at which the multiplication runs with differing thread
* counts.
*
* Portions of code were taken (and adapted) from
* https://www.geeksforgeeks.org/c-program-multiply-two-matrices/
* (written by Aditya Ranjan)
*/
#define _CRT_SECURE_NO_WARNINGS //iso standard is good for us
//include statements
#include<stdio.h> 
#include<stdlib.h>
#include<omp.h>

// method stubs
double* multiply(int m1, int m2, double *mat1,
    int n1, int n2, double *mat2);
double* parallelMultiply(int mat1Row, int mat1Col, double* mat1,
    int mat2Row, int mat2Col, double* mat2);

int main() {
    //variable dictionary:
    //number of rows and columns for the 2 matrices
    const int row1 = 250000, col1 = 3, row2 = 3, col2 = 7;
    int r,c;  //loop variables
    double* mat1;  //first matrix (250000 x 3)
    double* mat2;  //seciond matrix (3 x 7)
    double* mat3;  //matrix resulting from mat1*mat2 (250000 x 7)
    double incrVal; //value to increment matrix elements by
    int x = 0; //value multiplied by incrVal -increments for every element
    //second matrix (3 x 7)
    double startTime, stopTime; //keeps track of time multiplication took
    double serialTime, pTime;   //stores serial and parallel times
    
    //standard initial output
    printf("David Dray ~ COMP 233 B ~ Spring 2020\n");
    printf("Matrix - Multiply Matrices together and "
        "compare\nthe time different thread counts "
        "take to get the answer\n");

    incrVal = .03125;

    mat1 = (double*)malloc(row1 * col1 * sizeof(double));
    for (r = 0; r < row1; r++) {
        for (c = 0; c < col1; c++) {
            *(mat1 + r * col1 + c) = incrVal *x++;
        }
    }
   
    mat2 = (double*)malloc(row1 * col1 * sizeof(double));
    //initialize element's values
    for (c = 0; c < col2 - 3; c++) {
        *(mat2 + 0 * col2 + c) = 0.90 + (c * .02);
    }
    for (c = 0; c < col2 - 3; c++) {
        *(mat2 + 1 * col2 + c) = 0.85 + (c * .03);
    }
    for (c = 0; c < col2 - 3; c++) {
        *(mat2 + 2 * col2 + c) = 0.75 + (c * .05);
    }

    for (r = 0; r < row2; r++) {
        for (c = 1; c < col2-3; c++) {
            *(mat2 + r * col2 + (c+3)) = c;
        }
    }

    mat3 = (double*)malloc(row1 * col2 * sizeof(double));

    startTime = omp_get_wtime();
    multiply(row1, col1, mat1, row2, col2, mat2);
    stopTime = omp_get_wtime();
    serialTime = stopTime - startTime;

    startTime = omp_get_wtime();
    mat3 = parallelMultiply(row1, col1, mat1, row2, col2, mat2);
    stopTime = omp_get_wtime();
    pTime = stopTime - startTime;

    /*printf("\n\nmatrix3:\n");
    for ( r = 0; r < row1; r++)
    {
        for (c = 0; c < col2; c++)
        {
            printf("%f ", *(mat3 + r * col2 + c));
        }
        printf("\n");
    }*/

    printf("Serial multiplication time: %f\n", serialTime);
    printf("Parallel multiplication time: %f\n", pTime);
    free(mat1);
    free(mat3);

    printf("\n\n\t<<Normal Termination>>\n\n");
    return 0;


}

double * multiply(int mat1Row, int mat1Col, double *mat1,
    int mat2Row, int mat2Col, double *mat2) {
    int x, r, c;
    double * res = (double*)malloc(mat1Row * mat2Col * sizeof(double));

    for (c = 0; c < mat2Col; c++) {
        for (r = 0; r < mat1Row; r++) {
            *(res+ r* mat2Col+c) = 0;
            for (x = 0; x < mat1Col; x++) {
                *(res + r * mat2Col + c) += *(mat1 + r * mat1Col + x) *
                    *(mat2 + x * mat2Col + c);
            }
        }
    }
    return res;
}

double* parallelMultiply(int mat1Row, int mat1Col, double* mat1,
    int mat2Row, int mat2Col, double* mat2) {
    int x, r, c;
    double* res = (double*)malloc(mat1Row * mat2Col * sizeof(double));

#pragma omp parallel num_threads(2)
    {
        int threadNum = omp_get_num_threads();  //how many?
        int threadId = omp_get_thread_num();    //who am I?
        for (c = 0; c < mat2Col; c++) {
            for (r = threadId; r < mat1Row; r+=threadNum) {
                *(res + r * mat2Col + c) = 0;
                for (x = 0; x < mat1Col; x++) {
                    *(res + r * mat2Col + c) += *(mat1 + r * mat1Col + x) *
                        *(mat2 + x * mat2Col + c);
                }
            }
        }
    }
    return res;
}
