#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define Item(X, I, J, NCOL)      X[((J) + (I) * (NCOL))]

extern void matmult(int* A, int* B, int* C, int l, int m, int n);
extern void matmult_v1(int* A, int* B, int* C, int l, int m, int n);
extern void matmult_v2(int* A, int* B, int* C, int l, int m, int n);
extern void init_to_zeros(int* C, int l, int n);
void populateMatrix(int *X, int nrow, int ncol);
void printMatrix(int *X, int nrow, int ncol);
int main()
{
    int l=10, m=8, n=6;
    int *A = (int*)malloc(l*m*sizeof(int));
    int *B = (int*)malloc(m*n*sizeof(int));
    int *C = (int*)malloc(l*n*sizeof(int));
    clock_t start,end;
    double exec_time;

    populateMatrix(A, l, m);
    populateMatrix(B, m, n);
    printf("matrix A:\n");

    printMatrix(A, l, m);
     printf("matrix B:\n");
    printMatrix(B, m, n);
    start = clock();
    matmult(A, B, C, l, m, n);
    end = clock();
    exec_time = ((double)end-start)/ CLOCKS_PER_SEC;
    printf("without threads: execution time = %lf\n",exec_time);
    printMatrix(C, l, n);
    start = clock();
    init_to_zeros(C,l,n);
    printMatrix(C, l, n);
    matmult_v1(A, B, C, l, m, n);
    end = clock();
    exec_time = ((double)end-start)/ CLOCKS_PER_SEC;
    printf("threads element by element : execution time = %lf\n",exec_time);
        printMatrix(C, l, n);
        start = clock();
    matmult_v2(A, B, C, l, m, n);
    end = clock();
    exec_time = ((double)end-start)/ CLOCKS_PER_SEC;
    printf("threads row by row : execution time = %lf\n",exec_time);
        printMatrix(C, l, n);

    free(A);
    free(B);
    free(C);

    return 0;
}


/*
 * fill in a matrix with random numbers
 */
void populateMatrix(int *X, int nrow, int ncol)
{
    for(int i=0; i<nrow; i++)
        for(int j=0; j<ncol; j++)
            Item(X, i, j, ncol) = (((long)X)
                                + ((long)&nrow)
                                + ((long)&nrow)
                                - (~(i*ncol+nrow)
                                   +(j*nrow+ncol))
                                ) %29311%10;
}



/*
 * prints a matrix on stdout
 */
void printMatrix(int *X, int nrow, int ncol)
{
    for(int i=0; i<nrow; i++)
    {
        for(int j=0; j<ncol; j++)
            printf("%d\t", Item(X, i, j, ncol));
        printf("\n");
    }
    printf("\n");
    printf("\n");
}
