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

//global variables
//number of rows and columns for the 2 matrices
const int row1 = 2, col1 = 2, row2 = 2, col2 = 2; 

// method stubs
double * multiply(int m1, int m2, double *mat1,
    int n1, int n2, double *mat2);


int main() {
    //variable dictionary:
    int i,j,x;  //loop variables
    double* mat1;  //first matrix (250000 x 3)
    double* mat2;  //second matrix (3 x 7)
    double* mat3;  //matrix resulting from mat1*mat2 (250000 x 7)

    //standard initial output
    printf("David Dray ~ COMP 233 B ~ Spring 2020\n");
    printf("Matrix - Multiply Matrices together and "
        "compare\nthe time different thread counts "
        "take to get the answer\n");

    mat1 = (double*)malloc(row1 * col1 * sizeof(double));
    for (i = 0; i < row1; i++) {
        for (j = 0; j < col1; j++) {
            *(mat1 + i * col1 + j) = i+j;
        }
    }

    mat2 = (double*)malloc(row2 * col2 * sizeof(double));
    for (i = 0; i < row2; i++) {
        for (j = 0; j < col2; j++) {
            *(mat2 + i * col2 + j) = i+j;
        }
    }

    printf("matrix1:\n");
    for (i = 0; i < row1; i++) {
        for (j = 0; j < col2; j++) {
            printf("%f ", *(mat1 + i * col2 + j));
        }
        printf("\n");
    }
    printf("matrix2:\n");
    for (i = 0; i < row1; i++) {
        for (j = 0; j < col2; j++) {
            printf("%f ", *(mat2 + i * col2 + j));
        }
        printf("\n");
    }



    mat3 = (double*)malloc(row1 * col2 * sizeof(double));
    
    mat3 = multiply(row1, col1, mat1, row2, col2, mat2);
    printf("matrix2:\n");
    for ( i = 0; i < row1; i++)
    {
        for (j = 0; j < col2; j++)
        {
            printf("%f ", *(mat3 + i * col2 + j));
        }
        printf("\n");
    }
    return 0;

    printf("\n\n\t<<Normal Termination>>\n\n");

}

double * multiply(int m1, int m2, double *mat1,
    int n1, int n2, double *mat2) {
    int x, i, j;
    double * res = (double*)malloc(m1 * n2 * sizeof(double));
    for (i = 0; i < m1; i++) {
        for (j = 0; j < n2; j++) {

            *(res+i*n2+j) = 0;
            for (x = 0; x < m2; x++) {
                *(res + i * n2 + j) += *(mat1 + i * n1 + x) *
                    *(mat2 + x*n2 + j);
            }
        }
    }
    return res;
}
