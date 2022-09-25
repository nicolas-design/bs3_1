#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#define A_x 10
#define B_y 12
#define AB 20

typedef struct data{
    int* arrA;
    int* arrB;
    int thread_num;
    int bY;
    int aX;

} data;



 void* halfSum(void* p){
    data* ptr = (data*)p;
    int n = ptr->bY;
    int x = ptr->aX;

    int* thread_sum = (int*) calloc(1, sizeof(int));

    for (int i = 0; i < AB; i++) {
        thread_sum[0] = thread_sum[0] + ptr->arrB[i + AB*n] * ptr->arrA[x + AB*i];
    }
    uintptr_t sum = thread_sum[0];
     free(thread_sum);
     return (void*)sum;
}


void printMatrix(int* arr, int x, int y){
    for (int i = 0; i < y; ++i) {
        for (int j = 0; j < x; ++j) {
            printf("%i \t", arr[j+x*i]);
        }
        printf("\n");
    }
    int max = arr[0];
    int min = arr[0];

    for (int i = 1; i < x*y; ++i) {
        if (arr[i]>max){
            max = arr[i];
        }
    }
    for (int i = 1; i < x*y; ++i) {
        if (arr[i]<min){
            min = arr[i];
        }
    }

    int sum = 0;
    for (int i = 0; i < x*y; ++i) {
        sum = sum + arr[i];
    }

    printf("\n Minimaler Wert: %i; Maximaler Wert: %i; Summe aller Werte: %i", min, max, sum);

 }


int main() {
    int* A = (int*) malloc((A_x * AB) * sizeof(int ));
    int* B = (int*) malloc((B_y * AB) * sizeof(int ));
    for (int i = 0; i < (A_x * AB); ++i) {
        A[i]=i;
    }
    for (int i = 0; i < (B_y * AB); ++i) {
        B[i]=i;
    }

    pthread_t* tid= (pthread_t*)malloc((A_x*B_y) * sizeof(pthread_t));

    data inpData[A_x * B_y];

    int yB = 0;
    int xA = 0;

    for (int i = 0; i < (A_x * B_y); ++i) {
        inpData[i].arrB = B;
        inpData[i].arrA = A;
        inpData[i].bY = yB;
        inpData[i].aX = xA;

        pthread_create(&tid[i], NULL, halfSum, &inpData[i]);

        xA++;
        if (xA==10){
            xA=0;
            yB++;
        }
    }

    int* C = (int*)malloc((A_x * B_y) * sizeof(int ));


    for (int i = 0; i < (A_x * B_y); ++i) {
        pthread_join(tid[i], (void**)&C[i]);
    }


    printMatrix(C, A_x, B_y);

    free(A);
    free(B);
    free(C);
    free(tid);

    return 0;
}
