/*
* David Dray ~ COMP 233 ~ Spr 2020
* File:    Matrix.c
* Purpose: Multiply matrices together and test the speed
* at which the multiplication runs with differing thread
* counts.
*/
#define _CRT_SECURE_NO_WARNINGS //iso standard is good for us
//include statements
#include<stdio.h> 


const int m1 = 2, m2 = 2, n1 = 2, n2 = 2;

// method stubs
void multiply(int m1, int m_2, int mat1[][m2],
    int n1, int n_2, int mat2[][n2]);

// Driver code 
int main() {
    //standard initial output
    printf("David Dray ~ COMP 233 B ~ Spring 2020\n");
    printf("Matrix - Multiply Matrices together and "
        "compare\nthe time different thread counts "
        "take to get the answer\n");
    int mat1[][2] = { { 2, 4 }, { 3, 4 } };
    int mat2[][2] = { { 1, 2 }, { 1, 3 } };
    multiply(m1, m2, mat1, n1, n2, mat2);
    return 0;

    printf("\n\n\t<<Normal Termination>>\n\n");

}

void multiply(int m_1, int m_2, int mat1[][m2],
    int n1, int n_2, int mat2[][n2]) {
    int x, i, j;
    int res[m1][n2];
    for (i = 0; i < m1; i++)
    {
        for (j = 0; j < n2; j++)
        {
            res[i][j] = 0;
            for (x = 0; x < m2; x++)
            {
                *(*(res + i) + j) += *(*(mat1 + i) + x) *
                    *(*(mat2 + x) + j);
            }
        }
    }
    for (i = 0; i < m1; i++)
    {
        for (j = 0; j < n2; j++)
        {
            printf("%d ", *(*(res + i) + j));
        }
        printf("\n");
    }
}
