#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#define Item(X, I, J, NCOL)      X[((J) + (I) * (NCOL))]
typedef struct
{
    int row;
    int column;
    int* A;
    int* B;
    int* C;
    int l;
    int m;
    int n;
} mult; //struct to pass arguments to other function as pthread_create takes only one function parameter

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult(int* A, int* B, int* C, int l, int m, int n)
{
    for(int i=0; i<l; i++)
        for(int j=0; j<n; j++)
        {
            int sum = 0;
            for(int k=0; k<m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
        }
}



/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void * mult_by_element(void* mult_arguments)
{
    mult* arg = mult_arguments;
    int sum = 0;
    for(int k=0; k<arg->m ; k++) //only loop on rows of 2nd matrix
        sum += Item(arg->A, arg->row, k, arg->m) * Item(arg->B, k, arg->column, arg->n);
    Item(arg->C, arg->row, arg->column, arg->n) = sum;
    return 0;

}
void init_to_zeros(int* C, int l, int n){//function to reset all op elements to zero
 for(int i=0; i<l; i++)
        for(int j=0; j<n; j++)
        {
            Item(C, i, j, n) = 0;
        }

}
void matmult_v1(int* A, int* B, int* C, int l, int m, int n)
{  init_to_zeros(C,l,n); //reset op matrix
   mult* mult_arguments=(mult*)malloc(sizeof(mult)*l*n); //array of struct of size equal to number of op elements to pass tp pthread_create function
   pthread_t *thread = (pthread_t *)malloc(sizeof(pthread_t)*l*n);// array of thread ids of size equal to number of op elements

    int thread_number = 0;
    for(int i = 0; i < l ; i++)
    {
        for(int j= 0 ; j < n; j++) //loop for all op elements
        {   //add arguments to a struct to send to the element x element mult function
            mult_arguments[thread_number].A= A;
            mult_arguments[thread_number].B= B;
            mult_arguments[thread_number].C= C;
            mult_arguments[thread_number].l= l;
            mult_arguments[thread_number].m= m;
            mult_arguments[thread_number].n= n;
            mult_arguments[thread_number].row= i;
            mult_arguments[thread_number].column= j;
           if(pthread_create(&thread[thread_number],NULL,mult_by_element,&(mult_arguments[thread_number])))
           {perror("pthread_create() error");
           exit(1);}
             //create thread for each element
            thread_number++;
        }
    }
    //creating all before joining then joining all at once tp allow concurrency
    for(int i=0; i< l*n; i++)
    {
        if(pthread_join(thread[i],NULL))
         {perror("pthread_create() error");
         exit(3);}

    }

}

void * mult_by_row(void* mult_arguments)
{
    mult* arg = mult_arguments;

    for(int j=0; j< arg->n; j++)
    {
        int sum = 0;
        for(int k=0; k<arg->m ; k++)
            sum += Item(arg->A, arg->row, k, arg->m) * Item(arg->B, k, j, arg->n);
        Item(arg->C, arg->row, j, arg->n) = sum;
    }
    //free(arg);
    return 0;

}

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult_v2(int* A, int* B, int* C, int l, int m, int n)
{
   init_to_zeros(C,l,n);  //reset op matrix
     mult* mult_arguments=(mult*)malloc(sizeof(mult)*l); //array of struct of size equal to number of op rows to pass tp pthread_create function
   pthread_t *thread = (pthread_t *)malloc(sizeof(pthread_t)*l);//array of thread ids of size equal to number of op  rows
    int thread_number = 0;

    for(int i = 0; i < l ; i++)
    {   //add arguments to a struct to send to the row mult function

        mult_arguments[thread_number].A= A;
        mult_arguments[thread_number].B= B;
        mult_arguments[thread_number].C= C;
        mult_arguments[thread_number].l= l;
        mult_arguments[thread_number].m= m;
        mult_arguments[thread_number].n= n;
        mult_arguments[thread_number].row= i;
        if(pthread_create(&thread[thread_number],NULL,mult_by_row,&mult_arguments[thread_number])) //create thread for each element
        //create thread for each row
        {perror("pthread_create() error");
           exit(1);}
           thread_number++;

    }
    //creating all before joining then joining all at once tp allow concurrency
    for(int i=0; i< l; i++)
    {
        if(pthread_join(thread[i],NULL))
        {perror("pthread_create() error");
           exit(3);}
    }


}
