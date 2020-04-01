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
    int n1, int n2, double *mat2,
    double* startTime, double* stopTime);
double* parallelMultiply(int mat1Row, int mat1Col, double* mat1,
    int mat2Row, int mat2Col, double* mat2,
    double* startTime, double* stopTime);

int main() {
    //variable dictionary:
    //number of rows and columns for the 2 matrices
    const int row1 = 250000, col1 = 3, row2 = 3, col2 = 7;
    int r,c;  //loop variables
    double* mat1;  //first matrix (250000 x 3)
    double* mat2;  //seciond matrix (3 x 7)
    double* mat3;  //matrix resulting from mat1*mat2 (250000 x 7)
    double incrVal; //value to increment matrix elements by
    int x = 0; //increments for every element (multiplied by incrVal)
    double startTime, stopTime; //keeps track of time multiplication took
    double serialTime, pTime;   //stores serial and parallel times
    FILE* fp;   //file variable for writing to a file

    //allocate memory
    mat1 = (double*)malloc(row1 * col1 * sizeof(double));
    mat2 = (double*)malloc(row1 * col1 * sizeof(double));

    incrVal = .03125;   //set the incrementing value for mat1

    //standard initial output
    printf("David Dray ~ COMP 233 B ~ Spring 2020\n");
    printf("Matrix - Multiply Matrices together and "
        "compare\nthe time different thread counts "
        "take to get the answer\n");

    //initialize matrix 1
    for (r = 0; r < row1; r++) {
        for (c = 0; c < col1; c++) {
            *(mat1 + r * col1 + c) = incrVal *x++;
        }
    }
   
    //initialize each element's value for matrix 2
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

    // run & time serial matrix multiplication
    multiply(row1, col1, mat1, row2, col2, mat2,
        &startTime, &stopTime);
    serialTime = stopTime - startTime;

    // run & time parallel matrix multiplication
    mat3 = parallelMultiply(row1, col1, mat1, row2, col2, mat2,
        &startTime, &stopTime);
    pTime = stopTime - startTime;

    
    //inform user of data outputting to file
    printf("\n\nPrinting results to Dray.csv in current directory\n");

    /* open the file for writing*/
    fp = fopen("Dray.csv", "w");

    //print name and info for file heading
    fprintf(fp,"David Dray ~ COMP 233 B\n");
        //print each of the product matrix's elements
        for ( r = 0; r < row1; r++) {
            for (c = 0; c < col2; c++) {
                fprintf(fp,"%.3f,", *(mat3 + r * col2 + c));
            }
            fprintf(fp,"\n");
        }
        //print times each multiplication method took
        fprintf(fp,"\nSerial multiplication time: %.3f\n", serialTime);
        fprintf(fp,"Parallel multiplication time: %.3f\n", pTime);

    /* close the file*/
    fclose(fp);

    //free memory
    free(mat1);
    free(mat2);
    free(mat3);
    //standard termination message
    printf("\n\n\t<<Normal Termination>>\n\n");
    return 0;


}

/************************************************************************
* Function:    multiply (serial)

* Purpose:     take in 2 matrices along with their sizes and multiply the
    the first by the 2nd, timing how long it takes

* Input args:  takes in the number of rows and columns of matrix 1,
    the number of rows and columns in matrix 2, the two 2D arrays 
    (our matrices), and pointers to the timing variables

* sources:     This multiplication method was manipulated but based off
    of Aditya Ranjan's algorithm (found in the link provided in the
    project description).
************************************************************************/
double * multiply(int mat1Row, int mat1Col, double *mat1,
    int mat2Row, int mat2Col, double *mat2,
    double* startTime, double* stopTime) {
    int x, r, c;    //loop variables
    //product of the two matrices' multiplication
    double * res = (double*)malloc(mat1Row * mat2Col * sizeof(double));

    *startTime = omp_get_wtime();       //start timing

    //loop through the columns of matrix 2 and the rows of matrix 1
        for (c = 0; c < mat2Col; c++) {
            for (r = 0; r < mat1Row; r++) {
            *(res+ r* mat2Col+c) = 0;   //set product's i,j element to 0

            //loop to get dot products for each i,j element
            //notice summing each mulitplication into product matrix
            for (x = 0; x < mat1Col; x++) {
                *(res + r * mat2Col + c) += *(mat1 + r * mat1Col + x) *
                    *(mat2 + x * mat2Col + c);
            }
        }
    }
    *stopTime = omp_get_wtime();        //stop timing

    return res;     //return the resulting 2d array (matrix)
}

/************************************************************************
* Function:    parallelMultiply

* Purpose:     take in 2 matrices along with their sizes and multiply the
    the first by the 2nd using cyclical parallel computing. It also times
    how long it takes.

* Input args:  takes in the number of rows and columns of matrix 1,
    the number of rows and columns in matrix 2, the two 2D arrays
    (our matrices), and pointers to the timing variables

* sources:     This multiplication method was manipulated but based off
    of Aditya Ranjan's algorithm (found in the link provided in the
    project description).
************************************************************************/
double* parallelMultiply(int mat1Row, int mat1Col, double* mat1,
    int mat2Row, int mat2Col, double* mat2,
    double * startTime, double * stopTime) {

    //product of the two matrices' multiplication
    double* res = (double*)malloc(mat1Row * mat2Col * sizeof(double));

    *startTime = omp_get_wtime();       //start timing

#pragma omp parallel num_threads(2) //give me 2 threads 
    {
        int x, r, c;    //loop control variables for each thread
        int threadNum = omp_get_num_threads();  //how many?
        int threadId = omp_get_thread_num();    //who am I?
        
        //loop through the rows of the large matrix 
        //(want to do this as infrequent as possible)
        for (r = threadId; r < mat1Row; r += threadNum) {
            //loop through the 2nd matrix's columns
            for (c = 0; c < mat2Col; c++) {
                *(res + r * mat2Col + c) = 0;   //set product's i,j to 0
                
                //loop to get dot products for each i,j element
                //notice summing each mulitplication into product matrix
                for (x = 0; x < mat1Col; x++) {
                    *(res + r * mat2Col + c) += *(mat1 +r * mat1Col + x)*
                        *(mat2 + x * mat2Col + c);
                }
            }
        }
    }
    *stopTime = omp_get_wtime();        //stop timing

    return res; //return the result matrix
}
