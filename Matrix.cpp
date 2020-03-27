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

//global variables
//number of rows and columns for the 2 matrices
const int row1 = 250000, col1 = 3, row2 = 3, col2 = 7; 

// method stubs
double * multiply(int m1, int m2, double *mat1,
    int n1, int n2, double mat2[][col2]);


int main() {
    //variable dictionary:
    int i,j;  //loop variables
    double* mat1;  //first matrix (250000 x 3)
    double* mat3;  //matrix resulting from mat1*mat2 (250000 x 7)
    double incrVal; //value to increment matrix elements by
    int x = 0;  //value multiplied by incrVal - increments for every element
    //second matrix (3 x 7)
    double mat2[][col2] = { {0.90,0.92,0.94,0.96,1.0,2.0, 3.0},
    {0.85,0.88,0.91,0.94,1.0,2.0,3.0}, {0.75,0.8,0.85,0.9,1.0,2.0,3.0} };
    double startTime, stopTime; //keeps track of time multiplication took

    
        
        

    //standard initial output
    printf("David Dray ~ COMP 233 B ~ Spring 2020\n");
    printf("Matrix - Multiply Matrices together and "
        "compare\nthe time different thread counts "
        "take to get the answer\n");

    incrVal = .03125;

    mat1 = (double*)malloc(row1 * col1 * sizeof(double));
    for (i = 0; i < row1; i++) {
        for (j = 0; j < col1; j++) {
            *(mat1 + i * col1 + j) = incrVal *x++;
        }
    }
    

    /*printf("matrix1:\n");
    for (i = 0; i < row1; i++) {
        for (j = 0; j < col1; j++) {
            printf("%f ", *(mat1 + i * col1 + j));
        }
        printf("\n");
    }

    printf("matrix2:\n");
    for (i = 0; i < row2; i++) {
        for (j = 0; j < col2; j++)
        {
            printf("%f ", *(*(mat2 + i) + j));
        }
        printf("\n");
    }*/



    mat3 = (double*)malloc(row1 * col2 * sizeof(double));

    startTime = omp_get_wtime();
    mat3 = multiply(row1, col1, mat1, row2, col2, mat2);
    stopTime = omp_get_wtime();

    /*printf("matrix3:\n");
    for ( i = 0; i < row1; i++)
    {
        for (j = 0; j < col2; j++)
        {
            printf("%f ", *(mat3 + i * col2 + j));
        }
        printf("\n");
    }*/
    printf("Serial multiplication time: %f\n", stopTime - startTime);
    free(mat1);
    free(mat3);

    printf("\n\n\t<<Normal Termination>>\n\n");
    return 0;


}

double * multiply(int m1, int m2, double *mat1,
    int n1, int n2, double mat2[][col2]) {
    int x, i, j;
    double * res = (double*)malloc(m1 * n2 * sizeof(double));
    for (i = 0; i < m1; i++) {
        for (j = 0; j < n2; j++) {

            *(res+i*n2+j) = 0;
            for (x = 0; x < m2; x++) {
                *(res + i * n2 + j) += *(mat1 + i * n1 + x) *
                    *(*(mat2 + x) + j);
            }
        }
    }
    return res;
}
